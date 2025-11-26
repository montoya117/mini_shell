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
*/

#include "nanoshell.h"

static int  exec_or(t_ast *node, t_data *data)
{
    int status;

    if (!node || node->type != AST_OR)
        return (1);
    status = exec_ast(node->left, data);
    if ( status != 0)
        return (exec_ast(node->right, data));
    return (status);    
}

static int  exec_and(t_ast *node, t_data *data)
{
    int status;

        if (!node || node->type != AST_AND)
            return (1);
        status = exec_ast(node->left, data);
        if  (status == 0)
            return (exec_ast(node->right, data));
        return (status);
}

int	exec_ast(t_ast *node, t_data *data)
{
	if (!node)
		return (0);
	else if (node->type == AST_COMMAND)
		return (exec_command(node, data));
	else if (node->type == AST_AND)
		return (exec_and(node, data));
	else if (node->type == AST_OR)
		return (exec_or(node, data));
	//else if (node->type == AST_PIPE)
		//return (exec_pipeline(node, data));
	//else if (node->type == AST_REDIRECT)
		//return (apply_redirects_and_exec(node, data));
	//else if (node->type == AST_SUBSHELL)
		//return (exec_subshell(node, data));
	return (1);
}
