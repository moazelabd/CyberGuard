#!/usr/bin/env bash
set -e

mkdir -p include src third_party db scripts || true

move_file () {
    if [ -f "$1" ]; then
        git mv "$1" "$2" 2>/dev/null || mv "$1" "$2"
    fi
}

# Headers
move_file Database.h include/
move_file User.h include/
move_file Secret.h include/
move_file PasswordEntry.h include/
move_file Vault.h include/
move_file Utils.h include/

# Source files
move_file Database.cpp src/
move_file User.cpp src/
move_file PasswordEntry.cpp src/
move_file Vault.cpp src/
move_file Utils.cpp src/
move_file main.cpp src/

# SQLite
move_file sqlite3.c third_party/
move_file sqlite3.h third_party/

# Database file
move_file cyberguard.db db/

echo "✅ Reorganization complete."