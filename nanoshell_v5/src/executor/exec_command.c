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

        EXEC_COMMAND.C
*/

#include "nanoshell.h"

int  wait_for_child(pid_t pid)
{
    int status;
    int ret;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return (127);
    }
    if (WIFEXITED(status))
        ret = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        ret = 128 + WTERMSIG(status);
    else
        ret = status;
    return (ret);
}
// +128 para distinguir fallo por señal \ fallo por exit code.

void reset_sig_in_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void exec_child(t_ast *node, t_data *data)
{
    char    *path;

    if (!node || !node->argv || !node->argv[0])
        exit(127);
    path = find_path((char *)node->argv[0], data->envp);
    if (!path)
    {
        exec_error("command not found", node->argv[0]);
        exit(127);
    }
    reset_sig_in_child();
    execve(path, node->argv, data->envp);
    perror("execve");
    free(path);    
    exit(127);
}

int exec_command(t_ast *node, t_data *data)
{
    pid_t   pid;
    int     ret;

    if (!node || !data)
        return (127);
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (127);
    }
    if (pid == 0)
        exec_child(node, data);
    ret = wait_for_child(pid);
    return (ret);
}
