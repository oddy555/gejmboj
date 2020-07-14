CC= g++
CFLAGS =  -g -Wall -O3 -c
LDFLAGS = -g 	

main: cpu.o memory.o instructions.o video.o
	$(CC) $(LDFLAGS) -lSDL2 cpu.o memory.o instructions.o video.o -o main		

memory.o: src/memory.cpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/memory.cpp 

instructions.o: src/instructions.cpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/instructions.cpp

cpu.o: src/cpu.cpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/cpu.cpp

video.o: src/video.cpp inc/video.hpp
	$(CC) $(CFLAGS) -lSDL2 src/video.cpp

memory_test: test/memory_test.cpp src/memory.cpp inc/memory.hpp
	$(CC) $(LDFLAGS) test/memory_test.cpp src/memory.cpp inc/memory.hpp 
clean:
	rm *.o  
