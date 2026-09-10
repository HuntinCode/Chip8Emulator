#ifndef CHIP8_CPU
#define CHIP8_CPU

#include <iosfwd>

const unsigned short MAX_MEMORY_SIZE = 0x1000;
const unsigned char STACK_SIZE = 24;
const unsigned short SCREEN_WIDTH = 64;
const unsigned short SCREEN_LENGTH = 32;
const unsigned char MAX_REGISTERS = 16;



class Chip8
{

private:
    unsigned char v[MAX_REGISTERS];        // registers
    unsigned char memory[MAX_MEMORY_SIZE]; // memory;
    unsigned short I;                      // address pointer register.

    unsigned short NNN_address;
    unsigned char X_register;
    unsigned char Y_register;

    unsigned short pc; // program counter
    unsigned char sp;  // stack pointer

    unsigned char d_timer; // delay timer
    unsigned char s_timer; // sound timer

    unsigned short opcode;
    unsigned short stack[STACK_SIZE];

    bool key_states[0x10] = {false};

    unsigned char chip8_fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

    
    void get_next_opcode();
    void reset();
    void clear_display();
    void clear_mem();
    void clear_stack();
    void clear_keystates();
    void load_font_set();
    
    void opcode0_group();
    void opcode8_group();
    void opcodeE_group();
    void opcodeF_group();

  
    void execute_op();
    
    void opcode0NNN();
    void opcode00E0();
    void opcode2NNN();
    void opcode00EE();
    void opcode1NNN();
    void opcode3XNN();
    void opcode4XNN();
    void opcode5XY0();
    void opcode6XNN();
    void opcode7XNN();
    void opcode8XY0();
    void opcode8XY1();
    void opcode8XY2();
    void opcode8XY3();
    void opcode8XY4();
    void opcode8XY5();
    void opcode8XY6();
    void opcode8XY7();
    void opcode8XYE();
    void opcode9XY0();
    void opcodeANNN();
    void opcodeBNNN();
    void opcodeCXNN();
    void opcodeDXYN();
    void opcodeEX9E();
    void opcodeEXA1();
    void opcodeFX07();
    void opcodeFX0A();
    void opcodeFX15();
    void opcodeFX18();
    void opcodeFX1E();
    void opcodeFX29();
    void opcodeFX33();
    void opcodeFX55();
    void opcodeFX65();

public:
    
    unsigned char screen_data[SCREEN_LENGTH][SCREEN_WIDTH];
    
    Chip8();
    
    unsigned char get_pressed_key();
    
    void set_key(unsigned char, bool);
    void run();
    void decrease_timers();
    void load_rom(std::string);
    
};

#endif