#include "Solve_Maze.h"

mcpp::MinecraftConnection mc;
mcpp::Coordinate Solve_Maze::exitCoords(0, 0, 0);
std::vector<mcpp::Coordinate> Solve_Maze::openCoordsField;

// shows the player the way out of the maze
// option [2] in the solve maze menu section
bool Solve_Maze::showEscapeRoute(
    int mazeLength, int mazeWidth, mcpp::Coordinate mazeCoords,
    const std::vector<std::vector<char>>& mazeStructure, bool mode) {
    std::vector<mcpp::Coordinate> stepCoordinates;
    int stepNumber = 0;
    mcpp::Coordinate directions[4] = {
        mcpp::Coordinate(0, 0, -1), // North
        mcpp::Coordinate(1, 0, 0),  // East
        mcpp::Coordinate(0, 0, 1),  // South
        mcpp::Coordinate(-1, 0, 0)  // West
    };
    int dirIndex = 1;

    mcpp::Coordinate previousPlayerCoord(0, 0, 0);

    findExitCoords(mazeLength, mazeWidth, mazeCoords);
    mcpp::Coordinate altrCoords(1, 0, 1);
    exitCoords = exitCoords - altrCoords;

    getOpenBlocks(mazeStructure, mazeCoords, mazeLength, mazeWidth);

    // check if player is at a valid coordinate within the maze
    mcpp::Coordinate playerCoord(mc.getPlayerPosition());

    bool isPlayerCoordValid = false;
    for (long unsigned int i = 0; i < openCoordsField.size(); ++i) {
        if (openCoordsField[i] == playerCoord) {
            isPlayerCoordValid = true;
        }
    }

    // ends the function if the player isn't in the maze
    if (!isPlayerCoordValid) {
        return false;
    }

    stepCoordinates.push_back(playerCoord);

    // Move east if in testing mode and the east direction is open
    if (mode == 1) {
        mcpp::Coordinate eastPos = playerCoord + directions[1];
        if (mc.getBlock(eastPos) == mcpp::Blocks::AIR) {
            playerCoord = eastPos;
            stepCoordinates.push_back(playerCoord);
            ++stepNumber;
            mc.setBlock(playerCoord, mcpp::Blocks::LIME_CARPET);
            std::cout << "Step[" << stepNumber
                      << "]: " << stepCoordinates[stepNumber] << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }

    while (!isOutsideMaze(stepNumber, stepCoordinates)) {

        mc.setBlock(playerCoord, mcpp::Blocks::AIR);
        previousPlayerCoord = playerCoord;

        int rightIndex = (dirIndex + 1) % 4;
        mcpp::Coordinate rightPos = playerCoord + directions[rightIndex];

        // if theres a wall on the right
        if (mc.getBlock(rightPos) != mcpp::Blocks::AIR) {
            // check if the path ahead is open
            mcpp::Coordinate forwardPos = playerCoord + directions[dirIndex];
            if (mc.getBlock(forwardPos) == mcpp::Blocks::AIR ||
                mc.getBlock(forwardPos) == mcpp::Blocks::BLUE_CARPET) {
                playerCoord = forwardPos;

            } else {
                // turn left if blocked ahead
                dirIndex = (dirIndex + 3) % 4;
            }
        } else {
            // turn right and and move forward
            dirIndex = rightIndex;
            playerCoord = playerCoord + directions[dirIndex];
        }

        if (!(previousPlayerCoord == playerCoord)) {
            mc.setBlock(playerCoord, mcpp::Blocks::LIME_CARPET);
        }

        stepCoordinates.push_back(playerCoord);
        ++stepNumber;
        std::cout << "Step[" << stepNumber
                  << "]: " << stepCoordinates[stepNumber] << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }

    mc.setBlock(playerCoord, mcpp::Blocks::BLUE_CARPET);

    return true;
}

bool Solve_Maze::isOutsideMaze(
    int& stepNumber, const std::vector<mcpp::Coordinate>& stepCoordinates) {
    if (stepCoordinates[stepNumber] == exitCoords) {
        return true;
    }

    return false;
}

// checks every outside edge for an opening. skips the corner blocks by
// starting each check at the corner block + 1 in the direction intended
// to check.
mcpp::Coordinate Solve_Maze::findExitCoords(int length, int width,
                                            mcpp::Coordinate mazeCoords) {

    // Basepoint is off by -1 x and z, this is to correct
    mcpp::Coordinate alterMazeCoords(1, 0, 1);
    mcpp::Coordinate originalMazeCoords = mazeCoords;
    int i; // length
    int j; // width

    // checks right edge
    for (i = 0; i < length - 1; ++i) {
        mcpp::Coordinate changeCoords(0, 0, 1);
        mazeCoords = mazeCoords + changeCoords;
        if (i < length - 2) {
            if (mc.getBlock(mazeCoords) == mcpp::Blocks::AIR ||
                mc.getBlock(mazeCoords) == mcpp::Blocks::BLUE_CARPET) {
                mcpp::Coordinate minusX(1, 0, 0);
                exitCoords = mazeCoords + alterMazeCoords - minusX;
                return exitCoords;
            }
        }
    }

    // checks top edge
    for (j = 0; j < width - 1; ++j) {
        mcpp::Coordinate changeCoords(1, 0, 0);
        mazeCoords = mazeCoords + changeCoords;
        if (j < width - 2) {
            if (mc.getBlock(mazeCoords) == mcpp::Blocks::AIR ||
                mc.getBlock(mazeCoords) == mcpp::Blocks::BLUE_CARPET) {
                mcpp::Coordinate addZ(0, 0, 1);
                exitCoords = mazeCoords + alterMazeCoords + addZ;
                return exitCoords;
            }
        }
    }

    // clear variables and reset mazeCoords
    i = 0;
    j = 0;
    mazeCoords = originalMazeCoords;

    // checks bottom edge
    for (j = 0; j < width - 1; ++j) {
        mcpp::Coordinate changeCoords(1, 0, 0);
        mazeCoords = mazeCoords + changeCoords;
        if (j < width - 2) {
            if (mc.getBlock(mazeCoords) == mcpp::Blocks::AIR ||
                mc.getBlock(mazeCoords) == mcpp::Blocks::BLUE_CARPET) {
                mcpp::Coordinate minusZ(0, 0, 1);
                exitCoords = mazeCoords + alterMazeCoords - minusZ;
                return exitCoords;
            }
        }
    }

    // checks left edge
    for (i = 0; i < length - 1; ++i) {
        mcpp::Coordinate changeCoords(0, 0, 1);
        mazeCoords = mazeCoords + changeCoords;
        if (i < length - 2) {
            if (mc.getBlock(mazeCoords) == mcpp::Blocks::AIR ||
                mc.getBlock(mazeCoords) == mcpp::Blocks::BLUE_CARPET) {
                mcpp::Coordinate addX(1, 0, 0);
                exitCoords = mazeCoords + alterMazeCoords + addX;
                return exitCoords;
            }
        }
    }

    return exitCoords;
}

// drops the player into a random empty space in the maze
// option [1] in the solve maze menu section
bool Solve_Maze::placePlayerInMaze(
    const std::vector<std::vector<char>>& mazeStructure,
    mcpp::Coordinate baseCoord, int length, int width, bool mode) {

    getOpenBlocks(mazeStructure, baseCoord, length, width);

    if (mode == 1) {

        bool hasTeleported = false;

        std::vector<mcpp::Coordinate> searchPattern = {
            mcpp::Coordinate(0, 0, 0),   mcpp::Coordinate(0, 0, -1),
            mcpp::Coordinate(-1, 0, 0),  mcpp::Coordinate(-1, 0, -1),
            mcpp::Coordinate(0, 0, -2),  mcpp::Coordinate(-2, 0, 0),
            mcpp::Coordinate(-1, 0, -2), mcpp::Coordinate(-2, 0, -1),
            mcpp::Coordinate(-2, 0, -2)};

        mcpp::Coordinate oppositeCorner(width - 2, 0, length - 2);
        for (long unsigned int i = 0;
             i < searchPattern.size() && !hasTeleported; ++i) {
            mcpp::Coordinate coordVariance = searchPattern[i];
            mcpp::Coordinate altCoords =
                baseCoord + oppositeCorner + coordVariance;

            if (mc.getBlock(altCoords) == mcpp::Blocks::AIR) {
                mc.setPlayerPosition(altCoords);
                hasTeleported = true;
            }
        }

    } else {
        if (openCoordsField.size() > 0) {
            srand(static_cast<unsigned int>(time(0)));
            int vecElement = rand() % openCoordsField.size();
            mc.setPlayerPosition(openCoordsField[vecElement]);
        } else {
            return false;
        }
    }

    return true;
}

void Solve_Maze::getOpenBlocks(
    const std::vector<std::vector<char>>& mazeStructure,
    mcpp::Coordinate baseCoord, int length, int width) {

    openCoordsField.clear();

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            if (mazeStructure[i][j] == '.') {
                mcpp::Coordinate addCoord(j, 0, i);
                mcpp::Coordinate coord(addCoord + baseCoord);

                openCoordsField.push_back(coord);
            }
        }
    }
}