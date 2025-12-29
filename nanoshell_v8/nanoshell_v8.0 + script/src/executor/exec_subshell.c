#include "nanoshell.h"

int exec_subshell(t_ast *node, t_data *data)
{
    pid_t   pid;
    int     status;

    if (!node || node->type != AST_SUBSHELL || !node->left)
        return (1);
    
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
    {
        // CHILD: run subshell body in a separate process
        reset_sig_in_child();
        status = exec_ast(node->left, data);
        exit(status);
    }
    // PARENT: wait and return subshell status
    return (wait_for_child(pid));
}