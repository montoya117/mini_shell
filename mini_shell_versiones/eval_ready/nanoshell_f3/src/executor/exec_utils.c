/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:18:42 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/19 15:34:12 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	exec_error(const char *message, const char *subject)
{
	const char	*prefix;
	const char	*err;

	prefix = "minishell: ";
	write(2, prefix, ft_strlen_const(prefix));
	if (subject)
		write(2, subject, ft_strlen_const(subject));
	if (message && ft_strncmp(message, "command not found", 18) == 0)
	{
		write(2, ": command not found\n", 21);
		return ;
	}
	write(2, ": ", 2);
	if (errno != 0)
	{
		err = strerror(errno);
		if (err)
			write(2, err, ft_strlen_const(err));
		write(2, "\n", 1);
		return ;
	}
	if (message)
		write(2, message, ft_strlen_const(message));
	write(2, "\n", 1);
}

