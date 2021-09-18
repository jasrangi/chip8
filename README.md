# CHIP-8
CHIP-8 is an interpreted programming language designed to work on 8 bit 
computers. This repository contains the source code to an emulator for CHIP-8 
instructions to run on. 

## Purpose
This program was written to learn more about emulation. As such, its purpose
is strictly educational. 

## Build instructions
Dependencies: [SDL2](https://www.libsdl.org/download-2.0.php) \
For licensing information about SDL2, see the 
[SDL License page](https://www.libsdl.org/license.php). \
To build the CHIP-8 emulator clone this repository and run make.
```sh
git clone <url>
cd chip8
make
```

## Usage
The executable expects the folder that includes the CHIP-8 program to be the
current working directory. Before running, change the current working 
directory.
```sh
cd </path/to/program>
```
To run a CHIP-8 program, run the following in a
shell: 
```sh
</path/to/executable> <program> 
```
Here is an example of running PONG2, where the executable is located in the same
folder as PONG2.
```sh
./chip8emulator PONG2
```
Optionally, you can also specify the CPU clock speed and graphical window 
scale.
```sh
</path/to/executable> <program> <Clock Speed (Hertz)> <Window scale>
```
The default window size (scale = 1) is 64 x 32. Therefore a scale of 15 
will result in a window of size 960 x 480. If no arguments for clock 
speed and window scale are provided, they will be set to 1000 and 15
respectively.
### Controls
A CHIP-8 keypad has 16 keys. Below is a diagram of how the emulator maps
the keys on your keyboard (left) to CHIP-8 keys that a CHIP-8 program 
works with (right). \
1 2 3 4 &nbsp; -> &nbsp; &nbsp; 1 2 3 C \
Q W E R &nbsp; -> &nbsp; 4 5 6 D \
A S D F &nbsp; -> &nbsp; &nbsp; 7 8 9 E \
Z X C V &nbsp; -> &nbsp; &nbsp; A 0 B F

## Current Limitations
This emulator has only been tested to work well with PONG2 on default 
settings. However, it may work with other programs if the clock speed is 
properly set.

## Sources
The following sources were used to understand CHIP-8 Hardware specifications
and instruction set: \
[Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) \
[CHIP-8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8#The_stack) \
The following source was used to learn about SDL2 input handling and
video output: \
[SDL2 API Wiki](https://wiki.libsdl.org/APIByCategory) \
The source code uses the Google C++ Style Guide. See it here: \
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

## License
[MIT](https://choosealicense.com/licenses/mit/)
