*Este proyecto ha sido creado como parte del currículo de 42 por montoya117.*

# mini_shell

## Descripción
**mini_shell** es una implementación educativa de una *mini shell* estilo Bash (proyecto de 42). El objetivo es comprender cómo un intérprete de comandos:
- lee una línea de entrada,
- la tokeniza (lexer),
- la analiza (parser),
- construye/ejecuta comandos (incluyendo *pipes* y redirecciones),
- y gestiona procesos, señales y variables de entorno.

Este repositorio contiene distintas iteraciones/experimentos (carpetas `nanoshell_v*`) y una versión lista para evaluación en `eval_ready/`.

## Instrucciones
### Requisitos
- Sistema tipo Unix (Linux/macOS)
- `make`
- Compilador C (`cc`/`gcc`/`clang`)
- Librerías habituales de 42 (según tu entorno: `readline` si el proyecto la usa)

### Compilar
La versión preparada para evaluación está en:
- `eval_ready/minishell_f4/`

Ejemplo (ajusta la ruta si tu Makefile está en otro directorio):
```bash
cd eval_ready/minishell_f4
make
```

### Ejecutar
```bash
./minishell
```

### Limpieza
```bash
make clean
make fclean
make re
```

### Notas rápidas de uso
- Ejecuta comandos como en Bash: `ls -la`, `echo hola`, etc.
- *Pipes*: `ls | wc -l`
- Redirecciones: `cat < infile`, `echo hi > out`, `cmd >> out`
- *Heredoc* (si está implementado): `cat << EOF`

## Recursos
### Referencias clásicas
- Manual de Bash: https://www.gnu.org/software/bash/manual/
- POSIX Shell & Utilities: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- `execve(2)`: https://man7.org/linux/man-pages/man2/execve.2.html
- `fork(2)`: https://man7.org/linux/man-pages/man2/fork.2.html
- `pipe(2)`: https://man7.org/linux/man-pages/man2/pipe.2.html
- `dup2(2)`: https://man7.org/linux/man-pages/man2/dup2.2.html
- Señales (`signal(7)`): https://man7.org/linux/man-pages/man7.html

### Uso de IA (transparencia)
- **Dónde**: apoyo para redactar/estructurar este `README.md`.
- **Para qué tareas**: generación de plantilla, organización de secciones y redacción.
- **Qué no hizo**: no se utilizó para implementar ni modificar el código fuente de la shell (solo documentación).