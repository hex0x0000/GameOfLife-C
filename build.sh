#!/usr/bin/env bash
cd $PROJ_ROOT
cat gosper_glider_gun | python to_coords.py O > build/glider_gun
cd $PROJ_ROOT/build
gcc -Wall -Wextra -pedantic-errors -Werror -Wformat=2 -std=c17 -O3 -DDEBUG -DFILE_LOGS="\"logs.txt\"" -DFONT="\"$FONT\"" \
    $(pkg-config --cflags --libs sdl2) -L$SDL2TTF/lib -I$SDL2TTF/include -lSDL2_ttf -lpthread \
    ../src/*.c -o GameOfLife
