{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell rec {
    packages = with pkgs; [
        gcc
        SDL2
        terminus_font_ttf
        gnumake
        gtk3
        pkg-config
        SDL2_ttf
        valgrind
        python38
	clang
    ];

    PROJ_ROOT = "$PWD";

    shellHook = ''
    rm -rf build 2>/dev/null
    mkdir build
    export PROJ_ROOT=${PROJ_ROOT}
    export FONT=${pkgs.terminus_font_ttf}/share/fonts/truetype/TerminusTTF.ttf
    alias build="cd $PROJ_ROOT/build && ./../build.sh"
    '';
}
