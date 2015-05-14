all: geometry_generator

geometry_generator: main.o
	g++ -O3 -lboost_program_options main.o -o geometry_generator

main.o: main.cpp
	g++ -O3 -c main.cpp -o main.o

clean:
	rm main.o geometry_generator

test:
	./geometry_generator
