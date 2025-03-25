[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Jwdk2u8p)
# cosc2804-assignment3-template

## Team Allocation

|              | Member 1 (Tom Castanelli) [s4072172]         | Member 2 (Liam Moore) [s4095280]             | Member 3 (Erfan Samandarian) [s4089117]      |
|--------------|----------------------------------------------|----------------------------------------------|----------------------------------------------|
| Testing      | Test to cover Member 1's and Member 2's work | Test to cover Member 2's and Member 3's work | Test to cover Member 3's and Member 1's work |
| Base Program | Maze Generation                              | Maze Solving                                 | Build maze & Cleaning the world              |
| Enhancements | E3                                           | E1                                           | E2                                           |

## Team Video Link
https://drive.google.com/file/d/1XGCHKvxCXyFYM-tzKQ7rC1j0PZlwhJqQ/view?usp=sharing

## Team Progress

### Toms's Commits 

* Created basic Menu structure and navigation
* Created Maze class and populated with setters/getters, data etc.
* Implemented Test Mode check
* Implemented Import Maze from terminal
* Implemented Random Maze Generation
* Implemented Testing Mode for Maze Generation
* Implemented Enhancement
* Created Check_Adj Method
* Created Print Method

### Liam's Commits 

* Created test_invalidCharacterInput.input and expout files
* Created test_buildMaze.input and expout files
* Creatde test_mazeSolver.input and expout files
* Implemented functionality to find open blocks in maze to randomly teleport the player into
* Implemented findExitCoords() to get the exit coords of the maze for use in multiple functions
* Implemented maze solver using right-hand wall follower algorithm to show user the exit path
* Implemented testing mode for placing player in maze and solving maze using RHWF algorithm
* (E1) Implemented functionality to generate random mazes which take pre-existing terrain into
       account when generating their structure.
* (E1) Altered "Solve maze manually" and "Show Escape Route" functions to account for uneven terrain

### Erfan's Commits 

* Created test_generateMaze.input and expout file
* Created Build_Agent to contain type int values
* Created Block_Array::Vector to contain type int values
* Refactored Block_Array::Vector to use template for general data types
* Refactored Save_Terrain() to use getBlocks for heap connection + speed
* (E2) Implemented breadth first search for enhancement in my fork
* (E2) Implemented test for find shortest path enhancement in the fork
* Changed Restore_Terarin() to clear maze floor first to prevent falling sand or gravel blocks

## Assumptions

* The generated maze's length and width will not exceed 99.
* Blocks with temporary or invalid properties (e.g., water, leaves, lilypads, flowers) will be replaced with a brick block.
* If the player inputs a maze with too many characters, the excess input will be trimmed.
