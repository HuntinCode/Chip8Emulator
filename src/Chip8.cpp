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
        memory[i] = 0;

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
    extraction.x >>= 8;

    extraction.y = opcode & 0x00F0;
    extraction.y >>= 4;

return extraction;
}

// jumps to a machine function.
void Chip8::opcode0NNN(unsigned short opcode) {
unsigned short address = 0;

    stack[sp] = pc;
    address = extract_address(opcode);
    pc = address;
    sp++;

return;
}

// resets the screen data.
void Chip8::opcode00E0(unsigned short opcode) {
    clear_display();
return;
}

void Chip8::opcode00EE(unsigned short opcode) {
    
    pc = stack[sp];
    stack[sp] = 0;
    sp--;

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
    xReg = (opcode & 0x0F00) >> 8;

    if (v[xReg] == byte)
        pc += 2;

return;
}

// Compares the value of register X with NN. If they're not equal to each other it will skip the next instruction.
void Chip8::opcode4XNN(unsigned short opcode) {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = (opcode & 0x0F00) >> 8;

    if (v[xReg] != byte)
        pc += 2;

return;   
}

// Compares the value of register X and Y. If they're equal, it skips the next instruction.
void Chip8::opcode5XY0(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    if (v[reg.x] == v[reg.y])
        pc += 2;

return;
}

// Sets the value of vX register to the byte NN.
void Chip8::opcode6XNN(unsigned short opcode) {
unsigned char byte = 0;
unsigned char xReg = 0;

    byte = opcode & 0x00FF;
    xReg = (opcode & 0x0F00) >> 8;
    v[xReg] = byte;
   
return;
}

// Adds the byte to the vX register without affecting the carry flag.
void Chip8::opcode7XNN(unsigned short opcode) {
unsigned char xReg = 0;

    xReg = (opcode & 0x0F00) >> 8;
    v[xReg] += (opcode & 0x00FF);
   
return;
}

// sets the value of vX register to vY register. 
void Chip8::opcode8XY0(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] = v[reg.y];

return;
}

// Sets the value of vX to vX OR vY using bitwise operation.
void Chip8::opcode8XY1(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] |= v[reg.y];

return;
}

// Sets the value of vX to vX and vY using bitwise operation.
void Chip8::opcode8XY2(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] &= v[reg.y];
    
return;
}

// Sets vX register to value of vX XOR vY
void Chip8::opcode8XY3(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] ^= v[reg.y];
    
return;
}

// adds Vy to Vx and if theres an overflow, sets the vF flag to 1. Set to 0 if no overflow.
void Chip8::opcode8XY4(unsigned short opcode) {
RegisterIdentifiers reg;
int xVal = 0;
int yVal = 0;

    reg = extract_reg_identifer(opcode);
    xVal = reg.x;
    yVal = reg.y;

    v[0xf] = 0;

    if ((xVal + yVal) > 255) 
        v[0xf] = 1;

    v[reg.x] += v[reg.y];

return;
}

// subtracts vY from Vx and if theres an underflow, sets the flag register to 0. Otherwise the flag register is 1.
void Chip8::opcode8XY5(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[0xf] = 1;

    if (v[reg.x] >= v[reg.y]) 
        v[0xf] = 0;
    
    v[reg.x] -= v[reg.y];


return;
}

// Shifts vX to the right by 1, but before that it stores the least significant bit of vX into vF
void Chip8::opcode8XY6(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[0xF] = v[reg.x] & 0x000F;
    v[reg.x] >>= 1;

return;
}

// sets vX to Vy - Vx. VF is set to 0 when when theres a underflow.
void Chip8::opcode8XY7(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[0xf] = 1;

    if (v[reg.y] >= v[reg.x]) 
        v[reg.x] = 0;

    v[reg.x] = v[reg.y] - v[reg.x];

return;
}

void Chip8::opcode8XYE(unsigned short opcode) {
RegisterIdentifiers reg;
unsigned char temp = 0;

    reg = extract_reg_identifer(opcode);
    temp = v[reg.x] & 0xF000;
    v[reg.x] <<= 1;

    if (temp)
        v[0xF] = 1;
    else
        v[0xF] = 0;
  
return;
}

// compare value of vX and vY if it's not equal, skip the next instruction.
void Chip8::opcode9XY0(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    if (v[reg.x] != v[reg.y])
        pc += 2;
    

return; 
}

// sets address register to NNN
void Chip8::opcodeANNN(unsigned short opcode) {
unsigned short address = 0;

    address = extract_address(opcode);
    I = address;
    

return;
}

// Jumps the address NNN + v[0]
void Chip8::opcodeBNNN(unsigned short opcode) {
unsigned short address = 0;
    
    address = extract_address(opcode);
    pc = v[0] + address;

return;
}

// sets vX to result of a bitwise AND operation of a random number and NN
void Chip8::opcodeCXNN(unsigned short opcode) {
unsigned char random = std::rand() % 256;
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] = random & (opcode & 0x00FF);

return;
}

// Draws a sprite at a given coordinate X, Y, 8 pixels wide of a length N.
void Chip8::opcodeDXYN(unsigned short opcode) {
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

    reg = extract_reg_identifer(opcode);
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


// has to do with key presses. implement later.
void Chip8::opcodeEX9E(unsigned short opcode) {
    
}

// implement later has to do with key presses.
void Chip8::opcodeEXA1(unsigned short opcode) {
    
}

// Sets the vX to the value of the delay timer
void Chip8::opcodeFX07(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    v[reg.x] = d_timer;
   
return;
}

// waits for key press, implement later.
void Chip8::opcodeFX0A(unsigned short opcode) {

}

// sets delay timer to vX
void Chip8::opcodeFX15(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    d_timer = v[reg.x];
    
return;
}

// sets sound timer to vX
void Chip8::opcodeFX18(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    s_timer = v[reg.x];
    
return;
}

// adds vX to I address.
void Chip8::opcodeFX1E(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);
    I += v[reg.x];
    
return;
}

// sets the address I to the location of sprite character vX.
void Chip8::opcodeFX29(unsigned short opcode) {
RegisterIdentifiers reg;
unsigned char nibble = 0;

    reg = extract_reg_identifer(opcode);
    nibble = v[reg.x] & 0x000F;
    I = nibble;
 
return;
}

// stores the binary-coded representation of vX starting at memory location I
void Chip8::opcodeFX33(unsigned short opcode) {
RegisterIdentifiers reg;
int hundreds = 0;
int tens = 0;
int ones = 0;
int value = 0;

    reg = extract_reg_identifer(opcode);

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
void Chip8::opcodeFX55(unsigned short opcode) {
RegisterIdentifiers reg;


    reg = extract_reg_identifer(opcode);

    for (int i = 0; i <= reg.x; i++) {
        memory[I + i] = v[i];
    }

return;
}

// Fills v0 to vX with values from memory, starting at address I
void Chip8::opcodeFX65(unsigned short opcode) {
RegisterIdentifiers reg;

    reg = extract_reg_identifer(opcode);

    for (int i = 0; i <= reg.x; i++)
        v[i] = memory[I + i];

return;
}