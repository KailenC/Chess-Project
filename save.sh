#!/bin/bash
# save.sh - usage: ./save.sh v2-ep-castling-fixed
mkdir -p bin/versions
cp bin/chess_engine "bin/versions/chess_engine_$1"
echo "Saved as chess_engine_$1"