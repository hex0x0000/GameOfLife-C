# Conway's Game of Life
Yet another implementation of Conway's Game of Life written in C with SDL2

# Features
- Infinite grid (until int64_t limit is reached, at least)
- Moving around the grid
- Zoom-in Zoom-out
- Importing custom generations, you can generate them from a matrix with [to_coords.py](https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/to_coords.py). Example: `cat gosper_glider_gun | python to_coords.py O > build/glider_gun`
- Automatically exports current generation at exit

Future features:
- Changing settings within the game (generation speed, color, font size, etc)
- Editing the grid within the game
- Import/Export within the game

# Compiling
If you're on nixos:
```bash
$ nix-shell
$ build
```

If you're on any other distro:
1. Install SDL2 and SDL2_ttf
2. Make sure that `pkg-config` recognizes SDL2

After that (from the project's root):
```bash
$ export PROJ_ROOT="$(pwd)"
$ export FONT="/path/to/font.ttf" # whichever you want
$ export SDL2TTF="/usr/share" # or wherever SDL2_ttf is installed
$ mkdir build
$ cd build/
$ ../build.sh # chmod +x build.sh if you can't execute it
# done
```
