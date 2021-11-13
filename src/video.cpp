#include "../inc/video.hpp"
#include "../inc/memory.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <queue>
#include <iostream>
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

//Important addresses
const int LY = 0xFF44; //LCDC Y-Coordinate register
    
/*  Bit 7 - LCD Display Enable             (0=Off, 1=On)
    Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
    Bit 5 - Window Display Enable          (0=Off, 1=On)
    Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
    Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
    Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
    Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
    Bit 0 - BG/Window Display/Priority     (0=Off, 1=On) */
const int LCDC = 0xFF40; //LCDC register

/*  Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
    Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
    Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
    Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
              0: During H-Blank
              1: During V-Blank
              2: During Searching OAM
              3: During Transferring Data to LCD Driver*/
const int LCDC_STATUS = 0xFF41; //LCDC status register

static int dotCounter = 0;
int lcdX = 0; //TODO is this even correct? Who knows, maybe god.

SDL_Window* w = NULL;
SDL_Surface* sf = NULL;
SDL_Renderer* ren = NULL; 
struct pixel {
    int color;
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
std::queue<pixel> bgQ;

void close();

void init_video () {
    SDL_SetMainReady();
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not intialize! SDL_Error: %s\n",SDL_GetError());
    } else {
        w = SDL_CreateWindow( "SDL Tutorial", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        SDL_SetWindowBordered(w,SDL_TRUE);
        ren = SDL_CreateRenderer(w,-1, SDL_RENDERER_ACCELERATED);
        if(ren == NULL )
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        }
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
    std::cout << "Enter lcdController, cycles: " << cycles  << " docounter: " << dotCounter  <<std::endl;
    int cycles2 = 0;
    int ly = 0;
    int bgX = 0;
    int bgY = 0;
    int wX = 0;
    int wY = 0;
    int fetchX = 0;
    int fetchY= 0;
    int bgTileLoc = 0x9800;
    //int currX = 0;
    int tileMapSelect = 0;
    int mode3Len = 0;
    int lcdc = read_byte(LCDC);
    int lcdcStatus = read_byte(LCDC_STATUS);
    cycles = cycles;
    ly = read_byte(LY);

    for (int i = 0; i < cycles; i++) {
        sf = SDL_GetWindowSurface(w);
        //dotCounter++;
        
        if (lcdc & 0x80) {
            //OAM Scan
            if (dotCounter >= 0 && dotCounter <= 79) {
                 
            } else if (dotCounter > 79 && dotCounter <= mode3Len) {
                bgX = read_byte(0xFF43);
                bgY = read_byte(0xff42);
                wX = read_byte(0xFF4B);
                wY = read_byte(0xff4a);

                if (dotCounter == 80) {
                    lcdcStatus = lcdcStatus | 0x03;
                    std::queue<pixel> empty1;
                    std::queue<pixel> empty2;
                    std::swap(spriteQ,empty1);
                    std::swap(bgQ,empty2);
                }
                
                if ((lcdc & 0x04 && lcdX < wX)|| (lcdc & 0x07 && lcdX > wX)) { //TODO wX should not be used here, probably
                    bgTileLoc = 0x9c00;
                }
                uint8_t lyMod = ly % 8;
                uint8_t temp1 = read_byte(bgTileLoc + lyMod);
                uint8_t temp2 = read_byte(bgTileLoc + lyMod + 1); 
                for (int j = 7; j >= 0; j--) {
                    pixel bgP;
                    bgP.color = (((temp1 >> j) & 1)<<1);
                    bgP.color |= ((temp2 >> j) & 1);
                    bgQ.push(bgP);
                }
                for (int j = 0; j < 8; j++)
                {
                    pixel tempP = bgQ.front();
                    switch(tempP.color) {
                        case 00:
                            SDL_SetRenderDrawColor(ren,255,255,255,255);
                            break;
                        case 01:
                            SDL_SetRenderDrawColor(ren,169,169,169,255);
                            break;
                        case 10:
                            SDL_SetRenderDrawColor(ren,84,84,84,255);
                            break;
                        case 11:
                            SDL_SetRenderDrawColor(ren,0,0,0,255);
                            break;
                        default:
                            printf("ERROR ILLEGAL PIXEL VALUE");
                    }
                    SDL_RenderDrawPoint(ren,bgX+j,bgY+lyMod);
                    bgQ.pop();
                }
                SDL_UpdateWindowSurface(w);
                dotCounter += 2;
            } else if (dotCounter >= mode3Len && dotCounter < 455) {
               //HBlank 
            } else if (dotCounter == 455) {
                //TODO this will mess up timing probably
                write_byte(LY,ly++);
                dotCounter = i;
            }
        }
    }

    return 0;
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
