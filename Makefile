all: currentLoop mesh_generator geometry_generator

mesh_generator:
	(cd mesh; make)

geometry_generator:
	(cd geometry; make)

currentLoop: main.o
	g++ -fopenmp -O3 -lboost_program_options main.o -o currentLoop

main.o: main.cpp calculator.hpp reader.hpp outputter.hpp
	g++ -fopenmp -O3 -c main.cpp -o main.o

clean:
	rm main.o currentLoop
	(cd mesh; make clean)
	(cd geometry; make clean)

test:
	./currentLoop
