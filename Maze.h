#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <mcpp/mcpp.h>
#include <vector>

// Maze class stores only information related to construction or generation of
// the maze, not the maze's location in minecraft
class Maze {
  public:
    // Constructors
    Maze(unsigned int xLen, unsigned int zLen, bool mode);
    Maze(std::vector<std::vector<char>> mazeVec);
    Maze();

    void Set_Len(unsigned int xLen, unsigned int zLen);
    std::vector<unsigned int> Get_Len();

    void Set_Mode(bool mode);
    bool Get_Mode();

    void Set_Maze(std::vector<std::vector<char>> mazeVec);
    std::vector<std::vector<char>> Get_Maze();

    void Generate();

    void Print();
    void Print(std::vector<std::vector<char>> mazeVec);

    unsigned int getXLen();
    unsigned int getZLen();

    // Destructors
    ~Maze();

  private:
    // xLen = columns
    unsigned int xLen;
    // zLen = rows
    unsigned int zLen;
    bool mode;
    std::vector<std::vector<char>> mazeVec;

    // This Function checks adjacent squares for a specific character
    void Check_Adj(std::vector<std::vector<char>>& mazeVec, unsigned int& xCurr,
                   unsigned int& zCurr, char targetChar, unsigned int dist,
                   bool& north, bool& east, bool& south, bool& west);
};

#endif
// ASSIGN_MAZE_H