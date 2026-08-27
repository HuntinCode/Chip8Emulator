#include "Chip8.h"
#include <fstream>
#include <iostream>


Chip8::Chip8() {
    reset();
}


/* 
Returns the opcode from memory, and increments the program counter by 2.
Calling this again will get you the next opcode. 
*/
unsigned short Chip8::get_next_opcode() {
unsigned short op = 0;

    op = memory[pc];
    op << 8;
    op |= memory[pc + 1];

    pc += 2;

return op;
}

void Chip8::load_rom(std::string file_name) {
std::ifstream file(file_name, std::fstream::in | std::fstream::binary);
unsigned short length = 0;
unsigned char file_buffer[0xE00];

    if (!file)
        std::cerr << "Failed to open file\n";

    file.seekg(0, std::ios_base::end);
    length = file.tellg();
    file.seekg(0, std::ios_base::beg);

    file.read(reinterpret_cast<char *>(&file_buffer), length);

    if (file) {

        std::cout << "File is loaded succesfully. Now loading into memory." << std::endl;
        for (int i = 0; i < length; i++)
            memory[pc + i] = file_buffer[i];

    } else {

        std::cout << "File stopped loading at" << file.gcount() << " characters" << std::endl;
    }
    
    file.close();

return;
}

void Chip8::reset() {

    pc = 0x200;
    sp = 0;
    d_timer = 0;
    s_timer = 0;
    opcode = 0;
    

    clear_display();
    clear_mem();

return;
}

void Chip8::clear_display() {
    for (int i = 0; i < SCREEN_LENGTH; i++) 
        for (int j = 0; i < SCREEN_WIDTH; i++)
            screen_data[i][j] = 0;

return;
}

void Chip8::clear_mem() {
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) 
        memory[0] = 0;

return;
}

// Extracts address from the byte and nibble.
unsigned short Chip8::extract_address(unsigned short opcode) {

    return (opcode & 0x0FFF);
}

/// Extracts the register identifers from the two nibbles in the opcode. Doesn't necessary guarantee those nibbles are identfiers.
/// Need to look at the X & Y letters in the opcode to determine those are indeed register identifiers. 
RegisterIdentifiers Chip8::extract_reg_identifer(unsigned short opcode) {
RegisterIdentifiers extraction; 

    extraction.x = opcode & 0x0F00;
    extraction.y = opcode & 0x00F0;

return extraction;
}

// jumps to a machine function.
void Chip8::opcode0NNN(unsigned short opcode) {
unsigned short address = 0;

    stack[sp] = pc;
    address = extract_address(opcode);
    pc = address;
    sp += 1;

return;
}

// resets the screen data.
void Chip8::opcode00E0(unsigned short opcode) {
    clear_display();
return;
}

// jumps to a defined address.
void Chip8::opcode1NNN(unsigned short opcode) {
unsigned short address = 0;

    address = extract_address(opcode);
    pc = address;

return;
}

// calls a function at the designated address.
void Chip8::opcode2NNN(unsigned short opcode) {
unsigned short address = 0;

    stack[sp] = pc;
    address = extract_address(opcode);
    pc = address;
    sp += 1;

return;
}

// compares the value in V[x] register, with NN. If, they're equal, then skip the next instruction otherwise proceed.
void Chip8::opcode3XNN(unsigned short opcode) {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = opcode & 0x0F00;

    if (v[xReg] == byte)
        pc += 4;
    else
        pc += 2;

return;
}

// Compares the value of register X with NN. If they're not equal to each other it will skip the next instruction.
void Chip8::opcode4XNN(unsigned short opcode) {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = opcode & 0x0F00;

    if (v[xReg] != byte)
        pc += 4;
    else
        pc += 2;

return;   
}

// Compares the value of register X and Y. If they're equal, it skips the next instruction.
void Chip8::opcode5XY0(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    if (v[reg.x] == v[reg.y])
        pc += 4;
    else
        pc += 2;

return;
}

// Sets the value of vX register to the byte NN.
void Chip8::opcode6XNN(unsigned short opcode) {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = opcode & 0x0F00;

    v[xReg] = byte;
    pc += 2;

return;
}

// Adds the byte to the vX register without affecting the carry flag.
void Chip8::opcode7XNN(unsigned short opcode) {

    v[opcode & 0x0F00] += (opcode & 0x00FF);
    pc += 2;

    return;
}

// sets the value of vX register to vY register. 
void Chip8::opcode8XY0(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] = v[reg.y];
    pc += 2;

}

// Sets the value of vX to vX OR vY using bitwise operation.
void Chip8::opcode8XY1(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] |= v[reg.y];
    pc += 2;

}

// Sets the value of vX to vX and vY using bitwise operation.
void Chip8::opcode8XY2(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] &= v[reg.y];
    pc += 2;

}

void Chip8::opcode8XY3(unsigned short opcode) {

}

void Chip8::opcode8XY4(unsigned short opcode) {

}

void Chip8::opcode8XY5(unsigned short opcode) {

}

void Chip8::opcode8XY6(unsigned short opcode) {
    
}

void Chip8::opcode8XY7(unsigned short opcode) {
    
}

void Chip8::opcode8XYE(unsigned short opcode) {
    
}

void Chip8::opcode9XY0(unsigned short opcode) {

}

void Chip8::opcodeANNN(unsigned short opcode) {

}

void Chip8::opcodeBNNN(unsigned short opcode) {

}

void Chip8::opcodeCXNN(unsigned short opcode) {

}

void Chip8::opcodeDXYN(unsigned short opcode) {
    
}

void Chip8::opcodeEX9E(unsigned short opcode) {
    
}

void Chip8::opcodeEXA1(unsigned short opcode) {
    
}

void Chip8::opcodeFX07(unsigned short opcode) {
    
}