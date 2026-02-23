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
find_path_helper.c
*/
#include "nanoshell.h"

int	calc_need_and_slash(const char *dir, const char *cmd,
				size_t *need, size_t *add_slash)
{
	size_t dir_len;
	size_t cmd_len;

	if (!dir || !cmd || !need || !add_slash)
		return (0);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (dir_len == 0)
		*add_slash = 1;
	else if (dir[dir_len - 1] == '/')
		*add_slash = 0;
	else
		*add_slash = 1;
	if (cmd_len > SIZE_MAX - dir_len - *add_slash - 1)
		return (0);
	*need = dir_len + *add_slash + cmd_len + 1;
	return (1);
}

