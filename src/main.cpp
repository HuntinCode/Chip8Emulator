#include "Chip8.h"
#include <SDL3/SDL.h>
#include <string>
#include <iostream>

void handle_key_input(Chip8 *, SDL_Event*);
void draw_frame(Chip8 *, SDL_Renderer *);
void emulator_loop(Chip8 *, SDL_Renderer*, SDL_Window*);

void draw_frame(Chip8 *cpu, SDL_Renderer *renderer) {
unsigned char scaler = 10;
SDL_FRect rect;

    rect.w = scaler / 2;
    rect.h = scaler / 2;

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int y = 0; y < SCREEN_LENGTH; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
             if (cpu->screen_data[y][x] == 1) {
                rect.x = x * scaler;
                rect.y = y * scaler;
                SDL_RenderFillRect(renderer, &rect);
             
            }
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

return;
}

void handle_key_input(Chip8 *cpu, SDL_Event *e) {

    bool isPressed = (e->type == SDL_EVENT_KEY_DOWN);
    switch (e->key.scancode) {

        case SDL_SCANCODE_1: cpu->set_key(0x1, isPressed); break;
        case SDL_SCANCODE_2: cpu->set_key(0x2, isPressed); break;
        case SDL_SCANCODE_3: cpu->set_key(0x3, isPressed); break;
        case SDL_SCANCODE_4: cpu->set_key(0xC, isPressed); break;
        
        case SDL_SCANCODE_Q: cpu->set_key(0x4, isPressed); break;
        case SDL_SCANCODE_W: cpu->set_key(0x5, isPressed); break;
        case SDL_SCANCODE_E: cpu->set_key(0x6, isPressed); break;
        case SDL_SCANCODE_R: cpu->set_key(0xD, isPressed); break;
        
        case SDL_SCANCODE_A: cpu->set_key(0x7, isPressed); break;
        case SDL_SCANCODE_S: cpu->set_key(0x8, isPressed); break;
        case SDL_SCANCODE_D: cpu->set_key(0x9, isPressed); break;
        case SDL_SCANCODE_F: cpu->set_key(0xE, isPressed); break;
        
        case SDL_SCANCODE_Z: cpu->set_key(0xA, isPressed); break;
        case SDL_SCANCODE_X: cpu->set_key(0x0, isPressed); break;
        case SDL_SCANCODE_C: cpu->set_key(0xB, isPressed); break;
        case SDL_SCANCODE_V: cpu->set_key(0xF, isPressed); break;
        
        default:  break;
       
    }
    

return;
}

void emulator_loop(Chip8 *cpu, SDL_Renderer *render, SDL_Window *window) {
bool running = true;

    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);

    while (running) {
       SDL_Event event;
       Uint64 time_elapsed = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
           switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                    
                case SDL_EVENT_KEY_DOWN:
                    SDL_Log("Key pressed! Code: %d | name: %s", event.key.key, SDL_GetKeyName(event.key.key));
                    handle_key_input(cpu, &event);
                    break; 

                case SDL_EVENT_KEY_UP:
                    handle_key_input(cpu, &event);
                    break;
                }
        }

        for (int i = 0; i < 11; i++) {
            cpu->run();
        }

        cpu->decrease_timers();
        draw_frame(cpu, render);


        SDL_Delay(16);
    }
    
return;
}


int main (int argc, char *argv[]) {
Chip8 *chip = nullptr;
SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;

    chip = new Chip8;
    chip->load_rom("roms/octojam1title.ch8");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error in initalization: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("Chip8Emu", 640, 320, SDL_WINDOW_INPUT_FOCUS, &window, &render)) {
        SDL_Log("Error in creating window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    emulator_loop(chip, render, window);

    delete chip;
    chip = nullptr;

    SDL_DestroyRenderer(render);
    render = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    
return 0;
}
