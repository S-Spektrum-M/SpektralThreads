run: target/bin/program
	./target/bin/program

debug: main.o pool.o
	g++ main.o pool.o -o target/bin/program -std=c++20 -S

all: main.o pool.o outsr.o instr.o queue.o
	g++ main.o pool.o outsr.o -o target/bin/program -std=c++20

main.o:
	g++ -c src/main.cpp -std=c++20 -g -o main.o

pool.o:
	g++ -c src/Threading/Pool.cpp -std=c++20 -g -o pool.o

outsr.o:
	g++ -c src/Threading/outsr.cpp -std=c++20 -g -o outsr.o

instr.o:
	g++ -c src/Threading/instr.cpp -std=c++20 -g -o instr.o

queue.o:
	g++ -c src/Threading/Queue.cpp -std=c++20 -g -o queue.o

clean:
	rm -f *.o *.s core target/bin/program
