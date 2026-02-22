/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:18:22 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/19 15:30:14 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	open_redirect(t_ast *redir)
{
	int	fd;

	fd = -1;
	if (redir->redirect_type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redirect_type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->redirect_type == TOKEN_REDIR_IN
		|| redir->redirect_type == TOKEN_HEREDOC)
		fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	return (fd);
}

int	execute_redirection_list(t_ast **reds, int n)
{
	int	fd;
	int	i;

	i = n - 1;
	while (i >= 0)
	{
		fd = open_redirect(reds[i]);
		if (fd < 0)
			return (-1);
		if (dup2(fd, reds[i]->redirect_fd) < 0)
		{
			exec_error("dup2 failed", reds[i]->file);
			close(fd);
			return (-1);
		}
		if (fd != reds[i]->redirect_fd)
			close(fd);
		i--;
	}
	return (0);
}
