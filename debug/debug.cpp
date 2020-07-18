#include "debug.hpp"

#ifdef DEBUGGER 
#include "../inc/memory.hpp"
#include <iostream>
#include <string>

void start_debugger() {
    std::cout << "Starting debugger! Write help for commands" << std::endl;
}

void step() {
    std::string input = "";
    std::cout << "DBZ> ";
    std::cin >> input;
    if (input.find("help") != std::string::npos) {
        std::cout << "Command List!" << std::endl;
        std::cout << "Step: Continue with another iteration in loop" << std::endl;
        std::cout << "print memory: Prints the whole memory(65535 lines, be careful)" << std::endl; 
        std::cout << "print8 address: Prints a byte from memory at address(hex)" << std::endl;
        std::cout << "print16 address: Same as print8 but with a word instead" << std::endl;
        input = "";
    } else if (input.find("print memory") != std::string::npos) {
        print_memory();     
        input = "";
    }  else if (input.find("print8") != std::string::npos) {
        uint16_t temp = std::stoul(input.erase(0,input.find("print8")+ 7),nullptr,16);
                
        std::cout << read_byte(temp); 
        input = "";
    }
#ifdef DEBUG
    std::cout<<std::endl;
#endif

}

#endif
#ifdef DEBUG
#endif
