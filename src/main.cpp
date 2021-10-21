#include "../inc/cpu.hpp"
#include "../inc/memory.hpp"

int main() {
    init_mem();
    init_registers();

    int running = 1;
    while (running) {
        cpu_step();
    }
}
