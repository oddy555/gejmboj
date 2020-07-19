#define DEBUGON

#ifdef DEBUGON
#define DEBUG
#define DEBUGGER

#include <bitset>
#define BIN(n) std::bitset<8>(n)

#endif 



//#ifdef DEBUGGER 

void start_debugger();

int step(); 


//#endif
