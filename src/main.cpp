#include "../inc/cpu.hpp"
#include "../inc/memory.hpp"
//#include "../inc/ppu.hpp"

int main(int argc, char *argv[]) {
    printf("Starting gameboy");        
    init_mem();
    init_registers();
    //init_video();
    //skip_boot();
    //Ppu *ppu = new Ppu();
    Ppu ppu = Ppu();

    int running = 1;
    while (running) {
        cpu_step(running, ppu);
    }
}
