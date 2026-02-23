/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:58:53 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/11 14:24:46 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

char	*get_input_line(t_data *data)
{
	char	*prompt;
	char	*line;

	prompt = build_prompt(data);
	if (prompt)
		line = readline(prompt);
	else
		line = readline("minishell$ ");
	free(prompt);
	return (line);
}
