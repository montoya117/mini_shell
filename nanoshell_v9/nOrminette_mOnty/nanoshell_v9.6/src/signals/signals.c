/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:22:27 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:22:29 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 700
#include <signal.h>
#include <string.h>
#include "nanoshell.h"

volatile sig_atomic_t	g_signal_received = 0;

static void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_signal_received = signum;
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction SIGINT");
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
	}
}
