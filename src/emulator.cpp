#include <iostream>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <string>

#include "SDL2/SDL.h"

#include "system.h"

unsigned char SDLKeys[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};


int main(int argc, char** argv){

    if(argc < 2){
        std::cerr << "No ROM specified!\nUsage:\t" << argv[0] << " <ROM path>" << std::endl;
        return 1;
    }

    CHIP8 emulator;

    emulator.initialise();

    SDL_Window   *window   = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture  *texture  = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cerr << "Couldn't initialise SDL!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    int windowHeight =  768;
    int windowWidth  = 1024;

    window = SDL_CreateWindow("CHIP8-Emulator",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                windowWidth, windowHeight,
                SDL_WINDOW_RESIZABLE);

    if (window == NULL){
        std::cerr << "Couldn't create Window!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    64, 32);

    if(texture == NULL){
        std::cerr << "Couldn't create texture!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    int timer = 0;
    uint32_t pixels[32 * 64];

    if (!emulator.loadROM(argv[1])){
        exit(1);
    }

    while(true){
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);

            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);

                for (int i = 0; i < 16; ++i){
                    if (event.key.keysym.sym == SDLKeys[i]){
                        emulator.key[i] = 1;
                    }
                }
            }

            if (event.type == SDL_KEYUP){
                for (int i = 0; i < 16; ++i){
                    if (event.key.keysym.sym == SDLKeys[i]){
                        emulator.key[i] = 0;
                    }
                }
            }
        }

        if (timer == 900){
            emulator.emulateCycle();
            timer = 0;
        }

        else {
            timer += 1;
        }
        if(emulator.drawFlag){
            for (int i = 0; i < (32 * 64); ++i){
                uint8_t pixel = emulator.graphics[i];
                pixels[i] = ((0x00FFFFFF * pixel) | 0xFF000000);

            }

            if (SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32)) < 0){
                std::cerr << "Couldn't update texture!\n" << SDL_GetError() << std::endl;
                exit(1);
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture,NULL,NULL);
            SDL_RenderPresent(renderer);

            emulator.drawFlag = false;
        }


    }

    return 0;
}
