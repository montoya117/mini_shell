#!/usr/bin/env bash
# Minishell vs Bash test runner (numerado y con salidas claramente etiquetadas)
# Objetivo: comparar comportamiento y resaltar QUÉ debes corregir según el subject.
# - Numera cada test para depuración posterior.
# - Muestra stdout/stderr con prefijos por línea: "BASH:" y "MINISHELL:".
# - Muestra estados con etiquetas claras.
#
# Usa MINISHELL=/ruta/a/minishell para apuntar al binario (por defecto ./minishell).

set -u

MINISHELL="${MINISHELL:-./minishell}"
MAX_LINES="${MAX_LINES:-200}"     # Limita líneas impresas por bloque de salida
DIFF_TOOL="${DIFF_TOOL:-diff}"    # Cambia a 'colordiff' si lo tienes

if [[ ! -x "$MINISHELL" ]]; then
  echo "Error: no encuentro ejecutable minishell en '$MINISHELL'."
  echo "Compila primero y/o export MINISHELL=/ruta/a/minishell"
  exit 1
fi

# Estabiliza mensajes (bash en inglés)
export LANG=C

# Colores
yellow() { printf "\033[1;33m%s\033[0m\n" "$*"; }
green()  { printf "\033[1;32m%s\033[0m\n" "$*"; }
red()    { printf "\033[1;31m%s\033[0m\n" "$*"; }
cyan()   { printf "\033[1;36m%s\033[0m\n" "$*"; }
grey()   { printf "\033[0;37m%s\033[0m\n" "$*"; }
sep()    { printf "\n%s\n\n" "----------------------------------------"; }

# Numeración de tests
TEST_NO=0
next_test() { TEST_NO=$((TEST_NO + 1)); }

# Utilidades de impresión
print_file_prefixed() {
  # Imprime contenido de archivo con un prefijo por línea, o "(empty)" si no hay contenido
  local prefix="$1" file="$2"
  if [[ ! -f "$file" || ! -s "$file" ]]; then
    echo "${prefix} (empty)"
    return
  fi
  sed -n "1,${MAX_LINES}p" "$file" | sed "s/^/${prefix} /"
}

show_diff() {
  # Muestra diferencias si las hay
  local file1="$1" file2="$2" label="$3"
  if ! cmp -s "$file1" "$file2"; then
    yellow "Diff en ${label}:"
    $DIFF_TOOL -u "$file1" "$file2" | sed -n "1,${MAX_LINES}p"
  fi
}

# Utilidades de validación específicas para señalar correcciones
contains() { grep -qE "$1" "$2"; }          # ¿archivo contiene regex?
line_count() { wc -l < "$1" | tr -d ' '; }  # contar líneas
print_required_fix() { red "REQUIRED FIX: $*"; }
print_subject_diff() { yellow "SUBJECT DIFFERENCE (aceptable): $*"; }
print_info() { grey "INFO: $*"; }

run_case() {
  # Ejecuta un comando con Bash y con Minishell (-c) y compara resultados.
  local name="${1:-}" cmd="${2:-}" notes="${3:-}"
  if [[ -z "$name" || -z "$cmd" ]]; then
    red "run_case necesita 2 argumentos: nombre y comando"
    return 1
  fi

  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN

  next_test
  yellow "TEST #${TEST_NO}: ${name}"
  printf "CMD: %q\n" "$cmd"

  local b_out="$dir/bash.out" b_err="$dir/bash.err"
  local m_out="$dir/mini.out" m_err="$dir/mini.err"

  bash -c "$cmd" >"$b_out" 2>"$b_err"
  local b_status=$?

  "$MINISHELL" -c "$cmd" >"$m_out" 2>"$m_err"
  local m_status=$?

  echo "BASH STATUS: ${b_status}"
  echo "MINISHELL STATUS: ${m_status}"

  echo "BASH STDOUT:"
  print_file_prefixed "BASH:" "$b_out"
  echo "BASH STDERR:"
  print_file_prefixed "BASH:" "$b_err"

  echo "MINISHELL STDOUT:"
  print_file_prefixed "MINISHELL:" "$m_out"
  echo "MINISHELL STDERR:"
  print_file_prefixed "MINISHELL:" "$m_err"

  show_diff "$b_out" "$m_out" "stdout"
  show_diff "$b_err" "$m_err" "stderr"

  if [[ "$b_status" -ne "$m_status" ]]; then
    red "STATUS MISMATCH (bash=$b_status, mini=$m_status)"
  else
    green "STATUS OK"
  fi

  if [[ -n "$notes" ]]; then
    grey "Nota: $notes"
  fi

  sep
}

run_feed() {
  # Alimenta una sesión por stdin (múltiples líneas) y compara resultados.
  local name="${1:-}" script="${2:-}" notes="${3:-}"
  if [[ -z "$name" || -z "$script" ]]; then
    red "run_feed necesita 2 argumentos: nombre y script"
    return 1
  fi

  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN

  next_test
  yellow "TEST #${TEST_NO} (FEED): ${name}"
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

  echo "BASH STATUS: ${b_status}"
  echo "MINISHELL STATUS: ${m_status}"

  echo "BASH STDOUT:"
  print_file_prefixed "BASH:" "$b_out"
  echo "BASH STDERR:"
  print_file_prefixed "BASH:" "$b_err"

  echo "MINISHELL STDOUT:"
  print_file_prefixed "MINISHELL:" "$m_out"
  echo "MINISHELL STDERR:"
  print_file_prefixed "MINISHELL:" "$m_err"

  show_diff "$b_out" "$m_out" "stdout"
  show_diff "$b_err" "$m_err" "stderr"

  if [[ "$b_status" -ne "$m_status" ]]; then
    red "STATUS MISMATCH (bash=$b_status, mini=$m_status)"
  else
    green "STATUS OK"
  fi

  if [[ -n "$notes" ]]; then
    grey "Nota: $notes"
  fi

  sep
}

# Validadores auxiliares
check_no_double_error_lines() {
  local cmd="$1"
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  "$MINISHELL" -c "$cmd" >/dev/null 2>"$dir/mini.err"
  local m_lines; m_lines="$(line_count "$dir/mini.err")"
  if [[ "$m_lines" -gt 1 ]]; then
    print_required_fix "Evita imprimir el mismo error dos veces (solo una línea en stderr) para: $cmd"
  fi
}

check_feed_prints_number() {
  local script="$1"
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  printf "%s\n" "$script" > "$dir/s.txt"
  "$MINISHELL" < "$dir/s.txt" >"$dir/mini.out" 2>/dev/null
  if ! contains '^[0-9]+$' "$dir/mini.out"; then
    print_required_fix "La expansión de \$? en modo no interactivo no imprime el estado (script: $(printf %q "$script"))."
  fi
}

check_export_error_text() {
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  "$MINISHELL" -c 'export 1ABC=val' >/dev/null 2>"$dir/mini.err"
  if ! contains 'valid identifier' "$dir/mini.err"; then
    print_required_fix "Corrige el texto del error en 'export': usa 'not a valid identifier'."
  fi
}

check_unset_invalid_behaviour() {
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  "$MINISHELL" -c 'unset 1ABC' >/dev/null 2>"$dir/mini.err"
  local st=$?
  if [[ "$st" -ne 0 ]]; then
    print_required_fix "unset con identificador inválido debe devolver status 0 (como bash)."
  fi
  if ! contains 'valid identifier' "$dir/mini.err"; then
    print_required_fix "Corrige el texto del error en 'unset': usa 'not a valid identifier'."
  fi
}

# Prueba de permisos de redirección sin depender de /root
test_redir_permissions_local() {
  local dir; dir="$(mktemp -d)"
  trap 'rm -rf "$dir"' RETURN
  local target="$dir/no_write"
  : > "$target"
  chmod 000 "$target"
  run_case "redir sin permisos (local 000)" "echo hola > '$target'"
}

# ===========================
# Batería de pruebas
# ===========================

cyan "== 1) Sintaxis y operadores =="
run_case "pipe al inicio"                 "| ls"
run_case "AND al inicio"                  "&& ls"
run_case "OR al inicio"                   "|| ls"
run_case "pipe al final"                  "ls |"
run_case "AND al final"                   "ls &&"
run_case "OR al final"                    "ls ||"
run_case "doble pipe"                     "ls | | wc"

# Paréntesis (bonus). Bash devuelve 2 en sintaxis incorrecta.
run_case "paren sin cerrar"               "( ls"
check_no_double_error_lines "( ls"
run_case "paren suelta"                   ")"

cyan "== 2) Redirecciones =="
run_case "redir sin nombre (>)"           "echo hola >"
run_case "redir sin nombre (<)"           "cat <"
run_case "append sin nombre (>>)"         "echo hola >>"
run_case "in inexistente"                 "cat < NO_SUCH_FILE"
run_case "out sin permisos (/root)"       "echo hola > /root/out.txt"
test_redir_permissions_local

run_case "redir out simple"               "echo hola > out.txt"
run_case "redir append"                   "echo hola >> out.txt"
run_case "pipe + redir in"                "cat < out.txt | wc -l"

cyan "== 3) PATH y 'command not found' =="
run_case "comando no encontrado"          "NO_SUCH_CMD"

cyan "== 4) Comillas y caracteres especiales =="
run_case "comillas dobles sin cerrar"     'echo "hola'
check_no_double_error_lines 'echo "hola'
run_case "comillas simples sin cerrar"    "echo 'hola"
check_no_double_error_lines "echo 'hola"

run_case "single quote literal"           "echo '\$HOME'"
run_case "double quote expand"            "echo \"$HOME\""

# Subject: minishell NO debe tratar '\' como especial fuera de comillas.
run_case "backslash literal (subject)"    "echo \\a" \
  "Si difiere, es SUBJECT DIFFERENCE (aceptable según enunciado)."
print_subject_diff "Backslash fuera de comillas no es especial en minishell; en bash sí."

# Subject: minishell NO debe tratar ';' como separador de comandos.
run_case "semicolon referencia bash"      "echo hola; echo adios" \
  "Minishell no debe implementar ';'. Esta diferencia es aceptable."
print_subject_diff "El tester con ';' es referencial; usa tests sin ';' para validar tu minishell."

cyan "== 5) Pipes =="
run_case "pipe simple"                    "echo hola | wc -c"
run_case "pipe múltiple"                  "echo hola | tr a-z A-Z | rev"
run_case "pipe con fallo al final"        "ls | NO_SUCH_CMD"

cyan "== 6) Expansiones ($, $?, $$) =="
run_case "HOME expansion"                 "echo \$HOME"
run_case "var missing"                    "echo \$NO_SUCH_VAR"

# Estado de salida entre líneas (stdin)
run_feed "estado tras false"              $'false\necho $?' \
  "Minishell debe expandir \$? también en modo no interactivo."
check_feed_prints_number $'false\necho $?'

run_feed "estado tras pipeline"           $'ls | wc -l\necho $?' \
  "Minishell debe guardar el estado de la pipeline para \$?."
check_feed_prints_number $'ls | wc -l\necho $?'

# $$ (PID). Diferencia esperada
run_case "pid expansion $$"               "echo \$\$" \
  "Diferencia en número esperada; sólo compara status."
print_info "Es normal que $$ difiera (PIDs distintos)."

cyan "== 7) Builtins básicos =="
# echo
run_case "echo simple"                    "echo hola"
run_case "echo -n"                        "echo -n hola"
run_case "echo -nnn"                      "echo -nnn hola"
run_case "echo -n -n"                     "echo -n -n hola"
run_case "echo -n -x hola"                "echo -n -x hola"

# cd sin ';' para pruebas compatibles con subject
run_feed "cd a / (subject)"               $'cd /\npwd' \
  "Usa feed sin ';' para validar cd."
# Referencia bash con ';'
run_case "cd a / (bash referencia)"       "cd /; pwd" \
  "Si tu minishell no soporta ';', ignora este mismatch."
print_subject_diff "Si no soportas ';', usa la versión feed para validar cd."

run_case "cd inexistente"                 "cd NO_SUCH_DIR"
run_feed "cd sin HOME (subject)"          $'unset HOME\ncd\necho $?'

# pwd
run_case "pwd"                            "pwd"

# export / unset / env
run_case "export listar"                  "export"

# export set y leer
run_feed "export set y leer (subject)"    $'export FOO=bar\necho $FOO'
run_case "export set y leer (bash ref)"   "export FOO=bar; echo \$FOO" \
  "Si no soportas ';', usa la versión feed anterior."
print_subject_diff "No estás obligado a soportar ';'."

run_case "export identificador invalido"  "export 1ABC=val"
check_export_error_text

# Unset
run_feed "unset variable (subject)"       $'export FOO=bar\nunset FOO\necho $FOO'
run_case "unset invalido"                 "unset 1ABC"
check_unset_invalid_behaviour

run_case "env sin args"                   "env"
run_case "env con arg temporal"           "env FOO=bar"

# exit
run_case "exit simple"                    "exit"
run_case "exit 42"                        "exit 42"
run_case "exit no numeric"                "exit hola"
run_case "exit many args"                 "exit 1 2"

cyan "== 8) Bonus (AND/OR/paréntesis) si aplican =="
run_case "false && echo ok"               "false && echo ok"
run_case "true && echo ok"                "true && echo ok"
run_case "false || echo fallback"         "false || echo fallback"
run_case "(echo a && echo b) || echo c"   "(echo a && echo b) || echo c"

yellow "Fin de la batería."
