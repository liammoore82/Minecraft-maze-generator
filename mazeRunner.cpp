#include <iostream>
#include <iterator>

#include <mcpp/mcpp.h>

#include "Maze.h"
#include "Place_Maze.h"
#include "Solve_Maze.h"
#include "menuUtils.h"

#include <chrono>
#include <limits>
#include <thread>

#define NORMAL_MODE 0
#define TESTING_MODE 1
#define TP_X 4848
#define TP_Y 71
#define TP_Z 4369

enum States {
    ST_MainMenu,
    // Main Menu States
    ST_GenerateMazeMenu,
    ST_BuildMaze,
    ST_SolveMazeMenu,
    ST_TeamInfoMenu,
    ST_Exit
};

int main(int argc, char* argv[]) {
    mcpp::MinecraftConnection mc;

    mc.doCommand("time set day");
    mc.doCommand("gamerule doDaylightCycle false");
    mc.doCommand("weather clear");
    mc.doCommand("gamerule doWeatherCycle false");

    // VARIABLES
    Maze* currMaze = new Maze();
    Place_Maze p_maze;
    mcpp::Coordinate player_position = mc.getPlayerTilePosition();
    std::vector<std::vector<char>> maze_vec;
    std::vector<unsigned int> x_z_len;

    // Generate Random Seed According to time;
    srand(time(0) % 10000);

    // Check for Testing Mode, stored in boolean 'mode'
    bool mode = NORMAL_MODE;
    std::string testMode = "-testmode";
    if (argc > 1) {
        if (argv[1] == testMode) {
            mode = TESTING_MODE;
        }
    }

    printStartText();

    // Menu option is intentionally left outside the regular range

    States currState = ST_MainMenu;

    // State machine for menus
    while (currState != ST_Exit && !std::cin.eof()) {
        // Check State Clause
        if (currState == ST_MainMenu) {

            printMainMenu();

            // Try to execute menu logic, exceptions when bad data is input
            try {
                int menuOption = 0;
                std::string tempStr = "";
                std::cin >> tempStr;

                // This structure should avoid values like 1.11 entering two
                // digits sequentially
                if (all_of(tempStr.begin(), tempStr.end(), ::isdigit)) {
                    try {
                        menuOption = stoi(tempStr);
                    } catch (const std::exception& e) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        throw std::invalid_argument("");
                    }
                } else {
                    throw std::invalid_argument("");
                }

                // Case Check
                if (menuOption == 1) {
                    currState = ST_GenerateMazeMenu;
                } else if (menuOption == 2) {
                    currState = ST_BuildMaze;
                } else if (menuOption == 3) {
                    currState = ST_SolveMazeMenu;
                } else if (menuOption == 4) {
                    currState = ST_TeamInfoMenu;
                } else if (menuOption == 5) {
                    currState = ST_Exit;
                } else {
                    throw std::invalid_argument("");
                }
            } catch (const std::invalid_argument& a) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageRange(1, 5);
            } catch (const std::exception& e) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageException();
            }

        } else if (currState == ST_GenerateMazeMenu) {

            printGenerateMazeMenu();

            // Try to execute menu logic, exceptions when bad data is input
            try {
                int menuOption = 0;
                std::string tempStr = "";
                std::cin >> tempStr;

                if (all_of(tempStr.begin(), tempStr.end(), ::isdigit)) {
                    menuOption = stoi(tempStr);
                } else {
                    throw std::invalid_argument("");
                }

                // 1) Read Maze From Terminal
                if (menuOption == 1) {

                    try {
                        p_maze.Clear_Maze();
                        if (p_maze.Has_Terrain()) {
                            p_maze.Restore_Terrain();
                            x_z_len.clear();
                            maze_vec.clear();
                        }
                        unsigned int xLen = 0;
                        unsigned int zLen = 0;
                        std::string tempstr = "";
                        std::string isDone = "";

                        printGenerateMazePrompt();

                        std::cin >> isDone;

                        if (isDone != "done") {
                            std::string error =
                                "Input does not match string 'done'";
                            throw error;
                        }

                        if (mode == TESTING_MODE) {
                            mc.setPlayerPosition(
                                mcpp::Coordinate(TP_X, TP_Y, TP_Z));
                        }

                        player_position = mc.getPlayerTilePosition();
                        p_maze.Set_Player_Build_Pos(player_position);

                        printEnterLW();

                        std::cin >> xLen >> zLen;

                        //  generate dummy vector
                        if (xLen % 2 == 0 || zLen % 2 == 0) {
                            std::string error =
                                "Both inputs must be odd integers";
                            throw error;
                        }
                        if (xLen < 3 || zLen < 3) {
                            std::string error = "Both length inputs must be "
                                                "integers greater than 2";
                            throw error;
                        }
                        if (xLen > 99 || zLen > 99) {
                            std::string error = "Both length inputs must be "
                                                "integers less than 100";
                            throw error;
                        }

                        currMaze->Set_Len(xLen, zLen);

                        // Start with 'Dummy' Vector similar to that at
                        // beginning of 'Generate' Function
                        std::vector<std::vector<char>> mazeVec(
                            zLen, std::vector<char>(xLen, '.'));

                        // Checkerboard opposite to 'Generate' with full walls.
                        for (unsigned int i = 0; i < zLen; i++) {
                            for (unsigned int j = 0; j < xLen; j++) {
                                if ((i % 2 == 0 && j % 2 == 0) ||
                                    (i == 0 || j == 0 || i == zLen - 1 ||
                                     j == xLen - 1)) {
                                    mazeVec.at(i).at(j) = 'x';
                                }
                            }
                        }

                        printEnterStruct();

                        // This line is required to use std::getline in this
                        // case
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');

                        for (unsigned int i = 0; i < zLen; i++) {
                            std::string tempmazestr;
                            std::vector<char> tempvec;
                            std::getline(std::cin, tempmazestr);
                            for (long unsigned int j = 0;
                                 j < tempmazestr.length(); j++) {
                                if (j < xLen) {
                                    tempvec.push_back(tempmazestr.at(j));

                                    /*
                                     * This basic check makes sure the input is
                                     * a valid maze tile, if not it will not
                                     * become part of the maze vector
                                     */
                                    if (tempvec.at(j) == 'x' ||
                                        tempvec.at(j) == '.') {
                                        mazeVec.at(i).at(j) = tempvec.at(j);
                                    }
                                }
                            }
                        }

                        printMazeReadSuccess();

                        printPrintingStart();

                        printBasePoint(player_position);
                        std::cout << "Structure: " << std::endl;
                        currMaze->Set_Maze(mazeVec);
                        currMaze->Set_Len(mazeVec.at(0).size(), mazeVec.size());
                        currMaze->Set_Mode(mode);
                        currMaze->Print();
                        printPrintingEnd();

                    } catch (const std::invalid_argument& a) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageRange(1, 3);
                    } catch (const std::exception& e) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageException();
                    } catch (const std::string& f) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageInput(f);
                    }

                    // 2) Generate Random Maze
                } else if (menuOption == 2) {
                    // Act different in testing mode
                    currMaze->Set_Mode(mode);
                    try {
                        p_maze.Clear_Maze();
                        if (p_maze.Has_Terrain()) {
                            p_maze.Restore_Terrain();
                            x_z_len.clear();
                            maze_vec.clear();
                        }
                        unsigned int xLen = 0;
                        unsigned int zLen = 0;
                        std::string tempstr = "";
                        std::string isDone = "";
                        printGenerateMazePrompt();

                        std::cin >> isDone;

                        if (isDone != "done") {
                            std::string error =
                                "Input does not match string 'done'";
                            throw error;
                        }

                        if (mode == TESTING_MODE) {
                            mc.setPlayerPosition(
                                mcpp::Coordinate(TP_X, TP_Y, TP_Z));
                        }

                        player_position = mc.getPlayerTilePosition();
                        p_maze.Set_Player_Build_Pos(player_position);

                        printEnterLW();

                        std::cin >> xLen >> zLen;

                        if (xLen % 2 == 0 || zLen % 2 == 0) {
                            std::string error =
                                "Both inputs must be odd integers";
                            throw error;
                        }
                        if (xLen < 3 || zLen < 3) {
                            std::string error = "Both length inputs must be "
                                                "integers greater than 2";
                            throw error;
                        }
                        if (xLen > 99 || zLen > 99) {
                            std::string error = "Both length inputs must be "
                                                "integers less than 100";
                            throw error;
                        }

                        currMaze->Set_Len(xLen, zLen);

                        currMaze->Generate();
                        std::cout << "Maze generated successfully" << std::endl;
                        printPrintingStart();
                        printBasePoint(player_position);
                        std::cout << "Structure: " << std::endl;
                        std::vector<std::vector<char>> mazeVec;
                        currMaze->Print();
                        printPrintingEnd();

                    } catch (const std::invalid_argument& a) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageRange(1, 3);
                    } catch (const std::exception& e) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageException();
                    } catch (const std::string& f) {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        printErrorMessageInput(f);
                    }

                    // Back
                } else if (menuOption == 3) {
                    currState = ST_MainMenu;
                } else {
                    throw std::invalid_argument("");
                }
            } catch (const std::invalid_argument& a) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageRange(1, 3);
            } catch (const std::exception& e) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageException();
            }

            currState = ST_MainMenu;

        } else if (currState == ST_BuildMaze) {
            printPrintingStart();
            std::cout << "Structure: " << std::endl;
            std::vector<std::vector<char>> mazeVec = currMaze->Get_Maze();
            currMaze->Print();
            printPrintingEnd();

            if (p_maze.Has_Terrain()) {
                p_maze.Restore_Terrain();
                x_z_len.clear();
                maze_vec.clear();
            }

            x_z_len = currMaze->Get_Len();
            maze_vec = currMaze->Get_Maze();

            std::vector<std::vector<char>> maze_vec = currMaze->Get_Maze();

            p_maze.Load_Maze(maze_vec, x_z_len[0], x_z_len[1]);

            p_maze.Save_Terrain();

            p_maze.Flatten_Terrain();

            p_maze.Build_Maze();

            currState = ST_MainMenu;

        } else if (currState == ST_SolveMazeMenu) {

            printSolveMazeMenu();

            // Try to execute menu logic, exceptions when bad data is input
            try {
                int menuOption = 0;
                std::string tempStr = "";
                std::cin >> tempStr;

                if (all_of(tempStr.begin(), tempStr.end(), ::isdigit)) {
                    menuOption = stoi(tempStr);
                } else {
                    throw std::invalid_argument("");
                }

                mcpp::Coordinate alterPlayerPos(0, 1, 0);
                // Case Check
                if (menuOption == 1) {
                    // Act different in testing mode
                    if (mode == TESTING_MODE) {
                        if (currMaze->Get_Maze().empty()) {
                            printErrorMessageNoMaze();
                        } else {
                            if (currMaze->getZLen() == 0 ||
                                currMaze->getXLen() == 0) {
                                printErrorMessageInvalidMazeSize();
                            } else {
                                bool validMaze = Solve_Maze::placePlayerInMaze(
                                    currMaze->Get_Maze(),
                                    p_maze.Get_Player_Build_Pos() +
                                        alterPlayerPos,
                                    currMaze->getZLen(), currMaze->getXLen(),
                                    mode);
                                if (!validMaze) {
                                    printErrorMazeNoSpace();
                                } else {
                                    printReadyToSolve();
                                }
                            }
                        }
                    } else {
                        if (currMaze->Get_Maze().empty()) {
                            printErrorMessageNoMaze();
                        } else {
                            if (currMaze->getZLen() == 0 ||
                                currMaze->getXLen() == 0) {
                                printErrorMessageInvalidMazeSize();
                            } else {
                                Solve_Maze::placePlayerInMaze(
                                    currMaze->Get_Maze(),
                                    p_maze.Get_Player_Build_Pos() +
                                        alterPlayerPos,
                                    currMaze->getZLen(), currMaze->getXLen(),
                                    mode);
                                printReadyToSolve();
                            }
                        }
                    }

                } else if (menuOption == 2) {
                    // Act different in testing mode
                    if (mode == TESTING_MODE) {
                        if (currMaze->Get_Maze().empty()) {
                            printErrorMessageNoMaze();
                        } else {
                            if (currMaze->getZLen() == 0 ||
                                currMaze->getXLen() == 0) {
                                printErrorMessageInvalidMazeSize();
                            } else {
                                bool isInMaze = Solve_Maze::showEscapeRoute(
                                    currMaze->getZLen(), currMaze->getXLen(),
                                    p_maze.Get_Player_Build_Pos() +
                                        alterPlayerPos,
                                    currMaze->Get_Maze(), mode);
                                if (!isInMaze) {
                                    printErrorPlayerNotInMaze();
                                }
                            }
                        }
                    } else {

                        if (currMaze->Get_Maze().empty()) {
                            printErrorMessageNoMaze();
                        } else {
                            if (currMaze->getZLen() == 0 ||
                                currMaze->getXLen() == 0) {
                                printErrorMessageInvalidMazeSize();
                            } else {
                                bool isInMaze = Solve_Maze::showEscapeRoute(
                                    currMaze->getZLen(), currMaze->getXLen(),
                                    p_maze.Get_Player_Build_Pos() +
                                        alterPlayerPos,
                                    currMaze->Get_Maze(), mode);
                                if (!isInMaze) {
                                    printErrorPlayerNotInMaze();
                                }
                            }
                        }
                    }

                } else if (menuOption == 3) {
                    currState = ST_MainMenu;
                } else {
                    throw std::invalid_argument("");
                }
            } catch (const std::invalid_argument& a) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageRange(1, 3);
            } catch (const std::exception& e) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                printErrorMessageException();
            }

        } else if (currState == ST_TeamInfoMenu) {

            printTeamInfo();

            currState = ST_MainMenu;

            // Part of the generate maze menu
        } else {
            currState = ST_MainMenu;
        }
    }

    p_maze.Restore_Terrain();
    printExitMessage();

    return EXIT_SUCCESS;
}
