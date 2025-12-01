# mini_shell
 Mini_Shell_project

 This is Javi's & Monty's repo for mini_shell project:

 ---> NANOSHELL_V1 -----> LEXER + PARSER

 ---> NANOSHELL_V1 -----> LEXER + PARSER + EXECUTTOR


 THINGS MONTY TOUCHED.......
 
--> exec_redir.c
 added 
 if (waitpid(pid, &status, 0) == -1)
{
    perror("waitpid");
    if (node->redirect_type == TOKEN_HEREDOC && node->file)
        unlink(node->file);
    data->last_status = 1;
    return (data->last_status);
}

change _exit to exit

status = exec_ast(node->left, data);

--> exec_subshell.c

K ha pasaoo??
En el flujo original, las redirecciones (salida a archivo, entrada desde archivo, etc.) se gestionaban justo antes de ejecutar el comando, garantizando que los archivos se crearan o se abrieran según lo especificado. La introducción de la gestión de múltiples comandos en la nueva lógica de tuberías puede haber cambiado el orden de llamadas o incluso saltarse la lógica de redirecciones en ciertas condiciones, sobre todo si la diferencia entre comando simple y múltiple no se tuvo en cuenta al integrar el código de redirección.

Si ahora los archivos ya no se crean o las redirecciones no se aplican como se espera, habría que comprobar en concreto:

Si la configuración de redirecciones (apply_redirect_and_exec) se está invocando para cada comando dentro de la tubería o solo una vez para toda la tubería.

Que run_multi y las funciones que llama no estén saltándose sin querer la lógica de redirecciones para cada proceso hijo dentro del pipeline.

Que la lógica de abrir, duplicar (dup2) y cerrar descriptores de archivo no se haya movido fuera del fork del proceso hijo correspondiente.

Siguiendo estos puntos y revisando las transiciones entre la ejecución del pipeline y la configuración de redirecciones, es muy probable que se encuentre por qué dejó de funcionar todo después de introducir el nuevo exec_pipeline
 
 
