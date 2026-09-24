#!/usr/bin/env bash
set -euo pipefail

P="${1:-01}"
T="${2:-01}"

if [[ ! "$P" =~ ^[0-9]+$ || ! "$T" =~ ^[0-9]+$ ]]; then
    echo "Usage: $0 <practical> <task>"
    echo "Example: $0 01 02"
    exit 1
fi

printf -v P '%02d' "$((10#$P))"
printf -v T '%02d' "$((10#$T))"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

echo "Verifying P=$P T=$T"

make verify P="$P" T="$T"

echo
echo "Verification finished successfully."
