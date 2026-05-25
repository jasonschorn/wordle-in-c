# Wordle-C

## Overview
My attempt to write World in C in order to learn the C programming language.  You may need to go into the file
wordle.c and adjust the constants for font size, tile size, keypad size since this was also made for a slightly
blind friend who loves to play Wordle.

## Running Wordle
Copy-and-paste the following into your terminal.  Note that raylib-src will be downloaded and this takes time.

    git clone https://github.com/jasonschorn/wordle-in-c
    cd wordle-in-c
    mkdir build && cd build
    cmake ..
    cmake --build .
    ./wordle-in-c