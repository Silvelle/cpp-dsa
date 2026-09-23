#!/usr/bin/env bash
set -euo pipefail

P="${1:-}"
T="${2:-}"

if [[ -z "$P" || -z "$T" ]]; then
    echo "Usage: $0 <practical> <task>"
    echo "Example: $0 01 03"
    exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

PRACTICAL="practicals/$P"
TASK="$PRACTICAL/task$T"
DOCS="docs/p$P"
REQUIREMENTS="$DOCS/requirements/task$T.md"
SECTION="$DOCS/sections/task$T.typ"

if [[ ! -d "$PRACTICAL" ]]; then
    echo "Practical $P does not exist."
    exit 1
fi

if [[ -d "$TASK" ]]; then
    echo "Task $T already exists in practical $P."
    exit 1
fi

mkdir -p "$TASK/src"
mkdir -p "$TASK/include"
mkdir -p "$TASK/tests"

printf '%s\n' \
    '#include <iostream>' \
    '' \
    'int main() {' \
    '    std::cout << "Task not implemented yet.\n";' \
    '    return 0;' \
    '}' > "$TASK/src/main.cpp"

touch "$REQUIREMENTS"
touch "$SECTION"

echo "Created practical $P, task $T"
