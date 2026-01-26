#!/usr/bin/env bash
# Minishell HARDCORE Edge Cases Test Suite
# Complementa run_tests.sh con casos extremos y edge cases avanzados
# Usa: ./run_hardcore_tests.sh

set -u

MINISHELL="${MINISHELL:-./minishell}"
MAX_LINES="${MAX_LINES:-200}"
DIFF_TOOL="${DIFF_TOOL:-diff}"

if [[ ! -x "$MINISHELL" ]]; then
  echo "Error: no encuentro ejecutable minishell en '$MINISHELL'."
  echo "Compila primero y/o export MINISHELL=/ruta/a/minishell"
  exit 1
fi

export LANG=C

# Colores
yellow() { printf "\033[1;33m%s\033[0m\n" "$*"; }
green()  { printf "\033[1;32m%s\033[0m\n" "$*"; }
red()    { printf "\033[1;31m%s\033[0m\n" "$*"; }
cyan()   { printf "\033[1;36m%s\033[0m\n" "$*"; }
grey()   { printf "\033[0;37m%s\033[0m\n" "$*"; }
sep()    { printf "\n%s\n\n" "----------------------------------------"; }

TEST_NO=0
next_test() { TEST_NO=$((TEST_NO + 1)); }

print_file_prefixed() {
  local prefix="$1" file="$2"
  if [[ ! -f "$file" || ! -s "$file" ]]; then
    echo "${prefix} (empty)"
    return
  fi
  sed -n "1,${MAX_LINES}p" "$file" | sed "s/^/${prefix} /"
}

show_diff() {
  local file1="$1" file2="$2" label="$3"
  if ! cmp -s "$file1" "$file2"; then
    yellow "Diff en ${label}:"
    $DIFF_TOOL -u "$file1" "$file2" | sed -n "1,${MAX_LINES}p"
  fi
}

run_case() {
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

# ═══════════════════════════════════════════════════════════════════
cyan "╔═══════════════════════════════════════════════════════════════╗"
cyan "║         MINISHELL HARDCORE EDGE CASES TEST SUITE              ║"
cyan "╚═══════════════════════════════════════════════════════════════╝"
# ═══════════════════════════════════════════════════════════════════

# ═══════════════════════════════════════════════════════════════════
cyan "== 1) REDIRECTIONS - Edge Cases Avanzados =="
# ═══════════════════════════════════════════════════════════════════

run_feed "múltiples redirects mismo comando" $'echo test > out1 > out2\ncat out2\ncat out1' \
  "Solo out2 debe tener contenido 'test'"

run_feed "redirect + pipe con fallo" $'echo content > testfile\ncat < testfile | grep content'

run_case "redirect antes del comando" "< /etc/passwd head -3"

run_feed "múltiples heredocs" $'cat << EOF1 << EOF2\nfirst\nEOF1\nsecond\nEOF2' \
  "Solo el último heredoc debe usarse"

run_feed "heredoc con expansión" $'VAR=expanded\ncat << EOF\n$VAR\nEOF'

run_feed "heredoc sin expansión (quoted)" $'VAR=notexpanded\ncat << "EOF"\n$VAR\nEOF'

run_case "redirect stderr a archivo" "ls /nonexistent 2> err.txt"

# ═══════════════════════════════════════════════════════════════════
cyan "== 2) CD - Edge Cases Completos =="
# ═══════════════════════════════════════════════════════════════════

run_feed "cd sin argumentos (va a HOME)" $'cd\npwd'

run_feed "cd con HOME unset" $'unset HOME\ncd\necho $?'

run_feed "cd con múltiples argumentos" $'cd /tmp /home\necho $?'

run_feed "cd a archivo (no directorio)" $'touch testfile\ncd testfile\necho $?\nrm testfile'

run_feed "cd path relativo .." $'cd ..\npwd'

run_feed "cd path relativo ." $'cd .\npwd'

run_feed "cd directorio inexistente" $'cd /nonexistent_xyz_dir\necho $?'

# ═══════════════════════════════════════════════════════════════════
cyan "== 3) EXPORT - Edge Cases Completos =="
# ═══════════════════════════════════════════════════════════════════

run_feed "export sin argumentos (listar)" $'export | head -5'

run_feed "export valor vacío" $'export EMPTY=\necho "[$EMPTY]"'

run_feed "export con espacios en valor" $'export VAR="value with spaces"\necho $VAR'

run_case "export identificador inválido (empieza con dígito)" "export 1ABC=test"

run_case "export identificador inválido (guión)" "export TEST-VAR=test"

run_case "export identificador inválido (símbolo)" "export TEST@VAR=test"

run_feed "export múltiples variables" $'export A=1 B=2 C=3\necho $A $B $C'

run_feed "export sobrescribir variable" $'export FOO=old\nexport FOO=new\necho $FOO'

# ═══════════════════════════════════════════════════════════════════
cyan "== 4) UNSET - Edge Cases Completos =="
# ═══════════════════════════════════════════════════════════════════

run_feed "unset sin argumentos" $'unset\necho $?'

run_feed "unset variable existente" $'export FOO=bar\nunset FOO\necho $FOO'

run_feed "unset variable inexistente" $'unset NONEXISTENT\necho $?'

run_feed "unset múltiples variables" $'export A=1 B=2 C=3\nunset A B C\necho $A $B $C'

run_case "unset identificador inválido" "unset 1ABC"

run_feed "unset con identificadores mezclados" $'export FOO=1\nunset FOO 1ABC BAR\necho $?'

# ═══════════════════════════════════════════════════════════════════
cyan "== 5) EXIT - Edge Cases Completos =="
# ═══════════════════════════════════════════════════════════════════

run_case "exit sin argumentos (usa último status)" "false; exit"

run_case "exit con código numérico" "exit 42"

run_case "exit con argumento no numérico" "exit hello"

run_case "exit con múltiples argumentos" "exit 1 2"

run_case "exit con overflow" "exit 999999999999999"

run_case "exit con código negativo" "exit -42"

run_case "exit con código 256 (wrap)" "exit 256"

# ═══════════════════════════════════════════════════════════════════
cyan "== 6) PIPES - Edge Cases Avanzados =="
# ═══════════════════════════════════════════════════════════════════

run_case "comando fallido seguido de exitoso en pipe" "false | echo still runs"

run_feed "status de pipeline es último comando" $'true | false | true\necho $?'

run_case "builtin en medio de pipeline" "echo test | export FOO=bar | cat"

run_case "cat sin entrada en pipe" "echo test | cat | cat | cat"

run_feed "pipeline con 10 etapas" $'echo start | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat'

run_case "pipeline con comando no encontrado" "echo test | NOEXIST | cat"

# ═══════════════════════════════════════════════════════════════════
cyan "== 7) QUOTES Y EXPANSIONS - Edge Cases Avanzados =="
# ═══════════════════════════════════════════════════════════════════

run_case "dollar al final de palabra" "echo test\$"

run_case "expansión vacía" "echo \"\$NONEXISTENT\""

run_case "expansión en single quotes (no debe expandir)" "echo '\$HOME'"

run_case "expansión en double quotes (debe expandir)" "echo \"\$HOME\""

run_case "quotes consecutivas vacías" "echo \"\"''\"\"test"

run_case "quotes mezcladas complejas" "echo 'single'\"double\"'single'\"\""

run_case "espacios en quotes" "echo 'hello    world'"

run_case "caracteres especiales en single quotes" "echo '| > < & * ?'"

run_case "dollar seguido de caracteres especiales" "echo \$@ \$# \$*"

run_feed "expansión $? múltiple" $'false\necho $? $? $?'

# ═══════════════════════════════════════════════════════════════════
cyan "== 8) ECHO - Edge Cases Específicos =="
# ═══════════════════════════════════════════════════════════════════

run_case "echo -n básico" "echo -n hello"

run_case "echo -n múltiple" "echo -n -n -n test"

run_case "echo -nnn (múltiples n)" "echo -nnn hello"

run_case "echo -n seguido de flag inválido" "echo -n -x hello"

