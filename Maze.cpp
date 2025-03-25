#include "Maze.h"

#include <random>

/*
 * Constructors
 * Use before maze generation
 */

Maze::Maze(unsigned int xLen, unsigned int zLen, bool mode) {
    Maze::Set_Len(xLen, zLen);
    Maze::Set_Mode(mode);

    /*
     * Generate a vector with zLen rows and xLen columns,
     * default the value to '.'
     */
    std::vector<std::vector<char>> mazeVec(zLen, std::vector<char>(xLen, '.'));
    Maze::Set_Maze(mazeVec);
}

// Use when passing maze from input
Maze::Maze(std::vector<std::vector<char>> mazeVec) {
    Maze::Set_Maze(mazeVec);
    Maze::Set_Len(mazeVec.at(0).size(), mazeVec.size());
    // Mode doesn't matter here - i think it still needs to set one?
    Maze::Set_Mode(0);
}

Maze::Maze() {
    Maze::Set_Maze(std::vector<std::vector<char>>());
    Maze::Set_Len(0, 0);
    // Mode doesn't matter here - i think it still needs to set one?
    Maze::Set_Mode(0);
}

/*
 * Main logic for maze generation in here,
 * assumes Maze.set_len has already been used
 */
void Maze::Generate() {
    std::vector<unsigned int> lens;
    lens = Maze::Get_Len();
    unsigned int xLen = lens.at(0);
    unsigned int zLen = lens.at(1);
    bool mode = Maze::Get_Mode();
    std::vector<std::vector<char>> mazeVec(zLen, std::vector<char>(xLen, 'x'));

    // Generate 'odd' array
    for (unsigned int i = 0; i < zLen; i++) {
        for (unsigned int j = 0; j < xLen; j++) {
            if (i % 2 == 1 && j % 2 == 1) {
                mazeVec.at(i).at(j) = ' ';
            }
        }
    }

    unsigned int zStart = 0, xStart = 0, zIn = 0, xIn = 0;
    if (mode == false) {

        // pick a wall

        unsigned int wall = std::rand() % 4;

        // Top wall
        if (wall == 0) {
            xStart = (std::rand() % (xLen / 2) + 1) * 2 - 1;
            xIn = xStart;
            zStart = 0;
            zIn = 1;

            // Right wall
        } else if (wall == 1) {
            xStart = xLen - 1;
            xIn = xLen - 2;
            zStart = (std::rand() % (zLen / 2) + 1) * 2 - 1;
            zIn = zStart;

            // Bottom wall
        } else if (wall == 2) {
            xStart = (std::rand() % (xLen / 2) + 1) * 2 - 1;
            xIn = xStart;
            zStart = zLen - 1;
            zIn = zLen - 2;

            // Left wall
        } else if (wall == 3) {
            xStart = 0;
            xIn = 1;
            zStart = (std::rand() % (zLen / 2) + 1) * 2 - 1;
            zIn = zStart;
        }

    } else {
        zIn = 1;
        xIn = 1;
        zStart = 1;
        xStart = 0;
    }
    mazeVec.at(zStart).at(xStart) = '.';
    mazeVec.at(zIn).at(xIn) = '*';

    bool finished = false;

    // Solved check
    int badCount = 0;
    for (unsigned int i = 0; i < zLen; i++) {
        for (unsigned int j = 0; j < xLen; j++) {
            if (mazeVec.at(i).at(j) == ' ' || mazeVec.at(i).at(j) == '*') {
                badCount++;
            }
        }
    }

    // This implies the only square left is the current square
    if (badCount == 1) {
        for (unsigned int i = 0; i < zLen; i++) {
            for (unsigned int j = 0; j < xLen; j++) {
                if (mazeVec.at(i).at(j) == '*') {
                    mazeVec.at(i).at(j) = '.';
                }
            }
        }
        badCount = 0;
    }

    // break 'while' condition
    if (badCount == 0) {
        finished = true;
    }

    while (!finished) {
        // recognize current location
        unsigned int zCurr = 0;
        unsigned int xCurr = 0;
        for (unsigned int i = 0; i < zLen; i++) {
            for (unsigned int j = 0; j < xLen; j++) {
                if (mazeVec.at(i).at(j) == '*') {
                    zCurr = i;
                    xCurr = j;
                }
            }
        }

        // Check adjacencies

        // north/Up = 0
        bool north = false;
        bool east = false;
        bool south = false;
        bool west = false;
        Maze::Check_Adj(mazeVec, xCurr, zCurr, ' ', 2, north, east, south,
                        west);

        // If possible, carve, else backtrack
        if (north || east || south || west) {
            // This section picks a direction from among those available.
            std::vector<unsigned int> dirVec;
            if (north) {
                dirVec.push_back(0);
            }
            if (east) {
                dirVec.push_back(1);
            }
            if (south) {
                dirVec.push_back(2);
            }
            if (west) {
                dirVec.push_back(3);
            }
            unsigned int randDir = rand() % dirVec.size();
            // Carve in following order in testing mode: NESW
            if (mode == true) {
                randDir = 0;
            }
            unsigned int carveDir = dirVec.at(randDir);

            // This section 'carves' in the given direction

            // north
            if (carveDir == 0) {
                mazeVec.at(zCurr).at(xCurr) = '.';
                mazeVec.at(zCurr - 1).at(xCurr) = '.';
                mazeVec.at(zCurr - 2).at(xCurr) = '*';

                // east
            } else if (carveDir == 1) {
                mazeVec.at(zCurr).at(xCurr) = '.';
                mazeVec.at(zCurr).at(xCurr + 1) = '.';
                mazeVec.at(zCurr).at(xCurr + 2) = '*';

                // south
            } else if (carveDir == 2) {
                mazeVec.at(zCurr).at(xCurr) = '.';
                mazeVec.at(zCurr + 1).at(xCurr) = '.';
                mazeVec.at(zCurr + 2).at(xCurr) = '*';

                // west
            } else if (carveDir == 3) {
                mazeVec.at(zCurr).at(xCurr) = '.';
                mazeVec.at(zCurr).at(xCurr - 1) = '.';
                mazeVec.at(zCurr).at(xCurr - 2) = '*';
            }

        } else {
            // Backtrack Here

            // Default fromDir out of range (0-3)
            unsigned int fromDir = 4;

            bool adjacencyAvailable = false;
            // break backtrack using condition

            while (!adjacencyAvailable) {
                // Check available Backtrack directions
                bool north = false;
                bool east = false;
                bool south = false;
                bool west = false;

                Maze::Check_Adj(mazeVec, xCurr, zCurr, '.', 1, north, east,
                                south, west);

                /*
                 * This section picks a direction from among those available.
                 * This excludes the direction
                 */
                std::vector<unsigned int> dirVec;
                if (north && (fromDir != 0)) {
                    dirVec.push_back(0);
                }
                if (east && (fromDir != 1)) {
                    dirVec.push_back(1);
                }
                if (south && (fromDir != 2)) {
                    dirVec.push_back(2);
                }
                if (west && (fromDir != 3)) {
                    dirVec.push_back(3);
                }
                unsigned int randDir = rand() % dirVec.size();
                unsigned int carveDir = 4;
                if (dirVec.size() > 0) {
                    carveDir = dirVec.at(randDir);
                } else {
                    carveDir = fromDir;
                }

                // north
                if (carveDir == 0) {
                    mazeVec.at(zCurr).at(xCurr) = '.';
                    mazeVec.at(zCurr - 1).at(xCurr) = '.';
                    mazeVec.at(zCurr - 2).at(xCurr) = '*';
                    fromDir = 2;

                    // east
                } else if (carveDir == 1) {
                    mazeVec.at(zCurr).at(xCurr) = '.';
                    mazeVec.at(zCurr).at(xCurr + 1) = '.';
                    mazeVec.at(zCurr).at(xCurr + 2) = '*';
                    fromDir = 3;

                    // south
                } else if (carveDir == 2) {
                    mazeVec.at(zCurr).at(xCurr) = '.';
                    mazeVec.at(zCurr + 1).at(xCurr) = '.';
                    mazeVec.at(zCurr + 2).at(xCurr) = '*';
                    fromDir = 0;

                    // west
                } else if (carveDir == 3) {
                    mazeVec.at(zCurr).at(xCurr) = '.';
                    mazeVec.at(zCurr).at(xCurr - 1) = '.';
                    mazeVec.at(zCurr).at(xCurr - 2) = '*';
                    fromDir = 1;
                }

                // recognize current location
                for (unsigned int i = 0; i < zLen; i++) {
                    for (unsigned int j = 0; j < xLen; j++) {
                        if (mazeVec.at(i).at(j) == '*') {
                            zCurr = i;
                            xCurr = j;
                        }
                    }
                }

                // Check adjacencies
                // north/Up = 0
                north = false;
                east = false;
                south = false;
                west = false;

                Maze::Check_Adj(mazeVec, xCurr, zCurr, ' ', 2, north, east,
                                south, west);

                if (north || east || south || west) {
                    adjacencyAvailable = true;
                }
            }
        }

        // Solved check
        int badCount = 0;
        for (unsigned int i = 0; i < zLen; i++) {
            for (unsigned int j = 0; j < xLen; j++) {
                if (mazeVec.at(i).at(j) == ' ' || mazeVec.at(i).at(j) == '*') {
                    badCount++;
                }
            }
        }

        // This implies the only square left is the current square
        if (badCount == 1) {
            for (unsigned int i = 0; i < zLen; i++) {
                for (unsigned int j = 0; j < xLen; j++) {
                    if (mazeVec.at(i).at(j) == '*') {
                        mazeVec.at(i).at(j) = '.';
                    }
                }
            }
            badCount = 0;
        }

        // break 'while' condition
        if (badCount == 0) {
            finished = true;
        }
    }

    // maze should be complete, must store
    Maze::Set_Maze(mazeVec);
}

void Maze::Print() {
    std::vector<std::vector<char>> mazeVec = Maze::Get_Maze();
    std::vector<unsigned int> lens;
    lens = Maze::Get_Len();
    unsigned int xLen = lens.at(0);
    unsigned int zLen = lens.at(1);

    for (unsigned int i = 0; i < zLen; i++) {
        for (unsigned int j = 0; j < xLen; j++) {
            std::cout << mazeVec.at(i).at(j);
        }
        std::cout << std::endl;
    }
}

void Maze::Print(std::vector<std::vector<char>> mazeVec) {
    std::vector<unsigned int> lens;
    lens = Maze::Get_Len();
    unsigned int xLen = lens.at(0);
    unsigned int zLen = lens.at(1);

    for (unsigned int i = 0; i < zLen; i++) {
        for (unsigned int j = 0; j < xLen; j++) {
            std::cout << mazeVec.at(i).at(j);
        }
        std::cout << std::endl;
    }
}

void Maze::Check_Adj(std::vector<std::vector<char>>& mazeVec,
                     unsigned int& xCurr, unsigned int& zCurr, char targetChar,
                     unsigned int dist, bool& north, bool& east, bool& south,
                     bool& west) {
    std::vector<unsigned int> lens;
    lens = Maze::Get_Len();
    unsigned int xLen = lens.at(0);
    unsigned int zLen = lens.at(1);

    if (zCurr > dist - 1) {
        if (mazeVec.at(zCurr - dist).at(xCurr) == targetChar) {
            north = true;
        }
    }

    if (xCurr < xLen - dist) {
        if (mazeVec.at(zCurr).at(xCurr + dist) == targetChar) {
            east = true;
        }
    }

    if (zCurr < zLen - dist) {
        if (mazeVec.at(zCurr + dist).at(xCurr) == targetChar) {
            south = true;
        }
    }

    if (xCurr > dist - 1) {
        if (mazeVec.at(zCurr).at(xCurr - dist) == targetChar) {
            west = true;
        }
    }
}

// Destructors
Maze::~Maze() {}

void Maze::Set_Len(unsigned int xLen, unsigned int zLen) {
    // same as line 26
    Maze::xLen = xLen;
    Maze::zLen = zLen;
}
std::vector<unsigned int> Maze::Get_Len() {
    std::vector<unsigned int> result = {Maze::xLen, Maze::zLen};
    return result;
}

void Maze::Set_Mode(bool mode) { Maze::mode = mode; }
bool Maze::Get_Mode() { return Maze::mode; }

void Maze::Set_Maze(std::vector<std::vector<char>> mazeVec) {
    Maze::mazeVec = mazeVec;
}
std::vector<std::vector<char>> Maze::Get_Maze() { return Maze::mazeVec; }

unsigned int Maze::getXLen() { return Maze::xLen; }
unsigned int Maze::getZLen() { return Maze::zLen; }