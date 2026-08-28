#ifndef CHIP8_CPU
#define CHIP8_CPU

#include <iosfwd>


const unsigned short MAX_MEMORY_SIZE = 0x1000;
const unsigned char SCREEN_WIDTH = 64;
const unsigned char SCREEN_LENGTH = 32;

typedef void (*instruction)();

struct RegisterIdentifiers {
    unsigned char x;
    unsigned char y;
};



class Chip8 {

private:

    instruction jumpTable[0xf][0xf];

    unsigned char v[16]; // registers
    unsigned char memory[MAX_MEMORY_SIZE]; // memory;
    unsigned short I; // address pointer register.

    unsigned char pc; // program counter
    unsigned char sp; // stack pointer

    unsigned char d_timer; // delay timer
    unsigned char s_timer; // sound timer

    unsigned short opcode;
    unsigned short stack[24];
    

    unsigned short screen_data[SCREEN_WIDTH][SCREEN_WIDTH];



public:

    Chip8();

    unsigned short get_next_opcode();
    void load_rom(std::string);
    void reset();
    void clear_display();
    void clear_mem();
    void clear_stack();

    unsigned short extract_address(unsigned short);
    RegisterIdentifiers extract_reg_identifer(unsigned short);


    void opcode0NNN(unsigned short);
    void opcode00E0(unsigned short);
    void opcode00EE(unsigned short);
    void opcode1NNN(unsigned short);
    void opcode2NNN(unsigned short);
    void opcode3XNN(unsigned short);
    void opcode4XNN(unsigned short);
    void opcode5XY0(unsigned short);
    void opcode6XNN(unsigned short);
    void opcode7XNN(unsigned short);
    void opcode8XY0(unsigned short);
    void opcode8XY1(unsigned short);
    void opcode8XY2(unsigned short);
    void opcode8XY3(unsigned short);
    void opcode8XY4(unsigned short);
    void opcode8XY5(unsigned short);
    void opcode8XY6(unsigned short);
    void opcode8XY7(unsigned short);
    void opcode8XYE(unsigned short);
    void opcode9XY0(unsigned short);
    void opcodeANNN(unsigned short);
    void opcodeBNNN(unsigned short);
    void opcodeCXNN(unsigned short);
    void opcodeDXYN(unsigned short);
    void opcodeEX9E(unsigned short);
    void opcodeEXA1(unsigned short);
    void opcodeFX07(unsigned short);
    void opcodeFX0A(unsigned short);
    void opcodeFX15(unsigned short);
    void opcodeFX18(unsigned short);
    void opcodeFX1E(unsigned short);
    void opcodeFX29(unsigned short);
    void opcodeFX33(unsigned short);
    void opcodeFX55(unsigned short);
    void opcodeFX65(unsigned short);

};
     


#endif