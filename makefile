CC= g++
CFLAGS =  -g -Wall -O3 -c
LDFLAGS = -g 	

main: cpu.o memory.o instructions.o
	$(CC) $(LDFLAGS) cpu.o memory.o instructions.o -o main		

memory.o: src/memory.cpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/memory.cpp 

instructions.o: src/instructions.cpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/instructions.cpp

cpu.o: src/cpu.cpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/cpu.cpp

memory_test: test/memory_test.cpp src/memory.cpp inc/memory.hpp
	$(CC) $(LDFLAGS) test/memory_test.cpp src/memory.cpp inc/memory.hpp 
clean:
	rm *.o  
