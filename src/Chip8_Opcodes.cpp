#include "Chip8.h"
#include <cstdlib>

// jumps to a machine function.
void Chip8::opcode0NNN() {

    stack[sp++] = pc;
    pc = extract_address();

return;
}

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

    pc = extract_address();

return;
}

// calls a function at the designated address.
void Chip8::opcode2NNN() {
    
    stack[sp++] = pc;
    pc = extract_address();

return;
}

// compares the value in V[x] register, with NN. If, they're equal, then skip the next instruction otherwise proceed.
void Chip8::opcode3XNN() {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = (opcode & 0x0F00) >> 8;

    if (v[xReg] == byte)
        pc += 2;

return;
}

// Compares the value of register X with NN. If they're not equal to each other it will skip the next instruction.
void Chip8::opcode4XNN() {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = (opcode & 0x0F00) >> 8;

    if (v[xReg] != byte)
        pc += 2;

return;   
}

// Compares the value of register X and Y. If they're equal, it skips the next instruction.
void Chip8::opcode5XY0() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    if (v[reg.x] == v[reg.y])
        pc += 2;

return;
}

// Sets the value of vX register to the byte NN.
void Chip8::opcode6XNN() {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = (opcode & 0x0F00) >> 8;
    v[xReg] = byte;
   
return;
}

// Adds the byte to the vX register without affecting the carry flag.
void Chip8::opcode7XNN() {
unsigned char xReg = 0;

    xReg = (opcode & 0x0F00) >> 8;
    v[xReg] += (opcode & 0x00FF);
   
return;
}

// sets the value of vX register to vY register. 
void Chip8::opcode8XY0() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] = v[reg.y];

return;
}

// Sets the value of vX to vX OR vY using bitwise operation.
void Chip8::opcode8XY1() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] |= v[reg.y];

return;
}

// Sets the value of vX to vX and vY using bitwise operation.
void Chip8::opcode8XY2() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] &= v[reg.y];
    
return;
}

// Sets vX register to value of vX XOR vY
void Chip8::opcode8XY3() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] ^= v[reg.y];
    
return;
}

// adds Vy to Vx and if theres an overflow, sets the vF flag to 1. Set to 0 if no overflow.
void Chip8::opcode8XY4() {
RegisterIdentifiers reg;
int xVal = 0;
int yVal = 0;

    reg = extract_reg_identifer();
    xVal = v[reg.x];
    yVal = v[reg.y];

    v[0xf] = 0;

    if ((xVal + yVal) > 255) 
        v[0xf] = 1;

    v[reg.x] += v[reg.y];

return;
}

// subtracts vY from Vx and if theres an underflow, sets the flag register to 0. Otherwise the flag register is 1.
void Chip8::opcode8XY5() {
RegisterIdentifiers reg;
int xVal = 0;
int yVal = 0;

    reg = extract_reg_identifer();
    xVal = reg.x;
    yVal = reg.y;

    v[0xf] = 1;

    if (v[reg.y] > v[reg.x]) 
        v[0xf] = 0;
    
    v[reg.x] -= v[reg.y];

return;
}

// Shifts vX to the right by 1, but before that it stores the least significant bit of vX into vF
void Chip8::opcode8XY6() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[0xF] = v[reg.x] & 0x000F;
    v[reg.x] >>= 1;

return;
}

// sets vX to Vy - Vx. VF is set to 0 when when theres a underflow.
void Chip8::opcode8XY7() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[0xf] = 1;

    if (v[reg.y] >= v[reg.x]) 
        v[reg.x] = 0;

    v[reg.x] = v[reg.y] - v[reg.x];

return;
}

void Chip8::opcode8XYE() {
RegisterIdentifiers reg;
unsigned char temp = 0;

    reg = extract_reg_identifer();
    temp = (v[reg.x] & 0xF000) >> 12;
    v[reg.x] <<= 1;

    if (temp)
        v[0xF] = 1;
    else
        v[0xF] = 0;
  
return;
}

// compare value of vX and vY if it's not equal, skip the next instruction.
void Chip8::opcode9XY0() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    if (v[reg.x] != v[reg.y])
        pc += 2;
    
return; 
}

// sets address register to NNN
void Chip8::opcodeANNN() {

    I = extract_address();
    
return;
}

// Jumps the address NNN + v[0]
void Chip8::opcodeBNNN() {
unsigned short address = 0;
    
    address = extract_address();
    pc = v[0] + address;

return;
}

// sets vX to result of a bitwise AND operation of a random number and NN
void Chip8::opcodeCXNN() {
unsigned char random = std::rand() % 256;
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] = random & (opcode & 0x00FF);

return;
}

// Draws a sprite at a given coordinate X, Y, 8 pixels wide of a length N.
void Chip8::opcodeDXYN() {
RegisterIdentifiers reg;
unsigned char height = 0;
unsigned char x_cord  = 0;
unsigned char y_cord = 0;
unsigned char data = 0;
unsigned char x_pixel = 0;
unsigned char x_pixel_line = 0;
unsigned char mask = 0;
unsigned char x = 0;
unsigned char y = 0;

    reg = extract_reg_identifer();
    x_cord = v[reg.x];
    y_cord = v[reg.y];
    height = 0x000F & opcode;

    v[0xF] = 0;

    for (int yline = 0; yline < height; y++) {
        
        data = memory[I + yline];
        x_pixel_line = 7;
        x_pixel = 0;

        for (x_pixel = 0; x_pixel < 8; x_pixel++, x_pixel_line--) {

            mask = 1 << x_pixel_line;
            if (data & mask) {

                x = x_cord + x_pixel;
                y = y_cord + yline;

                if (screen_data[x][y] == 1)
                    v[0xF] = 1;
                
                screen_data[x][y] ^= 1;

            }

        }

    }

return;
}


// If the value of vX matches the pressed key, it skips the next instruction
void Chip8::opcodeEX9E() {
unsigned char key = 0;
unsigned char xReg = 0;

    key = get_pressed_key();
    xReg = (opcode & 0x0F00) >> 8;

    if (key == v[xReg])
        pc += 2;

return;
}

// If the value of vX doesn't match the pressed key, it skips the next instruction.
void Chip8::opcodeEXA1() {
unsigned char key = 0;
unsigned char xReg = 0;
    
    key = get_pressed_key();
    xReg = (opcode & 0x0F00) >> 8;

    if (key != v[xReg])
        pc += 2;

return;
}

// Sets the vX to the value of the delay timer
void Chip8::opcodeFX07() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    v[reg.x] = d_timer;
   
return;
}

// waits for key press, haults all instructions until key press but allows sound, delay timers to continue.
void Chip8::opcodeFX0A() {
unsigned key = 0;
unsigned xReg = 0;
    
    xReg = (opcode & 0x0F00) >> 8;
    key = get_pressed_key();

    if (key)
        v[xReg] = key;
    else
        pc -= 2; // repeats this instruction until key pressed.

return;
}

// sets delay timer to vX
void Chip8::opcodeFX15() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    d_timer = v[reg.x];
    
return;
}

// sets sound timer to vX
void Chip8::opcodeFX18() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    s_timer = v[reg.x];
    
return;
}

// adds vX to I address.
void Chip8::opcodeFX1E() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();
    I += v[reg.x];
    
return;
}

// sets the address I to the location of sprite character vX.
void Chip8::opcodeFX29() {
RegisterIdentifiers reg;
unsigned char nibble = 0;

    reg = extract_reg_identifer();
    nibble = v[reg.x] & 0x000F;
    I = memory[nibble * 5];
 
return;
}

// stores the binary-coded representation of vX starting at memory location I
void Chip8::opcodeFX33() {
RegisterIdentifiers reg;
int hundreds = 0;
int tens = 0;
int ones = 0;
int value = 0;

    reg = extract_reg_identifer();

    value = v[reg.x];

    hundreds = value / 100;
    tens = (value / 10) % 10;
    ones = value % 10;

    memory[I] = hundreds;
    memory[I + 1] = tens;
    memory[I + 2] = ones;

return;
}

// Stores values from v0 to vX to in memory, starting at addres I
void Chip8::opcodeFX55() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();

    for (int i = 0; i <= reg.x; i++) {
        memory[I + i] = v[i];
    }

    I += reg.x + 1;

return;
}

// Fills v0 to vX with values from memory, starting at address I
void Chip8::opcodeFX65() {
RegisterIdentifiers reg;

    reg = extract_reg_identifer();

    for (int i = 0; i <= reg.x; i++)
        v[i] = memory[I + i];

    I += reg.x + 1;

return;
}