#!/usr/bin/env sh
set -eu

my_path=$(git rev-parse --show-toplevel)

for venv in venv .venv .; do
  if [ -f "${my_path}/${venv}/Scripts/activate" ]; then
    . "${my_path}/${venv}/Scripts/activate"
    break
  fi
done

exec "$@"
