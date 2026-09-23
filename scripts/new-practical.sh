#!/usr/bin/env bash
set -euo pipefail

P="${1:-}"

if [[ -z "$P" ]]; then
    echo "Usage: $0 <practical>"
    echo "Example: $0 02"
    exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

PRACTICAL="practicals/$P"
DOCS="docs/p$P"
ASSETS="$DOCS/assets"

if [[ -d "$PRACTICAL" || -d "$DOCS" ]]; then
    echo "Practical $P already exists."
    exit 1
fi

mkdir -p "$PRACTICAL/results"

mkdir -p "$DOCS/requirements"
mkdir -p "$DOCS/sections"
mkdir -p "$ASSETS"
mkdir -p "$ASSETS/screenshots"
mkdir -p "$ASSETS/tasks"
mkdir -p "$DOCS/output"

touch "$DOCS/main.typ"
touch "$DOCS/config.typ"
touch "$DOCS/requirements/report.md"
touch "$DOCS/sections/introduction.typ"
touch "$DOCS/sections/conclusion.typ"

echo "Created practical $P"
echo " $PRACTICAL"
echo " $DOCS"
