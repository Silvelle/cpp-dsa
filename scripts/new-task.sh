#!/usr/bin/env bash
set -euo pipefail

RAW_P="${1:-}"
RAW_T="${2:-}"
LAYOUT="${3:-}"

if [[ ! "$RAW_P" =~ ^[0-9]+$ || ! "$RAW_T" =~ ^[0-9]+$ ]]; then
    echo "Usage: $0 <practical-number> <task-number> [--full]"
    echo "Example: $0 1 3"
    exit 1
fi

if [[ -n "$LAYOUT" && "$LAYOUT" != "--full" ]]; then
    echo "Unknown option: $LAYOUT"
    exit 1
fi

printf -v P '%02d' "$((10#$RAW_P))"
printf -v T '%02d' "$((10#$RAW_T))"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TEMPLATES="$ROOT_DIR/scripts/templates"
PRACTICAL="$ROOT_DIR/practicals/$P"
TASK="$PRACTICAL/task$T"
DOCS="$ROOT_DIR/docs/p$P"

if [[ ! -f "$PRACTICAL/CMakeLists.txt" || ! -d "$DOCS" ]]; then
    echo "Practical $P does not exist."
    echo "Create it first: scripts/new-practical.sh $P"
    exit 1
fi

if [[ -e "$TASK" ]]; then
    echo "Task $T already exists in practical $P."
    exit 1
fi

mkdir -p "$TASK/src"
if [[ "$LAYOUT" == "--full" ]]; then
    mkdir -p "$TASK/include" "$TASK/tests"
fi

install -m 0644 "$TEMPLATES/task-CMakeLists.txt" "$TASK/CMakeLists.txt"
install -m 0644 "$TEMPLATES/task-main.cpp" "$TASK/src/main.cpp"

touch "$DOCS/requirements/task$T.md"
touch "$DOCS/sections/task$T.typ"

echo "Created task $T in practical $P:"
echo "  practicals/$P/task$T"
echo "  docs/p$P/requirements/task$T.md"
echo "  docs/p$P/sections/task$T.typ"
echo "Build: make P=$P T=$T"
