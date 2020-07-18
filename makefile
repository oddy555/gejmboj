CC= g++
CFLAGS =  -g -Wall -O3 -c
LDFLAGS = -g 	

main: cpu.o memory.o instructions.o debug.o
	$(CC) $(LDFLAGS) cpu.o memory.o instructions.o debug.o -o main		

debug: cpu_debug.o memory.o instructions_debug.o debug.o
	$(CC) $(LDFLAGS) cpu.o memory.o instructions.o debug.o -o mainDebug	

memory.o: src/memory.cpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/memory.cpp 

instructions.o: src/instructions.cpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/instructions.cpp

cpu.o: src/cpu.cpp inc/instructions.hpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/cpu.cpp

memory_test: test/memory_test.cpp src/memory.cpp inc/memory.hpp
	$(CC) $(LDFLAGS) test/memory_test.cpp src/memory.cpp inc/memory.hpp 

#memory_debug.o: src/memory.cpp inc/memory.hpp debug/debug.cpp
#	$(CC) $(CFLAGS) src/memory.cpp 

instructions_debug.o: src/instructions.cpp inc/memory.hpp inc/instructions.hpp debug/debug.cpp
	$(CC) $(CFLAGS) src/instructions.cpp 

cpu_debug.o: src/cpu.cpp inc/memory.hpp inc/instructions.hpp debug/debug.hpp
	$(CC) $(CFLAGS) src/cpu.cpp

#video_debug.o: src/video.cpp inc/video.hpp
#	$(CC) $(CFLAGS) -lSDL2 src/video.cpp 


debug.o: debug/debug.cpp debug/debug.hpp inc/memory.hpp
	$(CC) $(CFLAGS) debug/debug.cpp 

clean:
	rm -f *.o main mainDebug
