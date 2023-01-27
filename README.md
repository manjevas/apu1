# APple emUlator 1 (APU1): Apple 1 Emulator 

A simplified implemetation of the Apple 1 computer, done in a way that is discrete and logical. 

## Introduction
This project was an excercise for me to get re-acquainted with C-Programming Language and also get acquainted with low level machine code and assembly code. I had been using computers for over two decades and never really understood how everyting really worked, digging deep into Apple 1 and MOS 6502 processor, gave me glimpse of the modern beheamoth processors. I enjoyed putting together this project and it is far from done. Here is  a list of TODOs that I would like to get to if time permits.

### TODO:
- [ ] Add more functionality to the Function Key options
- [ ] Write tape read and write functionality
- [ ] Load in Integer Basic and test it out
- [ ] Fix issue with reading in keyboard characters (Symbols)

## Installation

This project has been developed in the WSL environment and designed to run only on unix based OS. In order to use the emulator, it has to be built locally on the machine. Here are the instructions on how it can be done:

1. Clone the GIT repo to a local drive
2. Create a folder named 'build' in the downloaded repo
3. Load a session of shell and navigate to the 'build' directory
4. Type in 'clear; cmake ..; make; apu1/apu1' to create makefile, build and run the program.
5. In the program, hit the F1 key to get a list of commands. 

## Structure [WIP]

### Processor

#### Architecture

#### Instructions

### Memory

#### RAM

#### ROM

### PIA

#### Keyboard

#### Display
