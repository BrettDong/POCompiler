#!/usr/bin/env bash

if [ -z "$1" ]; then
    echo "Usage: ./validation.sh <path to POCompiler executable> <reference PO file>"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "Executable does not exist"
    exit 1
fi

if [ ! -f "$2" ]; then
    echo "Sample PO file does not exist"
    exit 1
fi

set -e
msgfmt -f -o ref.mo "$2"
msgunfmt -o ref.po ref.mo
rm ref.mo

"$1" -o test.mo "$2"
msgunfmt -o test.po test.mo
rm test.mo

sharef=$(shasum -a 256 ref.po | cut -d' ' -f 1)
shatest=$(shasum -a 256 test.po | cut -d' ' -f 1)

set +e

if [ "$sharef" = "$shatest" ]; then
    echo "POCompiler output matches with that of system msgfmt"
    rm ref.po test.po
    exit 0
else
    echo "POCompiler output differs from that of system msgfmt"
    echo "POCompiler   : $shatest"
    echo "system msgfmt: $sharef"
    echo "Output files left for inspection."
    exit 1
fi
