all: mesh_generator

mesh_generator: main.o
	g++ -lboost_program_options-mt main.o -o mesh_generator

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm main.o mesh_generator

test:
	./mesh_generator
