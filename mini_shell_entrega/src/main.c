/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:58:53 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/11 14:23:35 by alemonto         ###   ########.fr       */
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
	printf("                                            \n");
	printf("    ███████╗██╗  ██╗███████╗██╗     ██╗     \n");
	printf("    ██╔════╝██║  ██║██╔════╝██║     ██║     \n");
	printf("    ███████╗███████║█████╗  ██║     ██║     \n");
	printf("    ╚════██║██╔══██║██╔══╝  ██║     ██║     \n");
	printf("    ███████║██║  ██║███████╗███████╗███████╗\n");
	printf("    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
	printf("                                            \n");
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
	tree = parser(&tokens, data);
	if (!tree && g_signal_received == SIGINT)
	{
		data->last_status = 130;
		g_signal_received = 0;
	}
	else if (tree)
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

	print_banner();
	setup_signals();
	while (data->running == 0)
	{
		line = get_input_line(data);
		if (handle_signal_interrupt(data, line))
			continue ;
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			signal(SIGINT, SIG_IGN);
			execute_logic(line, data);
			setup_signals();
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
	rl_catch_signals = 0;
	g_signal_received = 0;
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
