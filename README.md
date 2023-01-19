# Instruction Set Architecture Project

<img src="https://user-images.githubusercontent.com/84153519/213369013-ae4b587a-603b-4924-be73-f1cde788dc4e.png" width="420" height="300">

In this project, I have created a disassembler and an emulator for a subset of the RISC-V ISA. The 32-bit RISC-V emulator emulates basic load, store and mathematical calculations within memory and registers. The disassembler can take a machine level binary code and disassemble it into its assembly language.

## Background

This project involves creating an emulator that is able to execute a subset of the RISC-V ISA. The goal is to provide the machinery to decode and execute a couple dozen RISC-V instructions.

## What is a disassembler and emulator?

A disassembler is a tool that translates an executable binary to a human-readable assembly program. An emulator is a software model representing the processor. Emulators are useful when you wish to run a program targeting a different CPU than the underlying hardware. In this project, we will be emulating RISC-V programs on an x86 processor of your laptop or CSIL machine.

## Check Yourself: Bitfields and Unions

The RISC-V ISA card provides some information necessary for completing this project. Lab 4/5 Video WARNING: YOU CANNOT USE THE CODE IN VIDEO VERBATIM i.e., You cannot keep video open in one window and code in another window. Verbatim reproduction is considered to be academic dishonesty

## Part 1

In part 1, you will add code to part1.c and utils.c to print out the human-readable disassembly corresponding to the instruction’s machine code. Look for /* Your Code */ 

## Part 2

In part 2, you’ll complete the program by adding code to part2.c to execute each instruction (including performing memory accesses). Your simulator must be able to handle the machine code versions of the RISC-V machine instructions below. We’ve already given you a framework for what cases of instruction types you should be handling. Check yourself: why does sizeof(Instruction)==4?

## The instruction set

The instruction set that your emulator must handle is listed below. All of the information here is copied from the RISC-V green sheet for your convenience; you may still use the green card as a reference.

Just like the regular RISC-V architecture, the RISC-V system you’re implementing is little-endian. This means that when given a value comprised of multiple bytes, the least-significant byte is stored at the lowest address.

## The Framework Code

The framework code we’ve provided operates by doing the following:

1. It reads the program’s machine code into the simulated memory (starting at address 0x01000). The program to “execute” is passed as a command line parameter. Each program is given 1 MiB of memory and is byte-addressable.
2. It initializes all 32 RISC-V registers to 0 and sets the program counter (PC) to 0x01000. The only exceptions to the initial initializations are the stack pointer (set to 0xEFFFF) and the global pointer (set to 0x03000).
3. In the context of our emulator, the global pointer will refer to the static portion of our memory. The registers and Program Counter are managed by the Processor struct defined in types.h.
4. It sets flags that govern how the program interacts with the user. Depending on the options specified on the command line, the simulator will either show a dissassembly dump (-d) of the program on the command line , or it will execute the program. More information on the command line options is below. 

5. It then enters the main simulation loop, which simply executes a single instruction repeatedly until the simulation is complete. Executing an instruction performs the following tasks:
    - It fetches an instruction from memory, using the PC as the address. 
    - It examines the opcode/funct3 to determine what instruction was fetched. 
    - It executes the instruction and updates the PC. 

## Command-line options
The framework supports a handful of command-line options:

- **-d**: Show a disassembly dump of the program on the command line
- **-e**: Execute the program

### Example
$ ./riscv -d ./code/input/R/R.input > ./code/out/R/R.s

This command will run the disassembler on the program at `./code/input/R/R.input` and output the assembly code to `./code/out/R/R.s`

## Additional Command Line options

- **-i**: Specify the memory initialization file. This file should contain a list of memory addresses and their corresponding values, separated by whitespace.
- **-r**: Specify the register initialization file. This file should contain a list of register names and their corresponding values, separated by whitespace.
- **-m**: Specify the memory size in bytes. By default, the emulator uses 1 MiB of memory.

### Example
$ ./riscv -e -i mem_init.txt -r reg_init.txt -m 2048 ./code/input/R/R.input

This command will run the emulator on the program at `./code/input/R/R.input` using the memory initialization file `mem_init.txt` and register initialization file `reg_init.txt`. The memory size is set to 2048 bytes.

By providing these command line options, you can easily configure the emulator to your specific needs and requirements, making it more versatile and flexible.

Please be aware that this is a simplified version of RISC-V emulator, it can handle only a limited subset of RISC-V instruction set and it's not intended to be used in any production environment.
