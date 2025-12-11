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
    int     status;
    int     ret;
    pid_t   w;

    while (1)
    {
        w = waitpid(pid, &status, 0);
        if (w == -1)
        {
            if (errno == EINTR)
                continue ;
            perror("waitpid");
            return (127);
        }
        break ;
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
    char    **env_for_cmd; // env clone for tmperary assigment 
    char    *path;
    int     status;
    int     exit_code;

    exit_code = 127;
    if (!node || !node->argv || !node->argv[0])
        exit(exit_code);
    // set env
    env_for_cmd = data->envp;
    // if Temporary VAR=VALUE cmd
    if (node->assignments && node->assignments[0])
    {
        env_for_cmd = dup_env(data->envp);
        apply_assignments_array(&env_for_cmd, node->assignments);
    }
    // Builtin that runs in child (echo, pwd, env, or any non-parent builtin)
    if (is_builtin(node->argv[0])
        && !builtin_should_run_in_parent(node->argv[0]))
    {
        reset_sig_in_child();
        // adapt if builtin needs env_for_cmd TODO??
        status = call_builtin(node->argv, data);
        if (env_for_cmd != data->envp)
            free_env(env_for_cmd);
        exit(status);
    }
    // External command
    path = find_path((char *)node->argv[0], env_for_cmd);
    if (!path)
    {
        exec_error("command not found", node->argv[0]);
        if (env_for_cmd != data->envp)
            free_env(env_for_cmd);
        exit(exit_code);
    }
    reset_sig_in_child();
    execve(path, node->argv, env_for_cmd);
    // si llega aquí, execve ha fallado
    if (errno == EACCES)
    {
        exec_error("Permission denied", node->argv[0]); // o mensaje más fino
        exit_code = 126;
    }
    else if (errno == ENOEXEC)
    {
        exec_error("Exec format error", node->argv[0]);
        exit_code = 126;
    }
    else
    {
        perror("execve");
        exit_code = 127;
    }
    free(path);
    if (env_for_cmd != data->envp)
        free_env(env_for_cmd);
    exit(exit_code);
}

int exec_command(t_ast *node, t_data *data)
{
    pid_t   pid;
    int     ret;

    if (!node || !data)
        return (127);
    // Parent-only builtin in a simple command
    if (node->argv && node->argv[0]
        && is_builtin(node->argv[0])
        && builtin_should_run_in_parent(node->argv[0]))
    {
        //??!! If you support redirections on single builtins, handle them here
        ret = call_builtin(node->argv, data);
        return (ret);

    }
    // Everything else goes to a child
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
