#include "../inc/video.hpp"
#include "../inc/memory.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <queue>

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

//Important addresses
const int LY = 0xFF44; //LCDC Y-Coordinate register
const int LCDC = 0xFF40; //LCDC register
const int LCDC_STATUS = 0xFF41; //LCDC status register

int dotCounter = 0;

SDL_Window* w = NULL;
SDL_Surface* sf = NULL;

struct pixel {
    int Color;
    int palette;
    //int spritePrio;
    int bgPrio;
};

struct sprite {
    uint8_t y;
    uint8_t x;
    uint8_t tileN;
    uint8_t attFlag;
};

std::queue<pixel> spriteQ;
std::queue<pixel> bgQueue;

void close();

void init_video () {
    SDL_SetMainReady();
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not intialize! SDL_Error: %s\n",SDL_GetError());
    } else {
        w = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if (w == NULL) {
            printf("Windows Could not be created! SDL_Error: %s\n",SDL_GetError());
        } else {
            sf = SDL_GetWindowSurface(w);
            //SDL_FillRect( sf, NULL, SDL_MapRGB(sf->format, 0xFF, 0xFF,0xFF));
            SDL_UpdateWindowSurface(w);
        }


    }
    //SDL_DestroyWindow(w);
    //SDL_Quit();
}

int lcdController(int cycles) {
    int cycles2 = 0;
    int ly = 0;
    int bgX = 0;
    int bgY = 0;
    int wX = 0;
    int wY = 0;
    int tileMapSelect = 0;
    int mode3Len = 0;
    int lcdc = read_byte(LCDC);
    int lcdcStatus = read_byte(LCDC_STATUS);
    cycles = cycles * 4;
    ly = read_byte(LY);

    for (int i = 0; i < cycles; i++) {
        sf = SDL_GetWindowSurface(w);
        dotCounter++;
        
        if (lcdc & 0x80) {
            //OAM Scan
            if (dotCounter >= 0 && dotCounter <= 79) {
            
            } else if (dotCounter > 70 && dotCounter <= mode3Len) {
            } if (dotCounter == 455) {
                //TODO this will mess up timing probably
                write_byte(LY,ly++);
                dotCounter = i;
            }
        }
    }

    return cycles2;
} 

int eventController() {
    int flag = 0;
    SDL_Event e;

    //Handle events on queue
    //User requests quit
    while( SDL_PollEvent( &e ) != 0 ) {//Update the surface
        if(e.type == SDL_QUIT)
        {
            flag = 1;
            SDL_UpdateWindowSurface(w);
            close();
            return flag;
        }
    }
    return flag;
}

void close() {
    //Deallocate surface
    SDL_FreeSurface(sf);
    sf = NULL;

    //Destroy window
    SDL_DestroyWindow(w);
    w = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
