run: target/bin/program
	./target/bin/program

debug: main.o pool.o
	g++ main.o pool.o -o target/bin/program -std=c++20 -S

all: main.o pool.o
	g++ main.o pool.o -o target/bin/program -std=c++20

main.o:
	g++ -c src/main.cpp -std=c++20 -g -o main.o

pool.o:
	g++ -c src/Threading/Pool.cpp -std=c++20 -g -o pool.o

clean:
	rm -f *.o *.s core target/bin/program
