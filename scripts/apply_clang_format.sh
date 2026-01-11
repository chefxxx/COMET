#!/usr/bin/env bash

# ===============================
# Configuration Variables
# ===============================

FILE_EXTENSIONS=("cpp" "cc" "c" "h" "hpp" "tpp")
CLANG_FORMAT_STYLE="file"

PATH_TO_DIRS="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )/.." &> /dev/null && pwd )"

FOLDERS=(
  "$PATH_TO_DIRS/lib"
  "$PATH_TO_DIRS/tests"
)

# ===============================
# Script Execution
# ===============================

num_cores=$(nproc)
echo "Using $num_cores CPU cores for formatting."

for TARGET_FOLDER in "${FOLDERS[@]}"; do
    echo "Processing folder: $TARGET_FOLDER"

    find_command=(find "$TARGET_FOLDER" -type f \( )
    for ext in "${FILE_EXTENSIONS[@]}"; do
        find_command+=(-name "*.$ext" -o)
    done
    unset 'find_command[${#find_command[@]}-1]'
    find_command+=(\) )

    "${find_command[@]}" | xargs -P "$num_cores" -I {} clang-format-17 -i -style="$CLANG_FORMAT_STYLE" "{}"
done

echo "Formatting completed."