#include <iostream>

void printStartText(void) {
    std::cout << std::endl;
    std::cout << "Welcome to MineCraft MazeRunner!" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void printMainMenu(void) {
    std::cout << std::endl;
    std::cout << "------------- MAIN MENU -------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Build Maze in MineCraft" << std::endl;
    std::cout << "3) Solve Maze" << std::endl;
    std::cout << "4) Show Team Information" << std::endl;
    std::cout << "5) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printGenerateMazeMenu(void) {
    std::cout << std::endl;
    std::cout << "------------- GENERATE MAZE -------------" << std::endl;
    std::cout << "1) Read Maze from terminal" << std::endl;
    std::cout << "2) Generate Random Maze" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printGenerateMazePrompt(void) {
    std::cout << "In Minecraft, navigate to where you need the maze"
              << std::endl
              << "to be built in Minecraft and type - done:";
    std::cout << std::endl;
}

void printEnterLW(void) {
    std::cout << "Enter the length and width of maze:";
    std::cout << std::endl;
}

void printEnterStruct(void) {
    std::cout << "Enter the maze structure:";
    std::cout << std::endl;
}

void printMazeReadSuccess(void) {
    std::cout << "Maze read successfully";
    std::cout << std::endl;
}

void printPrintingStart(void) {
    std::cout << "**Printing Maze**";
    std::cout << std::endl;
}

void printPrintingEnd(void) {
    std::cout << "**End Printing Maze**";
    std::cout << std::endl;
}

void printReadyToSolve(void) {
    std::cout << "Maze ready to Solve...";
    std::cout << std::endl;
}

void printSolveMazeMenu(void) {
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printTeamInfo(void) {
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;
    std::cout << "\t [1] Erfan Samandarian (s4089117@student.rmit.edu.au)"
              << std::endl;
    std::cout << "\t [2] Liam Moore (s4095280@student.rmit.edu.au)"
              << std::endl;
    std::cout << "\t [3] Thomas Castanelli (s4072172@student.rmit.edu.au)"
              << std::endl;
    std::cout << std::endl;
}

void printExitMessage(void) {
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
    std::cout << std::endl;
}

void printErrorMessageException(void) {
    std::cout << "Error: Unknown Exception";
    std::cout << std::endl;
}

void printErrorMessageInput(std::string errorStr) {
    std::cout << "Input Error: " << errorStr;
    std::cout << std::endl;
}

void printErrorMessageRange(int in1, int in2) {
    std::cout << "Input Error: Enter a number between ";
    std::cout << in1;
    std::cout << " and ";
    std::cout << in2;
    std::cout << " ....";
    std::cout << std::endl;
}

// error if entered length and width (i.e. 3 and 5) are different to entered
// maze dimensions.
void printErrorMessageArraySizeIncorrect(int length, int width) {
    std::cout << "Input Error: Entered length (" << length << ") and width ("
              << width << ") differs from manually entered maze's dimensions";
    std::cout << std::endl;
}

void printErrorMessageNoMaze() {
    std::cout << "Input Error: No available maze to solve";
    std::cout << std::endl;
}

void printErrorMessageInvalidMazeSize() {
    std::cout
        << "Input Error: At least one maze dimension has not been entered";
    std::cout << std::endl;
}

void printErrorPlayerNotInMaze() {
    std::cout << "Input Error: Player is not in maze";
    std::cout << std::endl;
}

// probably won't ever happen but eh
void printErrorMazeNoSpace() {
    std::cout << "Input Error: Maze has no open space";
    std::cout << std::endl;
}

void printBasePoint(mcpp::Coordinate playerPos) {
    std::cout << "BasePoint: " << playerPos << std::endl;
}