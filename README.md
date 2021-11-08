Project on Instruction Set Architecture

In this project, I have created a disassembler and an emulator that is able to execute a subset of the RISC-V ISA. 32-bit RISC-V emulator that emulates basic load, store and mathematical calculations within memory and registers. The disassembler could take a machine level binary code and disassemble it into its assembly language. 

# This was a class project so all the information regarding the project is given below:

Background
In this project, you will create an emulator that is able to execute a subset of the RISC-V ISA. You’ll provide the machinery to decode and execute a couple dozen RISC-V instructions.

What is a disassembler and emulator ?


A disassembler translates an executable binary to a human-readable assembly program. An emulator is a software model representing the processor. Emulators are useful when you wish to run a program targeting a different CPU than the underlying hardware. For instance, we are going to be emulating RISC-V programs on an x86 processor of your laptop or CSIL machine. Perhaps the most popular emulator framework is Qemu. The virtual box software that you are using is a type of emulator.

Check Yourself
Bitfields and Unions
The RISC-V ISA card provides some information necessary for completing this project.
Lab 4/5 Video
WARNING: YOU CANNOT USE THE CODE IN VIDEO VERBATIM i.e., You cannot keep video open in one window and code in another window. Verbatim reproduction is considered to be academic dishonesty

Source
Make sure you read through the entire spec before starting the project.

The files you will need to modify and submit are:

TODO

part1.c: The main file which you will modify for part 1.
utils.c: The helper file which will hold various helper functions for part 1; as well as parsing the instruction. If you get parse_instruction wrong then part1 and part2 will print bogus values.
part2.c: The main file which you will modify for part 2. You will not be submitting this file for part 1.
You will NOT be modifying any files other than the ones listed above. This means that you will not be modifying header files. If you add helper functions, please place the function prototypes in the corresponding C files. If you do not follow this step, your code will likely not compile and you will get a zero on the project.

You should definitely consult through the following, thoroughly:

types.h: C header file for the data types you will be dealing with. It is critical that you read and understand the definitions in types.h before starting the project. If they look mysterious, consult the bitfields and unions ppt with this repo.
Makefile: File which records all dependencies.
code/input: Various files to run tests.
code/ref: Reference outputs (_.solution - part 1 . _.trace - part 2)
utils.h: File that contains the format for instructions to print for part 1.
You should not need to look at these files, but here they are anyway:

riscv.h: C header file for the functions you are implementing.
riscv.c: C source file for the program loader and main function.
$ bash ./install-cunit.sh # Install unit testing framework. This will create a folder called CUnit/
$ make riscv
RISC-V Emulator
The files provided in the start kit comprise a framework for a RISC-V emulator.

In part 1, you will add code to part1.c and utils.c to print out the human-readable disassembly corresponding to the instruction’s machine code. Look for /* Your Code */
In part 2, you’ll complete the program by adding code to part2.c to execute each instruction (including performing memory accesses). Your simulator must be able to handle the machine code versions of the RISC-V machine instructions below. We’ve already given you a framework for what cases of instruction types you should be handling.
Check yourself: why does sizeof(Instruction)==4?

The instruction set that your emulator must handle is listed below. All of the information here is copied from the RISC-V green sheet for your convenience; you may still use the green card as a reference.



Just like the regular RISC-V architecture, the RISC-V system you’re implementing is little-endian. This means that when given a value comprised of multiple bytes, the least-significant byte is stored at the lowest address.

The Framework Code
The framework code we’ve provided operates by doing the following.

It reads the program’s machine code into the simulated memory (starting at address 0x01000). The program to “execute” is passed as a command line parameter. Each program is given 1 MiB of memory and is byte-addressable.
It initializes all 32 RISC-V registers to 0 and sets the program counter (PC) to 0x01000. The only exceptions to the initial initializations are the stack pointer (set to 0xEFFFF) and the global pointer (set to 0x03000). In the context of our emulator, the global pointer will refer to the static portion of our memory. The registers and Program Counter are managed by the Processor struct defined in types.h.
It sets flags that govern how the program interacts with the user. Depending on the options specified on the command line, the simulator will either show a dissassembly dump (-d) of the program on the command line, or it will execute the program. More information on the command line options is below.
It then enters the main simulation loop, which simply executes a single instruction repeatedly until the simulation is complete. Executing an instruction performs the following tasks:

It fetches an instruction from memory, using the PC as the address.
It examines the opcode/funct3 to determine what instruction was fetched.
It executes the instruction and updates the PC.
The framework supports a handful of command-line options:

-i	runs the simulator in interactive mode, in which the simulator executes an instruction each time the Enter key is pressed. The disassembly of each executed instruction is printed.
-t	runs the simulator in tracing mode, in which each instruction executed is printed.
-r	instructs the simulator to print the contents of all 32 registers after each instruction is executed. (Part 2) This option is most useful when combined with the -i flag.
-d	instructs the simulator to disassemble the entire program, then quit before executing. (Part 1)
-e	Part 2. Instructs the simulator to simulate till the end of the program (helpful when programs have loops)
Part 1 Disassembler
In part 1, you will be writing a disassembler that translates RISC-V machine code into human-readable assembly code. You will also be laying the building blocks for the actual emulator that you will implement in part 2. You will be implementing the following functions. The files in which they are located are in parentheses.

parse_instruction(instruction_bits) (utils.c): Uses the given instruction (encoded as a 32-bit integer) and returns the corresponding instruction. You will have to determine the proper format of the given instruction and use the correspdonding member of the instruction struct. You will find the green sheet particularly helpful here.
sign_extend_number(field, n) (utils.c): This function interprets the number in field as an n-bit number and sign-extends it to a 32-bit integer.
get_*_offset(instruction) (utils.c): For the corresponding instruction type (either branch, jump, or store), this function unpacks the immediate value and returns the number of bytes to offset by. In this case of branches and jumps, these functions should return the number of bytes to add to the PC to get to the desired label. In the case of stores, the corresponding function will return the offset on the destination address.
print_*(instruction) (part1.c): Prints the instruction to standard output. You should use the constants defined in the file utils.h to properly format your instructions. Failure to do so will cause issues with the autograder. You should also refer to registers by their numbers and not their names.
Part 2 Emulator
For part 2 of this project, you will be implementing the actual emulator that can execute RISC-V machine code. In order to accomplish this task, you will need to complete the functions below. Program Counter PC and Program are loaded starting at 0x1000 (4096).

execute\_\*(): These functions handle the majority of the execution of the reduced RISC-V instruction set we are implementing in this project. To reduce the amount of busy work, the ladders of switch statements are already implemented for you. To complete the implementation, you will only need to fill in the switch cases. You should be updating the appropriate register values and interacting with memory when applicable.
store(): This function takes an address, a size, and a value and stores the first -size- bytes of the given value at the given address.
load(): This function accepts an address and a size and returns the next -size- bytes starting at the given address. You will need to implement load() first in order to fetch instructions properly; without a functioning load() you will get errors due to invalid instructions.
Note that a correct implementation of this part will depend on the functions in utils.c. Thus, you should ensure that these functions (which you wrote in part 1) are working correctly. By the time you’re finished, your code should handle all of the instructions in the table above.

PC operations
The program counter (PC) register in the RISC V processor is special. It is a pointer to the memory location of the next instruction that will be run by the processor.

In order to make progress this has to be automatically incremented or changed after running the current instruction. If it appears that your program is not making any progress e.g., it is stuck running the same instruction repeatedly then check whether you are changing the PC register after running the current instruction. This is done within execute_*()
The program counter can be written explicitly by multiple types of instructions branches (SB type), JAL (J type), JALR (I type). These all provide immediates of various widths that need to be sign extended. Make sure that you are calculating the immediates appropriately based on the instruction type. Make sure you pass part 1 and are able to disassemble these instructions appropriately; without this its futile to attempt part 2
You may also need to do some arithmetic on the offset before setting the PC. This arithmetic may need you to read other registers (jalr) or even the PC itself (e.g., jal). Sometimes instructions may update the PC and other registers (e.g., JAL)
Finally, remember PC itself is just a pointer. Every step the processor has to read the memory location pointed to by the PC. This requires you to implement the load() and read memory (atleast 4 byte loads. Think why implementing 4 byte aligned loads is sufficient for running instructions?)
Part 2 Debugging
$ cgdb riscv
# Inside cgdb shell
$ run -r code/input/[*].input
$ run -r -i code/input[*].input # interactive emulator mode.
Testing



Unit tests
$ touch test_utils.c; make test-utils
If you see any build errors; check if you have run the CUnit install at the beginning.

$ bash ./install-cunit.sh # Install unit testing framework. This will create a folder called CUnit/
Instruction tests
Setup

bash ./scripts/localci.sh
# This will create the code/out folder. This is required for further testing below. This will also create files; make sure you overwrite them with the steps below before looking at them. Otherwise the .trace files in code/out/ might be stale.
YOU CAN ONLY READ (NOT MODIFY) THE FILES IN code/input and code/ref. HOWEVER YOU CAN UPDATE THE FILES in code/out. DO NOT GIT ADD code/out; OTHERWISE YOU WILL FAIL travis

Part 1

$ ./riscv -d ./code/input/R/R.input > ./code/out/R/R.solution
$ diff ./code/out/R/R.solution ./code/ref/R/R.solution
# You can replace R with any of the other instruction types R, I, S, SB, U, J
Part 2

$ ./riscv -r ./code/input/R/R.input > ./code/out/R/R.trace
$ python3 part2_tester.py R/R
# If there is any error, check if code/out/R/R.trace exists ? The python script will report errors in particular traces points if any. You can replace R with any of the other instruction types R, I, S, SB, U, J Ls are also in I/ folder as they are type of I
$ ./riscv -d ./code/input/I/L.input > ./code/out/I/L.solution
$ diff ./code/out/I/L.solution ./code/ref/I/L.solution
$ ./riscv -r ./code/input/I/L.input > ./code/out/I/L.trace
$ python3 part2_tester.py I/L
If your trace does not match the output, you will get the difference between the reference output and your output. Make sure you at least pass this test before modifying part1.c

R with init
SPECIAL TEST CASE FOR R instructions ONLY Ri IS NOT A SPECIAL INSTRUCTION TYPE.

Part 2 needs to check actual values in registers. To help we add an additional flag to help test R instructions. In this case we initialize registers (other than sp:x2 and frame pointer:x3 ) to 4. Notice the Ri. This is to indicate we are using the inited version of the CPU.

# Ri.input is same as R.input. But the -v flag inits the registers to 4. If you leave out the -v then your trace will not match
./riscv -v -r ./code/input/Ri/Ri.input > ./code/out/Ri/Ri.trace
$ python3 part2_tester.py Ri/Ri
# You get points for R instructions: part 2 only if you pass the Ri part 2.
Part 2 tests need to check the actual values in the registers. Program tests are the recommended method for debugging Part 2.

Program tests
# The possible test files are called simple, multiply, random.
$ ./riscv -r -e ./code/input/simple.input > ./code/out/simple.trace
$ python3 part2_tester.py simple
# Replace simple with multiply or random
An example error output

$ ./riscv -r -e ./code/input/random.input > ./code/out/random.trace
$ python3 part2_tester.py random

Starting random test
ERROR: instruction 10, register 5. Expected: 0x00000000, Actual: 0xffffffff
random test has failed.
You are provided with three test files that evaluate your emulator’s correctness. The output files are those with the .trace extension. The solutions are located in the folder code/ref, while your project’s code will write it’s output to the folder code/out. These trace files dump the contents of the registers after the execution of each instruction.

The testing suite is run through the python script part2_tester.py. This file will read in your output and compare it against the reference trace by looking at changes in the registers between instructions. If a test fails, you should see an output that lists which instruction and register was incorrect. You can then cross-reference this information with the tests’ .solution file (the disassembled version from part 1) to find the erroneous error.

If you would like to add new tests, you should follow the same steps as those outlined when creating tests for part 1. The only difference is that you need to create a .trace reference file that contains the register values after every instruction

End-to-End tests
Congratulations! You are ready for end-to-end tests

$ bash ./scripts/localci.sh
# if you see a file called SUCCESS in your repo and *.log.sucess then you passed. You can also check your *_Grade.json to see your tentative grade If you see FAILED, then inspect *.log.failed. Check the failed section in LOG.md to see what tests failed. LOG.md is a markdown file. You can view it using any markdown viewer. Copy paste LOG.md into this viewer e.g., https://markdownlivepreview.com/
Part 3 Custom Instructions
DO NOT ATTEMPT THIS WITHOUT COMPLETING PART 1 and 2

The RISC-V ISA enables the designer to customize and extend the ISA. It reserves bits in the opcode space so that designers can define new operations. In digital signal processors, accumulate operation is a common step that computes the product of two numbers and adds that product to an accumulator . The hardware unit that performs the operation is known as a accumulator; the operation itself is also often called a acc operation. The acc operation is defined as:

Instruction	Type	Opcode	Funct3	Funct7	Operation
acc rd,rs1,rs2	R	0x0b	0x1	0x0	rd = rd + rs1 + rs2
We are going to add one such new instruction to our disassembler (part 1) and emulator (part 2). You may need new or modify existing functions in part1.c, part2.c, utils.c. WARNING: You cannot modify the arguments and return values of any existing functions. You cannot modify any of the headers

# The test file is custom
$ $ ./riscv -d ./code/input/custom.input > ./code/out/custom.solution
$ diff ./code/out/custom.solution ./code/ref/custom.solution
$ ./riscv -r -e ./code/input/custom.input > ./code/out/custom.trace
$ python3 part2_tester.py custom
Grading
Congrats. You are now ready to receive your tentative grade.

$ cd $REPO # $REPO refers to the top of your cloned repo.
$ bash ./scripts/localci.sh # run from the repo folder
# Check if SUCCESS OR FAILED was dumped by scripts. There will be a _Grade.json (this is your tentative grade). The file is in json format. The mark component in each module indicates your score for that portion. LOG.md contains the log of your runs. This might give you more information.
This grade is only tentative. Based on additional test cases in our evaluation, you could score less points.

