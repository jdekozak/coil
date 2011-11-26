all: geometry_generator

geometry_generator: main.o
	g++ -lboost_program_options-mt main.o -o geometry_generator

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm main.o geometry_generator

test:
	./geometry_generator
