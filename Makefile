.default: all

all: mazeRunner

clean:
	rm -f mazeRunner *.o

mazeRunner: Block.o	Maze.o  Solve_Maze.o  Terrain_Array.o	Place_Maze.o	mazeRunner.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^
