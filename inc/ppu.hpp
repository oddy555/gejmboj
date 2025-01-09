#include <SDL2/SDL.h>

enum bgFetchState {
  GET_TILE,
  GET_LOW,
  GET_HIGH,
  PUSH
}; 

class Ppu {
    private:
        int dotCounter;
        int lx;
        int fetcherX;
        int fetcherY;
        int tileOffset;
        int tileX;
        SDL_Window* w = NULL;
        SDL_Surface* sf = NULL;
        SDL_Renderer* ren = NULL; 
        uint8_t tileLow;
        uint8_t tileHigh;
        bgFetchState state;

    public:
        Ppu();
        int lcdController(int cycles);
        int eventController();
        void close();
};

