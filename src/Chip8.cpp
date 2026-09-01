#include "Chip8.h"
#include <fstream>
#include <iostream>
#include <string>

Chip8::Chip8() {
    reset();
}

/* 
Returns the current opcode from memory, and increments the program counter by 2.
Calling this again will get you the next opcode. 
*/
unsigned short Chip8::get_next_opcode() {
unsigned short current_opcode = 0;

   std::cout << "Current value of program counter: " << pc << std::endl;
   current_opcode = memory[pc];
   current_opcode <<= 8;

   current_opcode |= memory[pc + 1];

return current_opcode;
}

void Chip8::load_rom(std::string file_name) {
std::ifstream file;
unsigned short length = 0;
unsigned char *buffer = nullptr;

    file.open(file_name, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new unsigned char[length];

    file.read(reinterpret_cast<char*>(buffer), length);
    file.close();

    for (int i = 0; i < length; i++)
        memory[pc + i] = buffer[i];

    delete [] buffer;
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
    clear_stack();

return;
}

void Chip8::clear_display() {
    for (int i = 0; i < SCREEN_LENGTH; i++) 
        for (int j = 0; j < SCREEN_WIDTH; j++)
            screen_data[i][j] = 0;

return;
}

void Chip8::clear_mem() {
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) 
        memory[i] = 0;

return;
}

void Chip8::clear_stack() {
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;

return;
}

// Extracts address from the byte and nibble.
unsigned short Chip8::extract_address() {

    return (opcode & 0x0FFF);
}

/// Extracts the register identifers from the two nibbles in the opcode. Doesn't necessary guarantee those nibbles are identfiers.
/// Need to look at the X & Y letters in the opcode to determine those are indeed register identifiers. 
RegisterIdentifiers Chip8::extract_reg_identifer() {
RegisterIdentifiers extraction; 

    extraction.x = (opcode & 0x0F00) >> 8;
    extraction.y = (opcode & 0x00F0) >> 4;

return extraction;
}

void Chip8::opcode0_group() {
unsigned char nibble = 0;

    nibble = (opcode & 0x000F);

    switch (nibble) {
        case 0x0:
            opcode00E0();
            break;
        case 0xE:
            opcode00EE();
            break;
        default:
            opcode0NNN();
            break;
    }

return;
}

void Chip8::opcode8_group() {
unsigned char nibble = 0;

    nibble = (opcode & 0x000F);
    switch (nibble) {
        case 0x0:
            opcode8XY0();
            break;
        case 0x1:
            opcode8XY1();
            break;
        case 0x2:
            opcode8XY2();
            break;
        case 0x3:
            opcode8XY3();
            break;
        case 0x4:
            opcode8XY4();
            break;
        case 0x5:
            opcode8XY5();
            break;
        case 0x6:
            opcode8XY6();
            break;
        case 0x7:
            opcode8XY7();
            break;
        case 0xE:
            opcode8XYE();
            break;
        default:
            break;
    }
return;
}

void Chip8::opcodeE_group() {
unsigned char nibble = 0;

    nibble = opcode & 0x000F;
    switch (nibble) {
        case 0xE:
            opcodeEX9E();
            break;
        case 0x1:
            opcodeEXA1();
            break;
        default:
            break;
    }
return;
}

void Chip8::opcodeF_group() {
unsigned char group_nibble = 0;
unsigned char id_nibble = 0;

    switch (group_nibble) {
        case 0x0:
            switch (id_nibble) {
                case 0x7:
                    opcodeFX07();
                    break;
                case 0xA:
                    opcodeFX0A();
                    break;
                default:
                    break;
            }
            break;

        case 0x1:
            switch(id_nibble) {
                case 0x5:
                    opcodeFX15();
                    break;
                case 0x8:
                    opcodeFX18();
                    break;
                case 0xE:
                    opcodeFX1E();
                    break;
                default:
                    break;
            }
            break;
        case 0x2:
            opcodeFX29();
            break;
        case 0x3:
            opcodeFX33();
            break;
        case 0x5:
            opcodeFX55();
            break;
        case 0x6:
            opcodeFX65();
            break;
        default:
            break;
    }

}

unsigned char Chip8::get_pressed_key() {
unsigned ret_value = 0;

    for (int i = 0; i < 16; i++)
        if (key_states[i] == true) {
            ret_value = i;
            break;
        }

return ret_value;
}

void Chip8::set_key(unsigned char key, bool isSet) {

        key_states[key] = isSet;

return;
}

void Chip8::execute_op() {
unsigned char nibble = 0; 

    nibble = (opcode & 0xF000) >> 12;
    
    switch (nibble) {
        case 0x0:
            opcode0_group();
            break;
        case 0x1:
            opcode1NNN();
            break;
        case 0x2:
            opcode2NNN();
            break;
        case 0x3:
            opcode3XNN();
            break;
        case 0x4:
            opcode4XNN();
            break;
        case 0x5:
            opcode5XY0();
            break;
        case 0x6:
            opcode6XNN();
            break;
        case 0x7:
            opcode7XNN();
            break;
        case 0x8:
            opcode8_group();
            break;
        case 0x9:
            opcode9XY0();
            break;
        case 0xA:
            opcodeANNN();
            break;
        case 0xB:
            opcodeBNNN();
            break;
        case 0xC:
            opcodeCXNN();
            break;
        case 0xD:
            opcodeDXYN();
            break;
        case 0xE:
            opcodeE_group();
            break;
        case 0xF:
            opcodeF_group();
            break;
    }

return; 
}


void Chip8::run() {
    opcode = get_next_opcode();
    execute_op();
}

void Chip8::decrease_timers() {

    if (d_timer > 0)
        d_timer--;
    
    if (s_timer > 0)
        s_timer--;
return;
}

