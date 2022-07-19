{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell rec {
    packages = with pkgs; [
        gcc
        valgrind
        python38
    ];

    inputsFrom = with pkgs; [
        SDL2
        SDL2_ttf
        terminus_font_ttf
    ];

    PROJ_ROOT = "$PWD";

    shellHook = ''
    rm -rf build 2>/dev/null
    mkdir build
    export PROJ_ROOT=${PROJ_ROOT}
    export SDL2TTF=${pkgs.SDL2_ttf}
    export FONT=${pkgs.terminus_font_ttf}/share/fonts/truetype/TerminusTTF.ttf
    alias build="cd $PROJ_ROOT/build && ./../build.sh"
    '';
}