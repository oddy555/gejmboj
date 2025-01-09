#include "../inc/memory.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <queue>
#include <iostream>
#include "../inc/ppu.hpp"

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

//VRAM
const int OAM_START = 0xFE00;


int lcdX = 0; //TODO is this even correct? Who knows, maybe god.

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

static sprite spriteBuffer[10];

/*std::queue<pixel> spriteQ;
std::queue<pixel> bgQ;
std::queue<pixel> pixelQ;*/
std::queue<uint8_t> spriteQ;
static std::queue<uint8_t> bgQ;
std::queue<pixel> pixelQ;

void close();

Ppu::Ppu () {
    SDL_SetMainReady();
    dotCounter = 0; 
    lx = 0;
    fetcherX = 0;
    fetcherY = 0;
    state = GET_TILE;
    write_byte(LY, 0);

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


int Ppu::lcdController(int cycles) {
    //std::cout << "===== LCD Enter lcdController, cycles: " << cycles  << " docounter: " << dotCounter  <<std::endl;
    int bgTileMapLoc = LCDC & 0x08? 0x9800: 0x9C00;
    int mode3Len = 172;
    int lcdc = read_byte(LCDC);
    int lcdcStatus = read_byte(LCDC_STATUS);
    uint8_t ly = read_byte(LY);
    uint8_t scX = 0; 
    uint8_t scY = 0;
   /* for (int i = 0; i < 144; i++) {
        for (int j = 0; j < 160; j++) {
            SDL_SetRenderDrawColor(ren,255,0,255,255);
            SDL_RenderDrawPoint(ren,j,i);
            SDL_RenderPresent( ren );
        }
    }*/
    if (read_byte(LCDC) & 0x80) {
        std::cout << "=== LCD LCD enabled" << std::endl;
        for (int i = 0; i < cycles*4; i++) { // Four dots per cycle
            std::cout << "LCD i: " << std::dec << i << " dotCounter: " << dotCounter << std::endl;
            if (ly > 143 && ly < 153) {
                std::cout << "LCD ly > 143 && ly < 153" << std::endl;
                if (ly == 152) {
                    write_byte(LY,0);
                    dotCounter=0;
                    continue;
                }
                write_byte(LY,ly++);
                continue;
            }
            if (dotCounter >= 0 && dotCounter < 80) {
                std::cout << "LCD dotCounter >= 0 && dotCounter < 80" << std::endl;
                //OAM SCAN
                dotCounter++;
            } else if (dotCounter >= 80 && dotCounter <= mode3Len) {
                if (state == GET_TILE) {
                    tileX = (((scX/8)+fetcherX)&0x1F);
                    //int tileY = (ly + scY)& 0xFF;
                    tileOffset = tileX + (32 * (((ly + scY) & 0xFF) / 8)) & 0x3ff;
                    state = GET_LOW;
                    dotCounter += 1;
                    std::cout << "LCD tileOffset" << std::dec << tileOffset  << " scX: " << int(scX) << std::endl;
                } else if (state == GET_LOW) {
                    tileLow = read_byte(bgTileMapLoc + tileOffset);
                    state = GET_HIGH;
                    dotCounter += 1;
                    std::cout << "LCD tileLow" << std::hex << int(tileLow) << std::endl;
                } else if (state == GET_HIGH) {
                    tileHigh = read_byte((bgTileMapLoc + tileOffset+1));
                    state = PUSH;
                    dotCounter +=1;
                    std::cout << "LCD tileHigh" << std::hex << int(tileHigh) << std::endl;
                } else if (state == PUSH) {
                    std::cout << "LCD PUSH" << std::endl;
                    if (bgQ.empty()) {
                        for (int j = 0; j < 8; j++) {
                            uint8_t p1 = (tileLow>>j) & 0x01;
                            uint8_t p2 = ((tileHigh>>j) & 0x01)<<1;
                            uint8_t p3 = p1 | p2;
                            bgQ.push(p3);
                        }
                        state = GET_TILE;
                    }
                    dotCounter += 1;
                }
                std::cout << "LCD !bgQ.empty(): " << !bgQ.empty() << std::endl;
                if (lx < 160 && !bgQ.empty()) {
                    std::cout << "LCD lx < 160 && !bgQ.empty()" << std::endl;
                    sf = SDL_GetWindowSurface(w);
                    int tempP = bgQ.front();
                    std::cout << "LCD tempP: " << std::hex << tempP << std::endl;
                    switch(tempP) {
                        case 0x0:
                            SDL_SetRenderDrawColor(ren,255,255,255,255);
                            break;
                        case 0x1:
                            SDL_SetRenderDrawColor(ren,169,169,169,255);
                            break;
                        case 0x2:
                            SDL_SetRenderDrawColor(ren,84,84,84,255);
                            break;
                        case 0x4:
                            SDL_SetRenderDrawColor(ren,0,0,0,255);
                            break;
                        default:
                            printf("ERROR ILLEGAL PIXEL VALUE");
                    }
                    std::cout << "LCD SDL_RenderDrawPoint lx: " << std::dec << lx << " ly: "<< int(ly) << std::endl;
                    SDL_RenderDrawPoint(ren,lx++,ly);
                    //SDL_RenderDrawPoint(ren,lx+scX+j,bgY+bgY+lyMod);
                    bgQ.pop();
                }
            } else if (dotCounter > mode3Len && dotCounter < 456) {
                std::cout << "LCD dotCounter > mode3Len && dotCounter" << std::endl;
                if (dotCounter == 455) {
                    std::cout << "LCD dotCounter == 455 ly inc" << std::endl;
                    SDL_RenderPresent( ren );
                    dotCounter = 0;
                    ly += 1;
                    lx = 0;
                    write_byte(LY,ly);
                    continue;
                }
                dotCounter ++;
            }

            
            //bgQ.pop();
                    /*if (pixelQ.size() > 8) {
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
                        SDL_RenderDrawPoint(ren,bgX+scX+j,bgY+bgY+lyMod);
                        //bgQ.pop();
                    }*/
        }
    }
    return 0;
} 

int Ppu::eventController() {
    int flag = 0;
    SDL_Event e;

    //Handle events on queue
    //User requests quit
    while(SDL_PollEvent(&e) != 0) {//Update the surface
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

void Ppu::close() {
    //Deallocate surface
    SDL_FreeSurface(sf);
    sf = NULL;

    //Destroy window
    SDL_DestroyWindow(w);
    w = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
