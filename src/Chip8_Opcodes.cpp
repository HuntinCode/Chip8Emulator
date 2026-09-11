#include "Chip8.h"
#include <cstdlib>
#include <iostream>

// resets the screen data.
void Chip8::opcode00E0() {
    clear_display();
return;
}

void Chip8::opcode00EE() {
    
    program_counter = stack[--stack_pointer];
    stack[stack_pointer] = 0;

return;
}

// jumps to a defined address.
void Chip8::opcode1NNN() {

    program_counter = get_NNN();

return;
}

// calls a function at the designated address.
void Chip8::opcode2NNN() {

    stack[stack_pointer++] = program_counter;
    program_counter = get_NNN();

return;
}

// compares the value in v_register[x] register, with NN. If, they're equal, then skip the next instruction otherwise proceed.
void Chip8::opcode3XNN() {


    if (v_register[get_X()] == get_NN())
        program_counter += 2;

return;
}

// Compares the value of register X with NN. If they're not equal to each other it will skip the next instruction.
void Chip8::opcode4XNN() {

    
    if (v_register[get_X()] != get_NN())
        program_counter += 2;

return;   
}

// Compares the value of register X and Y. If they're equal, it skips the next instruction.
void Chip8::opcode5XY0() {

    if (v_register[get_X()] == v_register[get_Y()])
        program_counter += 2;

return;
}

// Sets the value of vX register to the byte NN.
void Chip8::opcode6XNN() {

    v_register[get_X()] = get_NN();
   
return;
}

// Adds the byte to the vX register without affecting the carry flag.
void Chip8::opcode7XNN() {
unsigned char xReg = 0;

    v_register[get_X()] += get_NN();
   
return;
}

// sets the value of vX register to vY register. 
void Chip8::opcode8XY0() {

    v_register[get_X()] = v_register[get_Y()];

return;
}

// Sets the value of vX to vX OR vY using bitwise operation.
void Chip8::opcode8XY1() {

    v_register[get_X()] |= v_register[get_Y()];
    v_register[0xF] = 0;

return;
}

// Sets the value of vX to vX and vY using bitwise operation.
void Chip8::opcode8XY2() {

    v_register[get_X()] &= v_register[get_Y()];
    v_register[0xF] = 0;
    
return;
}

// Sets vX register to value of vX XOR vY
void Chip8::opcode8XY3() {

    v_register[get_X()] ^= v_register[get_Y()];
    v_register[0xF] = 0;
    
return;
}

// adds Vy to Vx and if theres an overflow, sets the vF flag to 1. Set to 0 if no overflow.
void Chip8::opcode8XY4() {
int xVal = 0;
int yVal = 0;

    xVal = v_register[get_X()];
    yVal = v_register[get_Y()];

    v_register[get_X()] += v_register[get_Y()];
    v_register[0xF] = 0;

    if ((xVal + yVal) > 255) 
        v_register[0xF] = 1;

return;
}

// subtracts vY from Vx and if theres an underflow, sets the flag register to 0. Otherwise the flag register is 1.
void Chip8::opcode8XY5() {
int xVal = 0;
int yVal = 0;

    xVal = v_register[get_X()];
    yVal = v_register[get_Y()];

    v_register[get_X()] -= v_register[get_Y()];

    if (xVal >= yVal) 
        v_register[0xF] = 1;
    else
        v_register[0xF] = 0;
    

return;
}

// Shifts vX to the right by 1, but before that it stores the least significant bit of vX into vF
void Chip8::opcode8XY6() {
unsigned char lsb = 0;


    lsb = v_register[get_Y()] & 1;
    v_register[get_X()] = v_register[get_Y()] >> 1;
    v_register[0xF] = lsb;

return;
}

// sets vX to Vy - Vx. VF is set to 0 when when theres a underflow.
void Chip8::opcode8XY7() {
int xVal = 0;
int yVal = 0;


    xVal = v_register[get_X()];
    yVal = v_register[get_Y()];
    
    v_register[get_X()] = v_register[get_Y()] - v_register[get_X()];

    if (yVal >= xVal) 
        v_register[0xF] = 1;
    else
        v_register[0xF] = 0;
        
return;
}

void Chip8::opcode8XYE() {
unsigned char msb = 0;

    msb = (v_register[get_Y()] & 0x80) >> 7; // 0x80 is the 8th bit in a byte e.g 10000000, works well here since 8 bit register.
    v_register[get_X()] = v_register[get_Y()] << 1;
    v_register[0xF] = (msb & 0x1);

return;
}

// compare value of vX and vY if it's not equal, skip the next instruction.
void Chip8::opcode9XY0() {

    if (v_register[get_X()] != v_register[get_Y()])
        program_counter += 2;
    
return; 
}

// sets address register to NNN
void Chip8::opcodeANNN() {

    I_register = get_NNN();
    
return;
}

// Jumps the address NNN + v_register[0]
void Chip8::opcodeBNNN() {

    program_counter = v_register[0] + get_NNN();

return;
}

// sets vX to result of a bitwise AND operation of a random number and NN
void Chip8::opcodeCXNN() {
unsigned char random = std::rand() % 256;

    v_register[get_X()] = random & (opcode & 0x00FF);

return;
}

// Draws a sprite at a given coordinate X, Y, 8 pixels wide of a length N.
void Chip8::opcodeDXYN() {
unsigned char height = 0;
unsigned char x_cord  = 0;
unsigned char y_cord = 0;
unsigned char data = 0;
unsigned char x_pixel = 0;
unsigned char x_pixel_line = 0;
unsigned char mask = 0;
unsigned char x = 0;
unsigned char y = 0;

    height = 0x000F & opcode;
    x_cord = v_register[get_X()] % 64;
    y_cord = v_register[get_Y()] % 32;

    v_register[0xF] = 0;

    for (int yline = 0; yline < height; yline++) {
        
        
        data = memory[I_register + yline];
        x_pixel_line = 7;
        x_pixel = 0;

        for (x_pixel = 0; x_pixel < 8; x_pixel++, x_pixel_line--) {
            if (x_cord > 64 || y_cord > 32 )
                break;

            mask = 1 << x_pixel_line;
            if (data & mask) {
                
                 x = (x_cord + x_pixel) % 64;
                 y = (y_cord + yline) % 32;

                if (screen_data[y][x] == 1)
                    v_register[0xF] = 1;
                
                screen_data[y][x] ^= 1;

            }

        }

    }

return;
}

// If the value of vX matches the pressed key, it skips the next instruction
void Chip8::opcodeEX9E() {
unsigned char lowestNibble = 0;

    lowestNibble = v_register[get_X()] & 0xF;
    
    if (key_states[lowestNibble])
        program_counter += 2;

return;
}

// If the value of vX doesn't match the pressed key, it skips the next instruction.
void Chip8::opcodeEXA1() {
unsigned char lowest_nibble = 0;

    lowest_nibble = v_register[get_X()] & 0xF;
    
    if (!key_states[lowest_nibble])
        program_counter += 2;

return;
}

// Sets the vX to the value of the delay timer
void Chip8::opcodeFX07() {

    v_register[get_X()] = delay_timer;
   
return;
}

// waits for key press, haults all instructions until key press but allows sound, delay timers to continue.
void Chip8::opcodeFX0A() {
unsigned char pressedKey = 0;
bool key_not_pressed = true;

    pressedKey = get_pressed_key();
  
    if (pressedKey != 255 )
        key_not_pressed = false;

    if (key_not_pressed)
        program_counter -= 2;

return;
}

// sets delay timer to vX
void Chip8::opcodeFX15() {

    delay_timer = v_register[get_X()];
    
return;
}

// sets sound timer to vX
void Chip8::opcodeFX18() {

    sound_timer = v_register[get_X()];
    
return;
}

// adds vX to I_register address.
void Chip8::opcodeFX1E() {

    I_register += v_register[get_X()];
    
return;
}

// sets the address I_register to the location of sprite character vX.
void Chip8::opcodeFX29() {
unsigned char nibble = 0;

    nibble = v_register[get_X()] & 0x000F;
    I_register = memory[nibble * 5];
 
return;
}

// stores the binary-coded representation of vX starting at memory location I_register
void Chip8::opcodeFX33() {
int hundreds = 0;
int tens = 0;
int ones = 0;
int value = 0;


    value = v_register[get_X()];

    hundreds = value / 100;
    tens = (value / 10) % 10;
    ones = (value % 100) % 10;

    memory[I_register] = hundreds;
    memory[I_register + 1] = tens;
    memory[I_register + 2] = ones;

return;
}

// Stores values from v0 to vX to in memory, starting at addres I_register
void Chip8::opcodeFX55() {


    for (int i = 0; i <= get_X(); i++) {
        memory[I_register + i] = v_register[i];
    }

    I_register += get_X() + 1;


return;
}

// Fills v0 to vX with values from memory, starting at address I_register
void Chip8::opcodeFX65() {

    for (int i = 0; i <= get_X(); i++)
        v_register[i] = memory[I_register + i];

    I_register += get_X() + 1;

return;
}