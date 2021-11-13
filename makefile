CC = g++
CFLAGS =  -g -Wall -O3 -c
LDFLAGS = -g -Wall -O3	
GTEST = -lgtest -lgtest_main -lpthread

main: cpu.o memory.o instructions.o video.o debug.o register.o
	$(CC) $(LDFLAGS) -lSDL2 src/main.cpp cpu.o memory.o instructions.o video.o debug.o register.o -o main		

debug: cpu_debug.o memory.o instructions_debug.o debug.o
	$(CC) $(LDFLAGS) cpu.o memory.o instructions.o debug.o -o mainDebug	

memory.o: src/memory.cpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/memory.cpp 

register.o: src/register.cpp inc/register.hpp
	$(CC) $(CFLAGS) src/register.cpp 

instructions.o: src/instructions.cpp inc/register.hpp inc/instructions.hpp inc/memory.hpp
	$(CC) $(CFLAGS) src/instructions.cpp

cpu.o: src/cpu.cpp inc/cpu.hpp inc/register.hpp inc/instructions.hpp inc/memory.hpp 
	$(CC) $(CFLAGS) src/cpu.cpp

video.o: src/video.cpp inc/video.hpp
	$(CC) $(CFLAGS) -lSDL2 src/video.cpp
#cpu_test: test/cpu_test.cpp src/cpu.cpp inc/cpu.hpp inc/register.hpp src/register.cpp inc/instructions.hpp src/instructions.cpp inc/memory.hpp src/memory.cpp
#	$(CC) $(LDFLAGS) test/cpu_test.cpp src/cpu.cpp inc/cpu.hpp 

cpu_test: test/cpu_test.cpp cpu.o memory.o instructions.o register.o debug.o 
	$(CC) $(LDFLAGS) test/cpu_test.cpp cpu.o memory.o instructions.o register.o debug.o $(GTEST)

instructions_test: test/instructions_test.cpp memory.o instructions.o register.o debug.o 
	$(CC) $(LDFLAGS) test/instructions_test.cpp memory.o instructions.o register.o debug.o $(GTEST) 

memory_test: test/memory_test.cpp src/memory.cpp inc/memory.hpp
	$(CC) $(LDFLAGS) test/memory_test.cpp src/memory.cpp inc/memory.hpp 

#register_test: test/register_test.cpp inc/register.hpp src/register.cpp
register_test: test/register_test.cpp src/register.cpp inc/register.hpp
	$(CC) $(LDFLAGS) test/register_test.cpp src/register.cpp inc/register.hpp $(GTEST)

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
	rm -f *.o main mainDebug a.out
