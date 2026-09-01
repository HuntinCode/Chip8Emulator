#include "Chip8.h"
#include <SDL3/SDL.h>
#include <string>
#include <iostream>

void handle_key_input(Chip8 *, SDL_Event*);
void draw_frame(Chip8 *, SDL_Renderer *);

void draw_frame(Chip8 *cpu, SDL_Renderer *renderer) {
bool success = 0;
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
   
    if (e->type == SDL_EVENT_KEY_DOWN)
        switch (e->key.key) {
            case SDLK_1: cpu->set_key(0, true); break;
            case SDLK_2: cpu->set_key(1, true); break;
            case SDLK_3: cpu->set_key(2, true); break;
            case SDLK_4: cpu->set_key(4, true); break;
            case SDLK_Q: cpu->set_key(5, true); break;
            case SDLK_W: cpu->set_key(6, true); break;
            case SDLK_E: cpu->set_key(6, true); break;
            case SDLK_R: cpu->set_key(7, true); break;
            case SDLK_A: cpu->set_key(8, true); break;
            case SDLK_S: cpu->set_key(9, true); break;
            case SDLK_D: cpu->set_key(0xA, true); break;
            case SDLK_F: cpu->set_key(0xB, true); break;
            case SDLK_Z: cpu->set_key(0xC, true); break;
            case SDLK_X: cpu->set_key(0xD, true); break;
            case SDLK_C: cpu->set_key(0xE, true); break;
            case SDLK_V: cpu->set_key(0xF, true); break;
            default: break;
        }

    if (e->type == SDL_EVENT_KEY_UP)
        switch (e->key.key) {
            case SDLK_1: cpu->set_key(0, false); break;
            case SDLK_2: cpu->set_key(1, false); break;
            case SDLK_3: cpu->set_key(2, false); break;
            case SDLK_4: cpu->set_key(4, false); break;
            case SDLK_Q: cpu->set_key(5, false); break;
            case SDLK_W: cpu->set_key(6, false); break;
            case SDLK_E: cpu->set_key(6, false); break;
            case SDLK_R: cpu->set_key(7, false); break;
            case SDLK_A: cpu->set_key(8, false); break;
            case SDLK_S: cpu->set_key(9, false); break;
            case SDLK_D: cpu->set_key(0xA, false); break;
            case SDLK_F: cpu->set_key(0xB, false); break;
            case SDLK_Z: cpu->set_key(0xC, false); break;
            case SDLK_X: cpu->set_key(0xD, false); break;
            default: break;
        }
return;
}


int main (int argc, char *argv[]) {
bool is_running = true;
Chip8 *chip = nullptr;
SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;
SDL_Event event;
Uint64 time_elapased = 0;
float frame_delay = 1000 / 60;
int frame_time = 0;
int number_opcodes = 10;


    chip = new Chip8;
    chip->load_rom("roms/3-corax+.ch8");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error in initalization: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("Chip8Emu", 640, 320, 0, &window, &render)) {
        SDL_Log("Error in creating window and renderer: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);

    while (is_running) {

            time_elapased = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT)
                    is_running = false;
            }
            
  
            frame_time = SDL_GetTicks() - time_elapased;
            if (frame_delay > frame_time) {
                 for (int i = 0; i < number_opcodes; i++) {}
                        chip->run();

                    draw_frame(chip, render);
                    SDL_Delay(frame_delay - frame_time);
                }

    }


    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete chip;
    
return 0;
}
