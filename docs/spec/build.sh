#!/bin/sh
# Build the specification into site/ (open site/index.html).
# Needs apmu-os and alsaqr-software side by side (headers are included verbatim).
# VENV overrides where the Python environment lives (default: .venv here).
set -e
cd "$(dirname "$0")"
VENV=${VENV:-.venv}
if [ ! -x "$VENV/bin/mkdocs" ]; then
    python3 -m venv "$VENV"
    "$VENV/bin/pip" install -q -r requirements.txt
fi
"$VENV/bin/mkdocs" build --strict "$@"
