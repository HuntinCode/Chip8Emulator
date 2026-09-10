#include "Chip8.h"
#include <cstdlib>
#include <iostream>

// resets the screen data.
void Chip8::opcode00E0() {
    clear_display();
return;
}

void Chip8::opcode00EE() {
    
    pc = stack[--sp];
    stack[sp] = 0;

return;
}

// jumps to a defined address.
void Chip8::opcode1NNN() {

    pc = NNN_address;

return;
}

// calls a function at the designated address.
void Chip8::opcode2NNN() {

    stack[sp++] = pc;
    pc = NNN_address;

return;
}

// compares the value in V[x] register, with NN. If, they're equal, then skip the next instruction otherwise proceed.
void Chip8::opcode3XNN() {
unsigned char byte = 0;

    byte = opcode & 0x00FF;

    if (v[X_register] == byte)
        pc += 2;

return;
}

// Compares the value of register X with NN. If they're not equal to each other it will skip the next instruction.
void Chip8::opcode4XNN() {
unsigned char byte = 0;

    byte = opcode & 0x00FF;
    
    if (v[X_register] != byte)
        pc += 2;

return;   
}

// Compares the value of register X and Y. If they're equal, it skips the next instruction.
void Chip8::opcode5XY0() {

    if (v[X_register] == v[Y_register])
        pc += 2;

return;
}

// Sets the value of vX register to the byte NN.
void Chip8::opcode6XNN() {
unsigned char byte = 0;

    byte = opcode & 0x00FF;
    v[X_register] = byte;
   
return;
}

// Adds the byte to the vX register without affecting the carry flag.
void Chip8::opcode7XNN() {
unsigned char xReg = 0;

   
    v[X_register] += (opcode & 0x00FF);
   
return;
}

// sets the value of vX register to vY register. 
void Chip8::opcode8XY0() {

    v[X_register] = v[Y_register];

return;
}

// Sets the value of vX to vX OR vY using bitwise operation.
void Chip8::opcode8XY1() {

    v[X_register] |= v[Y_register];
    v[0xF] = 0;

return;
}

// Sets the value of vX to vX and vY using bitwise operation.
void Chip8::opcode8XY2() {

    v[X_register] &= v[Y_register];
    v[0xF] = 0;
    
return;
}

// Sets vX register to value of vX XOR vY
void Chip8::opcode8XY3() {

    v[X_register] ^= v[Y_register];
    v[0xF] = 0;
    
return;
}

// adds Vy to Vx and if theres an overflow, sets the vF flag to 1. Set to 0 if no overflow.
void Chip8::opcode8XY4() {
int xVal = 0;
int yVal = 0;

    xVal = v[X_register];
    yVal = v[Y_register];

    v[X_register] += v[Y_register];
    v[0xF] = 0;

    if ((xVal + yVal) > 255) 
        v[0xF] = 1;

return;
}

// subtracts vY from Vx and if theres an underflow, sets the flag register to 0. Otherwise the flag register is 1.
void Chip8::opcode8XY5() {
int xVal = 0;
int yVal = 0;

    xVal = v[X_register];
    yVal = v[Y_register];

    v[X_register] -= v[Y_register];

    if (xVal >= yVal) 
        v[0xF] = 1;
    else
        v[0xF] = 0;
    

return;
}

// Shifts vX to the right by 1, but before that it stores the least significant bit of vX into vF
void Chip8::opcode8XY6() {
unsigned char lsb = 0;


    lsb = v[Y_register] & 1;
    v[X_register] = v[Y_register] >> 1;
    v[0xF] = lsb;

return;
}

// sets vX to Vy - Vx. VF is set to 0 when when theres a underflow.
void Chip8::opcode8XY7() {
int xVal = 0;
int yVal = 0;


    xVal = v[X_register];
    yVal = v[Y_register];
    
    v[X_register] = v[Y_register] - v[X_register];

    if (yVal >= xVal) 
        v[0xF] = 1;
    else
        v[0xF] = 0;
        
return;
}

void Chip8::opcode8XYE() {
unsigned char msb = 0;


    msb = (v[Y_register] & 0x80) >> 7; // 0x80 is the 8th bit in a byte e.g 10000000, works well here since 8 bit register.
    v[X_register] = v[Y_register] << 1;
    v[0xF] = (msb & 0x1);

return;
}

// compare value of vX and vY if it's not equal, skip the next instruction.
void Chip8::opcode9XY0() {

    if (v[X_register] != v[Y_register])
        pc += 2;
    
return; 
}

// sets address register to NNN
void Chip8::opcodeANNN() {

    I = NNN_address;
    
return;
}

// Jumps the address NNN + v[0]
void Chip8::opcodeBNNN() {

    pc = v[0] + NNN_address;

return;
}

// sets vX to result of a bitwise AND operation of a random number and NN
void Chip8::opcodeCXNN() {
unsigned char random = std::rand() % 256;

    v[X_register] = random & (opcode & 0x00FF);

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
    x_cord = v[X_register] % 64;
    y_cord = v[Y_register] % 32;

    v[0xF] = 0;

    for (int yline = 0; yline < height; yline++) {
        
        
        data = memory[I + yline];
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
                    v[0xF] = 1;
                
                screen_data[y][x] ^= 1;

            }

        }

    }

return;
}

// If the value of vX matches the pressed key, it skips the next instruction
void Chip8::opcodeEX9E() {
unsigned char lowestNibble = 0;

    lowestNibble = v[X_register] & 0x000F;
    
    if (key_states[lowestNibble])
        pc += 2;

return;
}

// If the value of vX doesn't match the pressed key, it skips the next instruction.
void Chip8::opcodeEXA1() {
unsigned char lowest_nibble = 0;

    lowest_nibble = v[X_register] & 0xF;
    

    if (!key_states[lowest_nibble])
        pc += 2;

return;
}

// Sets the vX to the value of the delay timer
void Chip8::opcodeFX07() {

    v[X_register] = d_timer;
   
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
        pc -= 2;

return;
}

// sets delay timer to vX
void Chip8::opcodeFX15() {

    d_timer = v[X_register];
    
return;
}

// sets sound timer to vX
void Chip8::opcodeFX18() {

    s_timer = v[X_register];
    
return;
}

// adds vX to I address.
void Chip8::opcodeFX1E() {

    I += v[X_register];
    
return;
}

// sets the address I to the location of sprite character vX.
void Chip8::opcodeFX29() {
unsigned char nibble = 0;

    nibble = v[X_register] & 0x000F;
    I = memory[nibble * 5];
 
return;
}

// stores the binary-coded representation of vX starting at memory location I
void Chip8::opcodeFX33() {
int hundreds = 0;
int tens = 0;
int ones = 0;
int value = 0;


    value = v[X_register];

    hundreds = value / 100;
    tens = (value / 10) % 10;
    ones = (value % 100) % 10;

    memory[I] = hundreds;
    memory[I + 1] = tens;
    memory[I + 2] = ones;

return;
}

// Stores values from v0 to vX to in memory, starting at addres I
void Chip8::opcodeFX55() {


    for (int i = 0; i <= X_register; i++) {
        memory[I + i] = v[i];
    }

    I += X_register + 1;


return;
}

// Fills v0 to vX with values from memory, starting at address I
void Chip8::opcodeFX65() {

    for (int i = 0; i <= X_register; i++)
        v[i] = memory[I + i];

    I += X_register + 1;

return;
}