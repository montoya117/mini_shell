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
	char *line;
	(void)ac;
	(void)argv;	

	// 1. Initialize core state
	line = NULL;
	data.last_status = 0;
	data.running = 0;
	data.cmd = NULL;
	data.exit = "exit";
	data.envp = dup_env(envp);
	data.pipes = NULL;
	data.pipes_count = 0;
	// 2. Setup signals for Ctrl-C, Ctrl-
	rl_catch_signals = 0;
	setup_signals();
	
	// 3. Set History
    read_history(".nanoshell_history");
    
	// 4. Print banner
    print_banner();
	// 5. Shell loop
	while (data.running == 0)
	{
		line = readline("$> ");
		// Handle signal flag if set
		if (g_signal_received == SIGINT) 
		{
    		// Handle Ctrl-C: redisplay prompt
    		g_signal_received = 0;
    		free(line);
			continue ;
		}
		else if (g_signal_received == SIGQUIT) 
		{
    		// Handle Ctrl-\: IGNORE at prompt, just reset and do nothing
    		g_signal_received = 0;
    		free(line);
    		continue ;
		}
		if (line == NULL)
		{
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
		}
		data.cmd = line;
		if (strcmp(data.cmd, data.exit) == 0)
		{
			data.running = 1;
			free(line);
			break ;
		}
		// tokeniser test
		t_token	*tokens = tokenizer(line, data.last_status, &data);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		//tokens_print_simple(tokens);

		// Guardar el head original para poder liberarlo tras el parser
		t_token *tokens_head = tokens;
		// parser & AST
		t_ast *tree = parser(&tokens);
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
	free(data.envp);
	return (0);
}
