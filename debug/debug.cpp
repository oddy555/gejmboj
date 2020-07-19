#include "debug.hpp"

#ifdef DEBUGGER 
#include "../inc/memory.hpp"
#include <iostream>
#include <string>

void start_debugger() {
    std::cout << "Starting debugger! Write help for commands" << std::endl;
}

int step() {
    char temp[15];
    std::string input = "";
    int run = 0;
    while (!run) {
        std::cout << "DBZ> ";
        std::cin.getline(temp,sizeof(temp));
        input = temp;
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
            try {
                int temp = std::stoi(input.erase(0,input.find("print8")+ 7),nullptr,16);
                std::cout << (int) read_byte(temp) << std::endl; 
                input = "";
            } catch (std::invalid_argument e) {
                std::cerr << e.what() << std::endl;
            }

            input = "";
        }  else if (input.find("print16") != std::string::npos) {
            try {
                int temp = std::stoi(input.erase(0,input.find("print16")+ 7),nullptr,16);
                std::cout << (int) read_word(temp) << std::endl; 
                input = "";
            } catch (std::invalid_argument e) {
                std::cerr << e.what() << std::endl;
            }

            input = "";
        } else if (input.find("step") != std::string::npos) {
            std::cout << std::endl;
            return 0;
        } else if (input.find("exit") != std::string::npos) {
            return 1;
        } else {
            std::cout << "Invalid Command!" << std::endl;
            input = "";
        }
    }
#ifdef DEBUG
    std::cout<<std::endl;
#endif
    return 0;
}

#endif
