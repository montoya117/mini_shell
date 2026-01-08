#!/usr/bin/env bash
# Minishell vs Bash test runner (ampliado)
# Ejecuta baterías de casos y compara stdout, stderr y códigos de salida.
# Usa MINISHELL=/ruta/a/minishell para apuntar al binario si no está en ./minishell

set -u

MINISHELL="${MINISHELL:-./minishell}"
MAX_LINES="${MAX_LINES:-200}"   # Limita líneas impresas por bloque de salida
DIFF_TOOL="${DIFF_TOOL:-diff}"  # Cambia a 'colordiff' si lo tienes

if [[ ! -x "$MINISHELL" ]]; then
  echo "Error: no encuentro ejecutable minishell en '$MINISHELL'."
  echo "Compila primero y/o export MINISHELL=/ruta/a/minishell"
  exit 1
fi

# Estabiliza mensajes de error en Bash para comparación
export LANG=C

# Colores
yellow() { printf "\033[1;33m%s\033[0m\n" "$*"; }
green()  { printf "\033[1;32m%s\033[0m\n" "$*"; }
red()    { printf "\033[1;31m%s\033[0m\n" "$*"; }
cyan()   { printf "\033[1;36m%s\033[0m\n" "$*"; }
sep()    { printf "\n%s\n\n" "----------------------------------------"; }

print_file() {
  # Imprime un archivo limitado a MAX_LINES líneas
  local file="$1"
  if [[ ! -f "$file" ]]; then
    echo "(no existe)"
    return
  fi
  sed -n "1,${MAX_LINES}p" "$file"
}

show_diff() {
  # Muestra diferencias si las hay
  local file1="$1" file2="$2" label="$3"
  if ! cmp -s "$file1" "$file2"; then
    yellow "Diff en $label:"
    $DIFF_TOOL -u "$file1" "$file2" | sed -n "1,${MAX_LINES}p"
  fi
}

run_case() {
  # Ejecuta un comando con Bash y con Minishell (-c) y compara resultados.
  local name="${1:-}"
  local cmd="${2:-}"
  if [[ -z "$name" || -z "$cmd" ]]; then
    red "run_case necesita 2 argumentos: nombre y comando"
    return 1
  fi

  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN

  yellow "CASE: $name"
  printf "CMD: %q\n" "$cmd"

  local b_out="$dir/bash.out" b_err="$dir/bash.err"
  local m_out="$dir/mini.out" m_err="$dir/mini.err"

  bash -c "$cmd" >"$b_out" 2>"$b_err"
  local b_status=$?

  "$MINISHELL" -c "$cmd" >"$m_out" 2>"$m_err"
  local m_status=$?

  echo "bash    status: $b_status"
  echo "mini    status: $m_status"

  echo "bash    stdout:"
  print_file "$b_out"
  echo "bash    stderr:"
  print_file "$b_err"
  echo "mini    stdout:"
  print_file "$m_out"
  echo "mini    stderr:"
  print_file "$m_err"

  show_diff "$b_out" "$m_out" "stdout"
  show_diff "$b_err" "$m_err" "stderr"

  if [[ "$b_status" -ne "$m_status" ]]; then
    red "STATUS MISMATCH (bash=$b_status, mini=$m_status)"
  else
    green "STATUS OK"
  fi
  sep
}

run_feed() {
  # Alimenta una sesión por stdin (múltiples líneas) y compara resultados.
  local name="${1:-}"
  local script="${2:-}"
  if [[ -z "$name" || -z "$script" ]]; then
    red "run_feed necesita 2 argumentos: nombre y script"
    return 1
  fi

  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN

  yellow "FEED SESSION: $name"
  echo "Script:"
  echo "----------------------------------------"
  echo "$script"
  echo "----------------------------------------"

  local sfile="$dir/script.txt"
  printf "%s\n" "$script" > "$sfile"

  local b_out="$dir/bash.out" b_err="$dir/bash.err"
  local m_out="$dir/mini.out" m_err="$dir/mini.err"

  bash < "$sfile" >"$b_out" 2>"$b_err"
  local b_status=$?

  "$MINISHELL" < "$sfile" >"$m_out" 2>"$m_err"
  local m_status=$?

  echo "bash    status: $b_status"
  echo "mini    status: $m_status"

  echo "bash    stdout:"
  print_file "$b_out"
  echo "bash    stderr:"
  print_file "$b_err"
  echo "mini    stdout:"
  print_file "$m_out"
  echo "mini    stderr:"
  print_file "$m_err"

  show_diff "$b_out" "$m_out" "stdout"
  show_diff "$b_err" "$m_err" "stderr"

  if [[ "$b_status" -ne "$m_status" ]]; then
    red "STATUS MISMATCH (bash=$b_status, mini=$m_status)"
  else
    green "STATUS OK"
  fi
  sep
}

# Prueba de permisos de redirección sin depender de /root
test_redir_permissions_local() {
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  local target="$dir/no_write"

  # Crea archivo sin permisos (000)
  : > "$target"
  chmod 000 "$target"

  # Construye el comando dinámicamente
  run_case "redir sin permisos (local 000)" "echo hola > '$target'"
}

# ===========================
# Batería de pruebas ampliada
# ===========================

cyan "== 1) Sintaxis y operadores =="
run_case "pipe al inicio" "| ls"
run_case "AND al inicio" "&& ls"
run_case "OR al inicio" "|| ls"
run_case "pipe al final" "ls |"
run_case "AND al final"  "ls &&"
run_case "OR al final"   "ls ||"
run_case "doble pipe"    "ls | | wc"

# Paréntesis (bonus). Bash devuelve 2 en sintaxis incorrecta.
run_case "paren sin cerrar" "( ls"
run_case "paren suelta" ")"

cyan "== 2) Redirecciones =="
run_case "redir sin nombre (>)"   "echo hola >"
run_case "redir sin nombre (<)"   "cat <"
run_case "append sin nombre (>>)" "echo hola >>"
run_case "in inexistente"         "cat < NO_SUCH_FILE"
run_case "out sin permisos (/root)" "echo hola > /root/out.txt"
test_redir_permissions_local

run_case "redir out simple"  "echo hola > out.txt"
run_case "redir append"      "echo hola >> out.txt"
run_case "pipe + redir in"   "cat < out.txt | wc -l"

cyan "== 3) PATH y 'command not found' =="
run_case "comando no encontrado" "NO_SUCH_CMD"

cyan "== 4) Comillas y caracteres especiales =="
run_case "comillas dobles sin cerrar" 'echo "hola'
run_case "comillas simples sin cerrar" "echo 'hola"
run_case "single quote literal" "echo '\$HOME'"
run_case "double quote expand"  "echo \"$HOME\""

# Subject: minishell NO debe tratar '\' como especial fuera de comillas.
# Bash sí lo hace, así que salida puede diferir.
run_case "backslash literal (subject)" "echo \\a"

# Subject: minishell NO debe tratar ';' como especial. Bash sí separa comandos.
# Este test probablemente mostrará diferencias en stdout/stderr.
run_case "semicolon referencia bash" "echo hola; echo adios"

cyan "== 5) Pipes =="
run_case "pipe simple"     "echo hola | wc -c"
run_case "pipe múltiple"   "echo hola | tr a-z A-Z | rev"
run_case "pipe con fallo al final" "ls | NO_SUCH_CMD"

cyan "== 6) Expansiones ($, $?, $$) =="
run_case "HOME expansion"  "echo \$HOME"
run_case "var missing"     "echo \$NO_SUCH_VAR"

# Estado de salida entre líneas (stdin)
run_feed "estado tras false"    $'false\necho $?'
run_feed "estado tras pipeline" $'ls | wc -l\necho $?'

# $$ (PID). Bash imprime pid del shell; minishell debe imprimir su propio pid.
run_case "pid expansion $$" "echo \$\$"

cyan "== 7) Builtins básicos =="
# echo
run_case "echo simple"         "echo hola"
run_case "echo -n"             "echo -n hola"
run_case "echo -nnn"           "echo -nnn hola"
run_case "echo -n -n"          "echo -n -n hola"
run_case "echo -n -x hola"     "echo -n -x hola"

# cd (nota: cd - es opcional)
run_case "cd a /"              "cd /; pwd"
run_case "cd inexistente"      "cd NO_SUCH_DIR"
run_feed "cd sin HOME (subject)" $'unset HOME\ncd\necho $?'

# pwd
run_case "pwd" "pwd"

# export / unset / env
run_case "export listar"           "export"
run_case "export set y leer"       "export FOO=bar; echo \$FOO"
run_case "export identificador invalido" "export 1ABC=val"
run_case "unset variable"          "export FOO=bar; unset FOO; echo \$FOO"
run_case "unset invalido"          "unset 1ABC"

run_case "env sin args"            "env"
# Diferencia con tu minishell (si rechaza argumentos):
run_case "env con arg temporal"    "env FOO=bar"

# exit
run_case "exit simple"             "exit"
run_case "exit 42"                 "exit 42"
run_case "exit no numeric"         "exit hola"
run_case "exit many args"          "exit 1 2"

cyan "== 8) Bonus (AND/OR/paréntesis) si aplican =="
run_case "false && echo ok"        "false && echo ok"
run_case "true && echo ok"         "true && echo ok"
run_case "false || echo fallback"  "false || echo fallback"
run_case "(echo a && echo b) || echo c" "(echo a && echo b) || echo c"

yellow "Fin de la batería."
