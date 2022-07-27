#!/usr/bin/env bash
cd $PROJ_ROOT
cat gosper_glider_gun | python to_coords.py O > build/glider_gun
cd $PROJ_ROOT/build
git clone https://github.com/mlabbe/nativefiledialog
cd nativefiledialog/build/gmake_linux
make nfd
cd $PROJ_ROOT/build
gcc -Wall -Wextra -pedantic-errors -Werror -Wformat=2 -std=c17 -O3 -DDEBUG -DFILE_LOGS="\"logs.txt\"" -DFONT="\"$FONT\"" \
    $(pkg-config --cflags --libs sdl2) $(pkg-config --cflags --libs SDL2_ttf) -lpthread \
    -Inativefiledialog/src/include -l:nativefiledialog/build/lib/Release/x64/libnfd.a \
    ../src/*.c -o GameOfLife
