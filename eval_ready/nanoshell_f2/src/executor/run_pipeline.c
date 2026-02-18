/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:59:11 by jadelgad          #+#    #+#             */
/*   Updated: 2025/12/03 13:19:29 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

pid_t	spawn_child(t_ast *cmd, int in_fd, int out_fd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		reset_sig_in_child();
		setup_child_io(data, in_fd, out_fd);
		run_child_and_exit(cmd, data);
	}
	return (pid);
}

int	run_single(t_ast *cmd, t_data *data)
{
	if ((!cmd->argv || !cmd->argv[0]) && cmd->assignments)
	{
		apply_assignments_array(&data->envp, cmd->assignments);
		return (0);
	}
	return (exec_command(cmd, data));
}
