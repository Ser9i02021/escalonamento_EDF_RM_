caminho_arquivo = $(wildcard arquivos/*.cc) $(wildcard *.cpp)
CC = g++
CHAVES = -Wall -std=c++11  # Set C++ standard to C++11

all: 
	@${CC} $(caminho_arquivo) $(CHAVES) -o main

run:
	./main

clean:
	rm -rf ./main/*.out