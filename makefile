run: target/bin/program
	./target/bin/program

debug: main.o industry.o security.o sector.o OrderExec.o Order.o ThreadPool.o SafeQueue.o Logging.o
	g++ main.o industry.o security.o Order.o OrderExec.o ThreadPool.o SafeQueue.o Logging.o -o target/bin/program -S

all: main.o Log.o LogItem.o
	g++ main.o LogItem.o Log.o -o target/bin/program -std=c++20

LogItem.o: src/Logging/LogItem.cpp
	g++ -c src/Logging/LogItem.cpp -O3 -std=c++20 -g -o LogItem.o

Log.o: src/Logging/Log.cpp
	g++ -c src/Logging/Log.cpp -O3 -std=c++20 -g -o Log.o

main.o:
	g++ -c src/main.cpp -O3 -std=c++20 -g -o main.o

clean:
	rm -f *.o *.s core
