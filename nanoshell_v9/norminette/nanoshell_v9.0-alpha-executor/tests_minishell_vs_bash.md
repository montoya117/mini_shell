# Batería de pruebas Minishell vs Bash

Recomendado:
- `export LANG=C`
- Compila tu minishell en `./minishell`

En cada prueba observa:
- Mensaje en stderr (no es obligatorio que coincida exactamente con Bash en 42, pero ayuda).
- Código de salida (`$?`). Bash suele devolver:
  - 0 éxito
  - 1 fallo de ejecución (comando no encontrado, permisos, etc. en algunos contextos puede variar)
  - 2 error de sintaxis

## 0) Básicos y entorno

- `echo hola` → stdout: "hola", `$?=0`
- `pwd` → ruta actual, `$?=0`
- `env` → lista de variables, `$?=0`
- `env FOO=bar` (Bash ejecuta `env` con variable temporal; en minishell la parte obligatoria dice “sin opciones o argumentos”) 
  - Si decides rechazar argumentos: stderr: “env: no arguments supported”, `$?=1` (tu implementación actual hace esto)

- PATH y ejecutables:
  - `ls` → debe encontrar vía PATH, `$?=0`
  - `NO_SUCH_CMD` → Bash: “command not found”, `$?=127` (comprueba que tu minishell devuelve 127)

## 1) Sintaxis (parte obligatoria prohíbe ; y \ como especiales) y comillas

- Operadores al principio:
  - `| ls` → Bash: syntax error, `$?=2`
  - `&& ls` → `$?=2`
  - `|| ls` → `$?=2`

- Operadores al final:
  - `ls |` → `$?=2`
  - `ls &&` → `$?=2`
  - `ls ||` → `$?=2`

- Paréntesis (si implementas bonus):
  - `( ls` → `$?=2`
  - `)` → `$?=2`
  - `ls | | wc` → `$?=2`

- Comillas sin cerrar:
  - `echo "hola` → `$?=2`
  - `echo 'hola` → `$?=2`

- Barra invertida y punto y coma (según subject: no especiales):
  - `echo \a` 
    - Bash fuera de comillas trata `\` como escape; el subject dice que minishell NO debe interpretar `\` como especial.
    - Resultado esperado en tu minishell conforme al subject: imprimir “\a”.
  - `echo hola; echo adios`
    - Bash usa `;` como separador; minishell (subject) no debe tratar `;` como especial: debería interpretar todo como un único argumento o fallar como palabra normal (no debe ejecutar dos comandos).

- Comillas:
  - `echo '$HOME'` → imprime literal `$HOME`
  - `echo "$HOME"` → expande HOME
  - `echo "$USER@$HOSTNAME"` → expande ambas

## 2) Redirecciones (<, >, >>, << heredoc)

- Nombre ausente:
  - `echo hola >` → `$?=2`
  - `cat <` → `$?=2`
  - `echo hola >>` → `$?=2`

- Error de apertura:
  - `cat < NO_SUCH_FILE` → `$?=1`
  - `echo hola > /root/out.txt` (sin permisos) → `$?=1`
  - `echo hola >> /root/out.txt` → `$?=1`

- Combinadas:
  - `echo hola > out.txt` → crea/actualiza; `$?=0`, contenido "hola\n"
  - `echo hola >> out.txt` → añade; `$?=0`
  - `cat < out.txt | wc -l` → `$?=0`

- Heredoc (<<):
  - Interactivo:
    - `cat << EOF` (introduce líneas, terminas con `EOF`)
    - Debe crear contenido temporal, no añadir al history, respetar Ctrl-C (cancela heredoc y borra tmp).
  - No interactivo:
    - Nota: tu `create_heredoc_tmp()` usa `readline`, por lo que en modo stdin no-interactivo puede no funcionar. Valídalo manualmente.

## 3) Pipes

- Simples y múltiples:
  - `echo hola | wc -c` → `$?=0`, cuenta bytes (incluye newline si lo hay)
  - `ls | wc -l` → `$?=0`
  - `echo hola | tr a-z A-Z | rev` → `$?=0`

- Comando no encontrado dentro de pipe:
  - `ls | NO_SUCH_CMD` → último código debe ser 127 (estado de la última etapa)

## 4) Expansiones ($, $?):

- Variables:
  - `echo $HOME` → expande
  - `echo $NO_SUCH_VAR` → imprime vacío (o nada)

- `$?` (estado del último comando):
  - Sesión con múltiples líneas:
    1. `false`
    2. `echo $?` → imprime 1
  - Tras pipeline:
    1. `ls | wc -l`
    2. `echo $?` → estado del pipeline completo (estado de la última etapa en Bash)
  - Dentro de comillas: `echo "$?"` → expande

- `$$` (PID del shell)
  - `echo $$` → Bash imprime el PID del shell; tu minishell implementa `$` seguido de `$` como PID del proceso actual. Verifica.

## 5) Señales (interactivo, según subject)

- Ctrl-C:
  - En prompt: imprime una nueva línea y muestra prompt, `$?=130` (valor tras Ctrl-C)
- Ctrl-D:
  - Termina el shell (EOF)
- Ctrl-\:
  - No hace nada (no interrumpe el prompt)

En heredoc:
- Ctrl-C debe cancelar heredoc y borrar el tmp.

## 6) Builtins (parte obligatoria)

- echo:
  - `echo hola` → "hola\n"
  - `echo -n hola` → "hola" sin newline
  - `echo -nnn hola` → sin newline (flags `-n` repetidos válidos)
  - `echo -n -n hola` → sin newline
  - `echo -n -x hola` → `-x` no es `-n`; debe imprimir "-x hola\n"

- cd (solo rutas relativas o absolutas; el subject no obliga `cd -`):
  - `cd /` → `$?=0`, `pwd` imprime `/`
  - `cd NO_SUCH_DIR` → stderr con perror, `$?=1`
  - Opcional: `cd -` (si lo implementas) → cambia a `OLDPWD` y muestra la nueva ruta

- pwd:
  - `pwd` → ruta actual, `$?=0`

- export (sin opciones):
  - `export` → listar variables estilo `declare -x NAME="VALUE"` ordenado
  - `export FOO=bar` → añade/actualiza
  - `export 1ABC=val` → error “not a valid identifier”, `$?=1`

- unset (sin opciones):
  - `unset FOO` → elimina si existe, `$?=0`
  - `unset 1ABC` → error “not a valid identifier”, `$?=1`

- env (sin opciones o argumentos):
  - `env` → lista
  - `env FOO=bar` → conforme a tu implementación, rechaza con mensaje y `$?=1`

- exit (sin opciones):
  - `exit` → termina con último `$?`
  - `exit 42` → termina con código 42
  - `exit 9999999999999` → Bash recorta a 8-bit; tu minishell hace cast `(unsigned char)`.
  - `exit hola` → “numeric argument required”, termina con 255
  - `exit 1 2` → “too many arguments”, no termina, `$?=1`
    - Nota: revisa typo actual: “too man y arguments”

## 7) Bonus (solo si lo obligatorio está PERFECTO)

- AND/OR y paréntesis:
  - `false && echo ok` → no imprime “ok”, `$?=1`
  - `true && echo ok` → imprime “ok”, `$?=0`
  - `false || echo fallback` → imprime “fallback”, `$?=0`
  - `(echo a && echo b) || echo c` → imprime a y b, no c

- Wildcards:
  - `echo *` → Bash expande al contenido del directorio actual; si no implementado, verás “*” literal.
  - `echo *.c` → nombres que coincidan

## 8) No interactivo (stdin)

Crea un archivo con:
```
echo hola
false
echo $?
```
Ejecuta `bash < script` y `./minishell < script`. Debe imprimir:
```
hola
1
```
y terminar con `$?=0` en Bash (exit de la última línea exitosa). En minishell, tu bucle stdin procesa línea a línea: valida que `$?` se actualiza entre líneas.