#run: target/bin/program
#	./target/bin/program

CXXFLAGS := -std=c++20 -c -ggdb -O0 -Wall -Wextra -fsanitize=address -static-libasan

CC := g++
SRCS := main.cpp \
		Threading/instr.cpp \
		Threading/outsr.cpp \
		Threading/Pool.cpp \
		# Threading/Queue.cpp

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

%.o: %.cpp
	$(CC) $(CXXFLAGS) $^ -o $@

all: $(OBJS)
	$(CC) $^ -fsanitize=address -static-libasan -o program

clean:
	rm -f $(OBJS) *.s core target/bin/program

info:
	echo "SRCS=$(SRCS)"
	echo "OBJS=$(OBJS)"

.PHONY: all clean info
