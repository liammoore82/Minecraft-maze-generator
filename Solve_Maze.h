#ifndef SOLVE_MAZE_H
#define SOLVE_MAZE_H

#include <chrono>
#include <iostream>
#include <mcpp/mcpp.h>
#include <thread>

class Solve_Maze {
  public:
    // returns true if solver reaches exit, and stops solver
    static bool isOutsideMaze(int&, const std::vector<mcpp::Coordinate>&);

    // used to find the coordinates of the exit using the length and width of
    // the maze and checking for openings on the edges
    static mcpp::Coordinate findExitCoords(int, int, mcpp::Coordinate);

    static bool placePlayerInMaze(const std::vector<std::vector<char>>&,
                                  mcpp::Coordinate, int, int, bool);

    // option [2], guides the player out of the maze
    static bool showEscapeRoute(int, int, mcpp::Coordinate,
                                const std::vector<std::vector<char>>&, bool);

    // populated openCoordsField with open blocks within the maze
    static void getOpenBlocks(const std::vector<std::vector<char>>&,
                              mcpp::Coordinate, int, int);

  private:
    // fields
    static mcpp::Coordinate exitCoords;
    static std::vector<mcpp::Coordinate> openCoordsField;
};

#endif // SOLVE_MAZE_H