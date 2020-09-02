#include <iostream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "SDL2/SDL.h"

#include "system.h"

#define WINDOW_HEIGHT  768
#define WINDOW_WIDTH  1024

#define GRAPHICS_HEIGHT  32
#define GRAPHICS_WIDTH   64

#define CLOCK_DELAYER   0
#define TOTAL_KEYS     16

#define FPS_CAP        60
unsigned char SDLKeys[TOTAL_KEYS] = {
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

    if (!emulator.loadROM(argv[1])){
        exit(1);
    }

    SDL_Window   *window   = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture  *texture  = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cerr << "Couldn't initialise SDL!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("CHIP8-Emulator",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_RESIZABLE);

    if (window == NULL){
        std::cerr << "Couldn't create Window!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL){
        std::cerr << "Couldn't create Renderer!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    texture = SDL_CreateTexture(renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    GRAPHICS_WIDTH, GRAPHICS_HEIGHT);

    if(texture == NULL){
        std::cerr << "Couldn't create texture!\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    unsigned int pixels[GRAPHICS_HEIGHT * GRAPHICS_WIDTH];
    int timer = 0;
    long start = SDL_GetTicks();
    long start_per_frame = SDL_GetTicks();
    long countedFrames = 0;

    while(true){

        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);

            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    exit(0);

                for (int i = 0; i < TOTAL_KEYS; ++i){
                    if (event.key.keysym.sym == SDLKeys[i]){
                        emulator.key[i] = 1;
                    }
                }
            }

            if (event.type == SDL_KEYUP){
                for (int i = 0; i < TOTAL_KEYS; ++i){
                    if (event.key.keysym.sym == SDLKeys[i]){
                        emulator.key[i] = 0;
                    }
                }
            }
        }

        if (timer == CLOCK_DELAYER){
            emulator.emulateCycle();
            timer = 0;
        }

        else {
            timer += 1;
        }


        int pitch;
        void* m_pixels;


        if (emulator.drawFlag){
            SDL_RenderClear(renderer);
            if(SDL_LockTexture(texture, NULL, (void **)&m_pixels, &pitch) < 0){
                std::cerr << "Couldn't lock texture!\n" << SDL_GetError() << std::endl;
            }
            for (int i = 0; i < (GRAPHICS_HEIGHT * GRAPHICS_WIDTH); ++i){
                pixels[i] = ((0x00FFFFFF *  emulator.graphics[i]) | 0xFF000000);
            }
            memcpy(m_pixels, pixels, GRAPHICS_HEIGHT*pitch);
            SDL_UnlockTexture(texture);
            // if (SDL_UpdateTexture(texture, NULL, pixels, GRAPHICS_WIDTH * sizeof(unsigned int)) < 0){
            //     std::cerr << "Couldn't update texture!\n" << SDL_GetError() << std::endl;
            //     exit(1);
            // }


            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture,NULL,NULL);
            SDL_RenderPresent(renderer);


            emulator.drawFlag = false;

            if((1000 / FPS_CAP) > (SDL_GetTicks() - start_per_frame)){
                SDL_Delay( (1000 / FPS_CAP) - (SDL_GetTicks() - start_per_frame));
            }
            start_per_frame = SDL_GetTicks();
            float avgFPS = countedFrames / ( (SDL_GetTicks() - start) / 1000.f );
			if( avgFPS > 2000000 ){
				avgFPS = 0;
			}
            ++countedFrames;

            std::cerr << "Avg. FPS: " << avgFPS << "\r";
        }
        if (emulator.beepFlag){
            //std::cerr << "Beep!!" << std::endl;
            //TODO

            emulator.beepFlag = false;
        }

    }

    return 0;
}
