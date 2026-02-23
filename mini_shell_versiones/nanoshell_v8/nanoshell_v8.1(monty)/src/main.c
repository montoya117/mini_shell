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

/*
static void	print_banner(void)
{
	printf("\n");
	printf(G);
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
*/

static void	print_banner(void)
{
	printf("\n");
	printf(C);
	printf(" ███╗   ███╗██╗███╗   ██╗██╗        ███████╗██╗  ██╗███████╗██╗     ██╗     \n");
	printf(" ████╗ ████║██║████╗  ██║██║        ██╔════╝██║  ██║██╔════╝██║     ██║     \n");
	printf(" ██╔████╔██║██║██╔██╗ ██║██║        ███████╗███████║█████╗  ██║     ██║     \n");
	printf(" ██║╚██╔╝██║██║██║╚██╗██║██║        ╚════██║██╔══██║██╔══╝  ██║     ██║     \n");
	printf(" ██║ ╚═╝ ██║██║██║ ╚████║██║███████╗███████║██║  ██║███████╗███████╗███████╗\n");
	printf(" ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
	printf(RST"\n");
}

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

	/* 1) -c MODE: tester compatibility */
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
            free_tokens(tokens_head);
        }
        free_env(data.envp);
        return (data.last_status);
    }
	/* 2) NON‑INTERACTIVE: stdin (pipes/files) */
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
            free_tokens(tokens_head);
            free(line);
        }
        write_history(".nanoshell_history");
        free_env(data.envp);
        return (data.last_status);
    }
	/* 3) INTERACTIVE: your readline loop ... */
	// Print banner
    print_banner();
	// Shell loop
	while (data.running == 0)
	{
        prompt = build_prompt(&data);
		if (prompt != NULL)
            prompt_to_use = prompt;
        else
            prompt_to_use = "$ ";
		line = readline(prompt_to_use);
		if (!line)
    		break;
		printf("DBG LINE: [%s]\n", line);
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
		if (ft_strcmp(data.cmd, data.exit) == 0) //autorizada????
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
		free_tokens(tokens_head);
        free(line); 
	}
	write_history(".nanoshell_history"); // Save session history
	rl_clear_history(); /* libera historial de readline */
	free_env(data.envp);
	return (data.last_status);
}
