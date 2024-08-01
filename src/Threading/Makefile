#run: target/bin/program
#	./target/bin/program

CXXFLAGS := -std=c++20 -c -ggdb -O0

CC := g++
SRCS := main.cpp \
		Threading/instr.cpp \
		Threading/outsr.cpp \
		Threading/Pool.cpp \
		Threading/Queue.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

%.o: %.cpp
	$(CC) $(CXXFLAGS) $^ -o $@

all: $(OBJS)
	$(CC) $^ -o program

#main.o:
#	g++ -c src/main.cpp -std=c++20 -g -o main.o
#
#pool.o:
#	g++ -c src/Threading/Pool.cpp -std=c++20 -g -o pool.o
#
#outsr.o:
#	g++ -c src/Threading/outsr.cpp -std=c++20 -g -o outsr.o
#
#instr.o:
#	g++ -c src/Threading/instr.cpp -std=c++20 -g -o instr.o
#
#queue.o:
#	g++ -c src/Threading/Queue.cpp -std=c++20 -g -o queue.o

clean:
	rm -f *.o *.s core target/bin/program
