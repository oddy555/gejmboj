#include "../inc/cpu.hpp"
#include "../inc/memory.hpp"
#include "../inc/video.hpp"

int main(int argc, char *argv[]) {
    printf("Starting gameboy");        
    init_mem();
    init_registers();
    //init_video();
    //skip_boot();
    int running = 1;
    while (running) {
        cpu_step(running);
    }
}
