all: mesh_generator

mesh_generator: main.o
	g++ -L/sw/lib -lboost_program_options-mt main.o -o mesh_generator

main.o: main.cpp
	g++ -I/sw/include/boost-1_41 -c main.cpp -o main.o

clean:
	rm main.o mesh_generator

test:
	./mesh_generator
