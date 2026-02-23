/*
 ▄▄▄██▀▀▀    ▄▄▄          ██▒   █▓    ██▓   ▓█████     ██▀███  
   ▒██      ▒████▄       ▓██░   █▒   ▓██▒   ▓█   ▀    ▓██ ▒ ██▒
   ░██      ▒██  ▀█▄      ▓██  █▒░   ▒██▒   ▒███      ▓██ ░▄█ ▒
▓██▄██▓     ░██▄▄▄▄██      ▒██ █░░   ░██░   ▒▓█  ▄    ▒██▀▀█▄  
 ▓███▒       ▓█   ▓██▒      ▒▀█░     ░██░   ░▒████▒   ░██▓ ▒██▒
 ▒▓▒▒░       ▒▒   ▓▒█░      ░ ▐░     ░▓     ░░ ▒░ ░   ░ ▒▓ ░▒▓░
 ▒ ░▒░        ▒   ▒▒ ░      ░ ░░      ▒ ░    ░ ░  ░     ░▒ ░ ▒░
 ░ ░ ░        ░   ▒           ░░      ▒ ░      ░        ░░   ░ 
 ░   ░            ░  ░         ░      ░        ░  ░      ░     
                              ░                                

        BUILTINS.C
*/

#include "nanoshell.h"

static struct s_builtin_entry {
	const char		*name;
	int				(*fn)(char **argv, t_data *data);
	int				run_in_parent;
} g_builtins[] = {
	{"echo", builtin_echo, 0},
	{"pwd", builtin_pwd, 0},
	{"cd", builtin_cd, 1},
	{"exit", builtin_exit, 1},
	{"export", builtin_export, 1},
	{NULL, NULL, 0}
};

int is_builtin(const char *name)
{
	int i;

	if (!name)
		return (0);
	i = 0;
	while (g_builtins[i].name)
	{
		if (ft_strncmp(g_builtins[i].name, name,
			(ft_strlen(g_builtins[i].name) + 1)) == 0)
			return (1);
		i++;
	}
	return (0);
}

int builtin_should_run_in_parent(const char *name)
{
	int	i;

	if (!name)
		return (0);
	i = 0;
	while (g_builtins[i].name)
	{
		if (ft_strncmp(g_builtins[i].name, name,
			 ft_strlen(g_builtins[i].name) + 1) == 0)
			 return (g_builtins[i].run_in_parent);
		i++;
	}
	return (0);
}

int call_builtin(char **argv, t_data *data)
{
	int i;

	if (!argv || !argv[0])
		return (1);
	i = 0;
	while (g_builtins[i].name)
	{
		if (ft_strncmp(g_builtins[i].name, argv[0],
			(ft_strlen(g_builtins[i].name) + 1)) == 0)
			return (g_builtins[i].fn(argv, data));
		i++;
	}
	return (127);
}

void builtin_error(const char *cmd, const char *arg, const char *message)
{
    write(2, "minishell: ", 11);
    write(2, cmd, strlen(cmd));
    write(2, ": `", 3);
    write(2, arg, strlen(arg));
    write(2, "': ", 3);
    write(2, message, strlen(message));
    write(2, "\n", 1);
}