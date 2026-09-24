#!/usr/bin/env bash
set -euo pipefail

RAW_P="${1:-}"

if [[ ! "$RAW_P" =~ ^[0-9]+$ ]]; then
    echo "Usage: $0 <practical-number>"
    echo "Example: $0 2"
    exit 1
fi

printf -v P '%02d' "$((10#$RAW_P))"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TEMPLATES="$ROOT_DIR/scripts/templates"
PRACTICAL="$ROOT_DIR/practicals/$P"
DOCS="$ROOT_DIR/docs/p$P"

if [[ -e "$PRACTICAL" || -e "$DOCS" ]]; then
    echo "Practical $P already exists."
    exit 1
fi

# Generated build, output, screenshot, data, and result directories are not
# scaffolded. They appear only when a command actually needs them.
mkdir -p "$PRACTICAL/common"
mkdir -p "$DOCS/assets" "$DOCS/requirements" "$DOCS/sections"

install -m 0644 "$TEMPLATES/practical-CMakeLists.txt" \
    "$PRACTICAL/CMakeLists.txt"
install -m 0644 "$TEMPLATES/practical.gitignore" \
    "$PRACTICAL/.gitignore"

touch "$DOCS/main.typ"
touch "$DOCS/config.typ"
touch "$DOCS/sections/introduction.typ"
touch "$DOCS/sections/conclusion.typ"

echo "Created practical $P:"
echo "  practicals/$P"
echo "  docs/p$P"
echo "Next: scripts/new-task.sh $P 01"
