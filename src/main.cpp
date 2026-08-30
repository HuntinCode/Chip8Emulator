#include "Chip8.h"
#include <SDL3/SDL.h>
#include <string>

void handle_key_input(Chip8 &, SDL_Event);

void handle_key_input(Chip8 &cpu, SDL_Event e) {
    if (e.type == SDL_EVENT_KEY_DOWN)
        switch (e.key.key) {
            case SDLK_1: cpu.set_key(0, true); break;
            case SDLK_2: cpu.set_key(1, true); break;
            case SDLK_3: cpu.set_key(2, true); break;
            case SDLK_4: cpu.set_key(4, true); break;
            case SDLK_Q: cpu.set_key(5, true); break;
            case SDLK_W: cpu.set_key(6, true); break;
            case SDLK_E: cpu.set_key(6, true); break;
            case SDLK_R: cpu.set_key(7, true); break;
            case SDLK_A: cpu.set_key(8, true); break;
            case SDLK_S: cpu.set_key(9, true); break;
            case SDLK_D: cpu.set_key(0xA, true); break;
            case SDLK_F: cpu.set_key(0xB, true); break;
            case SDLK_Z: cpu.set_key(0xC, true); break;
            case SDLK_X: cpu.set_key(0xD, true); break;
            case SDLK_C: cpu.set_key(0xE, true); break;
            case SDLK_V: cpu.set_key(0xF, true); break;
            default: break;
        }

    if (e.type == SDL_EVENT_KEY_UP)
        switch (e.key.key) {
            case SDLK_1: cpu.set_key(0, false); break;
            case SDLK_2: cpu.set_key(1, false); break;
            case SDLK_3: cpu.set_key(2, false); break;
            case SDLK_4: cpu.set_key(4, false); break;
            case SDLK_Q: cpu.set_key(5, false); break;
            case SDLK_W: cpu.set_key(6, false); break;
            case SDLK_E: cpu.set_key(6, false); break;
            case SDLK_R: cpu.set_key(7, false); break;
            case SDLK_A: cpu.set_key(8, false); break;
            case SDLK_S: cpu.set_key(9, false); break;
            case SDLK_D: cpu.set_key(0xA, false); break;
            case SDLK_F: cpu.set_key(0xB, false); break;
            case SDLK_Z: cpu.set_key(0xC, false); break;
            case SDLK_X: cpu.set_key(0xD, false); break;
}


int main (int argc, char *argv[]) {
Chip8 *chip = nullptr;
SDL_WINDOW

    chip = new Chip8();
    chip->load_rom("/home/mrzombie/Projects/Chip8Emu/roms");

    delete chip;


return 0;
}