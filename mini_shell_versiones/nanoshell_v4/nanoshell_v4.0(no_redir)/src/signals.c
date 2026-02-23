/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:49:50 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:49:56 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 700
#include <signal.h>
#include <string.h>
#include "nanoshell.h"

volatile sig_atomic_t g_signal_received = 0;

static void signal_handler(int signum) 
{
    if (signum == SIGINT)
    {
    	write(1, "\n", 1);		// move to a new line
    	rl_replace_line("", 0);  // Clear previous input text
    	rl_on_new_line();         // Move to new line
    	rl_redisplay();           // Redisplay prompt
    }
    g_signal_received = signum;  // Store the signal number
}

void    setup_signals(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0; // default behavior
    sigemptyset(&sa.sa_mask); // don't block any signals in handler

    if (sigaction(SIGINT, &sa, NULL) == -1) 
    {
        perror("sigaction SIGINT");
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1) 
    {
        perror("sigaction SIGQUIT");
    }
}
