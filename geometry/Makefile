all: geometry_generator

geometry_generator: main.o
	g++ -L/sw/lib -lboost_program_options-mt main.o -o geometry_generator

main.o: main.cpp
	g++ -I/sw/include/boost-1_41 -c main.cpp -o main.o

clean:
	rm main.o geometry_generator

test:
	./geometry_generator
