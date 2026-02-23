*This project was created as part of the 42 curriculum by montoya117.*

# minishell (eval_ready/minishell_f4)

## Description
This folder contains an evaluation-ready version of **minishell**, a 42 project that consists of building a small Unix shell inspired by Bash.

The goal of the project is to understand how a shell:
- reads a command line,
- tokenizes it (lexer),
- parses it (parser),
- executes commands (including pipes and redirections),
- manages processes, exit statuses, and signals.

## Instructions

### Requirements
- Unix-like OS (Linux/macOS)
- `make`
- a C compiler (`cc` / `gcc` / `clang`)
- (optional, depending on your project) `readline` development library

### Build
From the repository root:
```bash
cd eval_ready/minishell_f4
make
```

### Run
```bash
./minishell
```

### Clean
```bash
make clean
make fclean
make re
```

### Quick usage examples
- Basic commands: `ls -la`, `echo hello`, `pwd`
- Pipes: `ls | wc -l`
- Redirections: `cat < infile`, `echo hi > out`, `cmd >> out`
- Heredoc (if implemented): `cat << EOF`

## Resources

### Classic references
- Bash Reference Manual: https://www.gnu.org/software/bash/manual/
- POSIX Shell Command Language: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- `execve(2)`: https://man7.org/linux/man-pages/man2/execve.2.html
- `fork(2)`: https://man7.org/linux/man-pages/man2/fork.2.html
- `pipe(2)`: https://man7.org/linux/man-pages/man2/pipe.2.html
- `dup2(2)`: https://man7.org/linux/man-pages/man2/dup2.2.html
- Signals overview `signal(7)`: https://man7.org/linux/man-pages/man7/signal.7.html

### AI usage (transparency)
- **Where**: documentation only (this `README.md`).
- **For what**: structuring the sections and improving wording/clarity.
- **What it did not do**: it was not used to implement or modify the minishell source code.
