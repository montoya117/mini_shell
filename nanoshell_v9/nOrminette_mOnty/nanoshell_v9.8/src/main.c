/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:58:53 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 13:20:59 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	print_banner(void)
{
	printf("\n");
	printf(C);
	printf("         ██╗       ██╗       ███╗   ███╗    \n");
	printf("         ██║       ██║       ████╗ ████║    \n");
	printf("         ██║    ████████╗    ██╔████╔██║    \n");
	printf("    ██   ██║    ██╔═██╔═╝    ██║╚██╔╝██║    \n");
	printf("    ╚█████╔╝    ██████║      ██║ ╚═╝ ██║    \n");
	printf("     ╚════╝     ╚═════╝      ╚═╝     ╚═╝    \n");
	printf("                                             \n");
	printf("    ███████╗██╗  ██╗███████╗██╗     ██╗     \n");
	printf("    ██╔════╝██║  ██║██╔════╝██║     ██║     \n");
	printf("    ███████╗███████║█████╗  ██║     ██║     \n");
	printf("    ╚════██║██╔══██║██╔══╝  ██║     ██║     \n");
	printf("    ███████║██║  ██║███████╗███████╗███████╗\n");
	printf("    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
	printf("                                             \n");
	printf(RST"\n");
}

static void	execute_logic(char *line, t_data *data)
{
	t_token	*tokens;
	t_token	*head;
	t_ast	*tree;

	tokens = tokenizer(line, data->last_status, data);
	if (!tokens)
		return ;
	head = tokens;
	tree = parser(&tokens);
	if (tree)
	{
		data->last_status = exec_ast(tree, data);
		ast_free(tree);
	}
	else
		data->last_status = 2;
	free_tokens(head);
}

static void	run_non_interactive(t_data *data)
{
	char	*line;
	int		len;

	while (data->running == 0)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		execute_logic(line, data);
		free(line);
	}
}

static void	run_interactive(t_data *data)
{
	char	*line;
	char	*prompt;

	print_banner();
	setup_signals();
	while (data->running == 0)
	{
		prompt = build_prompt(data);
		if (prompt)
			line = readline(prompt);
		else
			line = readline("minishell$ ");
		if (prompt)
			free(prompt);
		if (g_signal_received == SIGINT)
		{
			data->last_status = 130;
			g_signal_received = 0;
			if (!line)
			{
				write(1, "\n", 1);
				continue ;
			}
		}
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			execute_logic(line, data);
		}
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	data.envp = dup_env(envp);
	read_history(".nanoshell_history");
	if (isatty(STDIN_FILENO))
		run_interactive(&data);
	else
		run_non_interactive(&data);
	write_history(".nanoshell_history");
	rl_clear_history();
	free_env(data.envp);
	return (data.last_status);
}

/*

** Logic: Tokenize -> Parse -> Execute
**  Guardamos 'head' porque el parser mueve el puntero 'tokens'.

static void	execute_logic(char *line, t_data *data)
{
	t_token	*tokens;
	t_token	*head;
	t_ast	*tree;

	tokens = tokenizer(line, data->last_status, data);
	if (!tokens)
		return ;
	head = tokens;
	tree = parser(&tokens);
	if (tree)
	{
		data->last_status = exec_ast(tree, data);
		ast_free(tree);
	}
	else
		data->last_status = 2; // Syntax error code
	free_tokens(head);
}

static void	run_non_interactive(t_data *data)
{
	char	*line;
	int		len;

	while (data->running == 0)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		// Si es solo Enter, liberar y siguiente
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		// Limpiar el \n del final para el tokenizer
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		execute_logic(line, data);
		free(line);
	}
}

static void	run_interactive(t_data *data)
{
	char	*line;
	char	*prompt;

	// print_banner(); // Opcional
	setup_signals(); // Asegúrate de que usa rl_on_new_line
	while (data->running == 0)
	{
		prompt = build_prompt(data);
		line = readline(prompt ? prompt : "minishell$ ");
		if (prompt)
			free(prompt);

		// 1. Manejo de Ctrl+C
		if (g_signal_received == SIGINT)
		{
			data->last_status = 130;
			g_signal_received = 0;
			// CRITICO: Si readline devolvió NULL por Ctrl+C, no debemos salir.
			if (!line)
			{
				write(1, "\n", 1); // Salto de línea estético
				continue ; // Volvemos al inicio del while (nuevo prompt)
			}
		}

		// 2. Manejo de Ctrl+D (EOF real)
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}

		// 3. Ejecución
		if (line[0] != '\0')
		{
			add_history(line);
			execute_logic(line, data);
		}
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	data.envp = dup_env(envp);
	
	// Bonus
	read_history(".nanoshell_history");
	
	if (isatty(STDIN_FILENO))
		run_interactive(&data);
	else
		run_non_interactive(&data);
	
	// Limpieza
	write_history(".nanoshell_history");
	rl_clear_history();
	free_env(data.envp);
	
	return (data.last_status);
}
*/
/*

MAIN VIEJO --->

int	main(int ac, char **argv, char **envp)
{
	t_data	data;
	char	*line;
	t_token	*tokens;
	t_token	*tokens_head;
	t_ast	*tree;
	char    *prompt;
	char 	*prompt_to_use; 

	// Initialize core state
	line = NULL;
	data.last_status = 0;
	data.running = 0;
	data.cmd = NULL;
	data.exit = "exit";
	data.envp = dup_env(envp);
	data.pipes = NULL;
	data.pipes_count = 0;

	// Setup signals for Ctrl-C, Ctrl-
	rl_catch_signals = 0;
	setup_signals();
	// Set History
	read_history(".nanoshell_history");

	// 1) -c MODE: tester compatibility
	if (ac >= 3 && strcmp(argv[1], "-c") == 0)
	{
		line = argv[2];  // the whole command string: "echo hello world"
		tokens = tokenizer(line, data.last_status, &data);
		if (tokens)
		{
			tokens_head = tokens;
			tree = parser(&tokens);
			if (tree)
			{
				data.last_status = exec_ast(tree, &data);
				ast_free(tree);
			}
			else
			{
				// parser error -> status 2 como bash
				data.last_status = 2;
			}
			free_tokens(tokens_head);
		}
		free_env(data.envp);
		return (data.last_status);
	}
	// 2) NON‑INTERACTIVE: stdin (pipes/files)
	if (!isatty(STDIN_FILENO))
	{
		while ((line = get_next_line(STDIN_FILENO)))
		{
			if (line[0] == '\n')
			{
				free(line);
				continue;
			}
			tokens = tokenizer(line, data.last_status, &data);
			if (!tokens)
			{
				free(line);
				continue;
			}
			tokens_head = tokens;
			tree = parser(&tokens);
			if (tree)
			{
				data.last_status = exec_ast(tree, &data);
				ast_free(tree);
			}
			else
			{
				data.last_status = 2;
			}
			free_tokens(tokens_head);
			free(line);
		}
		write_history(".nanoshell_history");
		free_env(data.envp);
		return (data.last_status);
	}
	// 3) INTERACTIVE: your readline loop ... 
	// Print banner
	print_banner();
	// Shell loop
	while (data.running == 0)
	{
		prompt = build_prompt(&data);
		if (prompt != NULL)
			prompt_to_use = prompt;
		else
			prompt_to_use = "$> ";
		line = readline(prompt_to_use);
		  if (prompt != NULL)
		{
			free(prompt);
			prompt = NULL;
		}
		if (line == NULL)
		{
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			data.last_status = 130;   // valor de $? tras Ctrl+C en prompt
			g_signal_received = 0;    // limpiar para el siguiente ciclo
			// IMPORTANTE: no hacer free(line) 
		}
		if (line[0] != '\0')
		{
			add_history(line);
		}
		data.cmd = line;
		if (strcmp(data.cmd, data.exit) == 0) //autorizada????
		{
			data.running = 1;
			free(line);
			break ;
		}
		// tokeniser test
		tokens = tokenizer(line, data.last_status, &data);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		//tokens_print_simple(tokens);

		// Guardar el head original para poder liberarlo tras el parser
		tokens_head = tokens;
		// parser & AST
		tree = parser(&tokens);
		if (tree)
		{
			//ast_print(tree, 0);
			data.last_status = exec_ast(tree, &data);
			ast_free(tree);
		}
		else
			{
				data.last_status = 2;
			}
		free_tokens(tokens_head);
		free(line); 
	}
	write_history(".nanoshell_history"); // Save session history
	rl_clear_history(); // libera historial de readline 
	free_env(data.envp);
	return (data.last_status);
}
*/