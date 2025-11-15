/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:58:53 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/08 13:36:40 by jadelgad         ###   ########.fr       */
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

int	main(int ac, char **av, char **ev)
{
	t_data	data;
	char *line;
	(void)ac;
	(void)av;
	(void)ev;

	// 1. Initialize core state
	line = NULL;
	data.status = 0;
	data.cmd = NULL;
	data.exit = "exit";
	
	// 2. Setup signals for Ctrl-C, Ctrl-
	rl_catch_signals = 0;
	setup_signals();
	
	// 3. Set History
    read_history(".nanoshell_history");
    
	// 4. Print banner
    print_banner();
	// 5. Shell loop
	while (data.status == 0)
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
			data.status = 1;
			free(line);
			break ;
		}
		// tokeniser
		t_token	*tokens = tokenizer(line);
		tokens_print_simple(tokens); // print for test
		
		 // Test commands only 
		 t_ast *cmd = parser_commands(tokens);
		 if (cmd)
		 {
			ast_print(cmd, 0);
			ast_free(cmd);
		 } 
		// free stufff
		free_tokens(tokens);
        free(line);
        
	}
	write_history(".nanoshell_history"); // Save session history
	return (0);
}


/*
    char *intro =
"      ▓█████  ███▄ ▄███▓ ▒█████    ██████  ██▓▓█████▄  ▒█████\n"
"      ▓█   ▀ ▓██▒▀█▀ ██▒▒██▒  ██▒▒██    ▒ ▓██▒▒██▀ ██▌▒██▒  ██▒           \n"
"      ▒███   ▓██    ▓██░▒██░  ██▒░ ▓██▄   ▒██▒░██   █▌▒██░  ██▒           \n"
"      ▒▓█  ▄ ▒██    ▒██ ▒██   ██░  ▒   ██▒░██░░▓█▄   ▌▒██   ██░           \n"
"      ░▒████▒▒██▒   ░██▒░ ████▓▒░▒██████▒▒░██░░▒████▓ ░ ████▓▒░           \n"
"      ░░ ▒░ ░░ ▒░   ░  ░░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░░▓   ▒▒▓  ▒ ░ ▒░▒░▒░            \n"
"       ░ ░  ░░  ░      ░  ░ ▒ ▒░ ░ ░▒  ░ ░ ▒ ░ ░ ▒  ▒   ░ ▒ ▒░            \n"
"         ░   ░      ░   ░ ░ ░ ▒  ░  ░  ░   ▒ ░ ░ ░  ░ ░ ░ ░ ▒             \n"
"         ░  ░       ░       ░ ░        ░▀▀█▄    ░        ░ ░             \n"
"▓█████  ███▄    █   ▄████  ▄▄▄       ███▄    █ ░▄▄▄      ▓█████▄  ▒█████  \n"
"▓█   ▀  ██ ▀█   █  ██▒ ▀█▒▒████▄     ██ ▀█   █ ▒████▄    ▒██▀ ██▌▒██▒  ██▒\n"
"▒███   ▓██  ▀█ ██▒▒██░▄▄▄░▒██  ▀█▄  ▓██  ▀█ ██▒▒██  ▀█▄  ░██   █▌▒██░  ██▒\n"
"▒▓█  ▄ ▓██▒  ▐▌██▒░▓█  ██▓░██▄▄▄▄██ ▓██▒  ▐▌██▒░██▄▄▄▄██ ░▓█▄   ▌▒██   ██░\n"
"░▒████▒▒██░   ▓██░░▒▓███▀▒ ▓█   ▓██▒▒██░   ▓██░ ▓█   ▓██▒░▒████▓ ░ ████▓▒░\n"
"░░ ▒░ ░░ ▒░   ▒ ▒  ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ▒ ▒  ▒▒   ▓▒█░ ▒▒▓  ▒ ░ ▒░▒░▒░ \n"
" ░ ░  ░░ ░░   ░ ▒░  ░   ░   ▒   ▒▒ ░░ ░░   ░ ▒░  ▒   ▒▒ ░ ░ ▒  ▒   ░ ▒ ▒░ \n"
"   ░      ░   ░ ░ ░ ░   ░   ░   ▒      ░   ░ ░   ░   ▒    ░ ░  ░ ░ ░ ░ ▒  \n"
"   ░  ░         ░       ░       ░  ░         ░       ░  ░   ░        ░ ░  \n"
"                                                          ░               \n";
*/
