#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n) {
  /* YOUR CODE HERE */
  return (int)field << (32-n)>>(32-n);
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */
Instruction parse_instruction(uint32_t instruction_bits) {
  /* YOUR CODE HERE */
  Instruction instruction;
  // add x8, x0, x0     hex : 00000433  binary = 0000 0000 0000 0000 0000 01000
  // Opcode: 0110011 (0x33) Get the Opcode by &ing 0x1111111, bottom 7 bits
  instruction.opcode = instruction_bits & ((1U << 7) - 1);

  // Shift right to move to pointer to interpret next fields in instruction.
  instruction_bits >>= 7;

  switch (instruction.opcode) {
  // R-Type
  case 0x33:
  case 0x0b:
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;
  // case for I-type 
  case 0x03:
  case 0x13:
  case 0x73:
     instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5; 

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;

    //case for S-type
     case 0x23:

     instruction.stype.imm5 = instruction_bits &((1U << 5) -1);
     instruction_bits >>=5;
     // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.stype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.stype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.stype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.stype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;
   // case for B-type
   case 0x63:
   instruction.sbtype.imm5 = instruction_bits &((1U << 5) -1);
     instruction_bits >>=5;
     // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.sbtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.sbtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.sbtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.sbtype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;
  //case for U-type
  case 0x37:
   instruction.utype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;
    
  instruction.utype.imm = instruction_bits & ((1U << 20) - 1);
  break;

  //case for J-type
  case 0x6f:
instruction.ujtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;
instruction.ujtype.imm = instruction_bits & ((1U << 20) - 1);
  break;

  default:
    exit(EXIT_FAILURE);
  }
  return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using
 * the given branch instruction */
int get_branch_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int offset = 0x0000000;
  // offset |= (instruction.sbtype.imm7 >> 5) & 0x00000001;
  // offset |= (instruction.sbtype.imm5) & 0x0000001e;
  // offset |= (instruction.sbtype.imm7 << 5) &  0x000007e0;
  // offset |= (instruction.sbtype.imm7 << 11) &  0x00000800;

  offset |= (instruction.sbtype.imm5 >> 1) & 0x0000000f;
  offset |= (instruction.sbtype.imm7  << 4) & 0x000003f0;
  offset |= (instruction.sbtype.imm5 << 10) &  0x00000400;
  offset |= ((instruction.sbtype.imm7 >> 6) << 11) &  0x00000800;
  offset <<=1;

  return sign_extend_number(offset,13);
}


int get_jump_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  
  int offset = 0x00000000;
  offset |= (instruction.ujtype.imm >> 9) & 0x003ff;
  offset |= (instruction.ujtype.imm << 2) & 0x00400;
  offset |= (instruction.ujtype.imm << 11) & 0x7f800;
  offset |= (instruction.ujtype.imm) & 0x80000;
   offset<<=1U;

  return sign_extend_number(offset,21);
}

int get_store_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int offset = 0x0000000;
  offset |= instruction.stype.imm5 & 0x0000001f;
  offset |= (instruction.stype.imm7 << 5) &  0x00000fe0;

  return sign_extend_number(offset,12);
}

void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}

void handle_invalid_read(Address address) {
  printf("Bad Read. Address: 0x%08x\n", address);
  exit(-1);
}

void handle_invalid_write(Address address) {
  printf("Bad Write. Address: 0x%08x\n", address);
  exit(-1);
}
