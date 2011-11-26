all: currentLoop

currentLoop: main.o
	g++ -fopenmp -O3 -lboost_program_options-mt main.o -o currentLoop

main.o: main.cpp calculator.hpp reader.hpp outputter.hpp
	g++ -fopenmp -O3 -c main.cpp -o main.o

clean:
	rm main.o currentLoop

test:
	./currentLoop
