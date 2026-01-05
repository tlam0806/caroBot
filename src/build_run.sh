#!/usr/bin/env bash
set -e  # có lỗi thì dừng luôn

SFML_PATH=$(brew --prefix sfml)

clang++ main.cpp board.cpp player.cpp bot.cpp match.cpp\
  -I"$SFML_PATH/include" \
  -L"$SFML_PATH/lib" \
  -lsfml-graphics -lsfml-window -lsfml-system \
  -std=c++17 \
  -o tictactoe3

./tictactoe3
