#define DEBUGON
#define DISASSEMBLY

#ifdef DEBUGON
//#define DEBUG
//#define DEBUGGER

#include <bitset>
#include <string>
#define BIN(n) std::bitset<8>(n)

#endif 

//string opCodeToInstrStr[];

//#ifdef DEBUGGER 

void debug_print(std::string dbgString);

void start_debugger();

int step(); 


//#endif
