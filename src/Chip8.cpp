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


unsigned short Chip8::extract_address(unsigned short opcode) {

    return (opcode & 0x0FFF);
}

RegisterIdentifiers Chip8::extract_reg_identifer(unsigned short opcode) {
RegisterIdentifiers extraction; 

    extraction.x = opcode & 0x0F00;
    extraction.y = opcode & 0x00F0;

return extraction;
}

void Chip8::opcode0NNN(unsigned short opcode) {
unsigned short address = 0;

    stack[sp] = pc;
    address = extract_address(opcode);
    pc = address;
    sp += 1;

return;
}

void Chip8::opcode00E0(unsigned short opcode) {
    clear_display();
return;
}

void Chip8::opcode1NNN(unsigned short opcode) {
unsigned short address = 0;

    address = extract_address(opcode);
    pc = address;

return;
}

void Chip8::opcode2NNN(unsigned short opcode) {
unsigned short address = 0;

    stack[sp] = pc;
    address = extract_address(opcode);
    pc = address;
    sp += 1;

return;
}

void Chip8::opcode3XNN(unsigned short opcode) {
unsigned short address = 0;

    address = extract_address(opcode);



}