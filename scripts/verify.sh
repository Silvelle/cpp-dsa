#!/usr/bin/env bash
set -euo pipefail

P="${1:-}"
T="${2:-}"

if [[ -z "$P" || -z "$T" ]]; then
    echo "Usage: $0 <practical> <task>"
    echo "Example: $0 01 02"
    exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

echo "Verifying P=$P T=$T"

make verify P="$P" T="$T"

echo
echo "Verification finished successfully."
