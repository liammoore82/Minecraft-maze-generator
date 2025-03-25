#ifndef PLACE_MAZE_H
#define PLACE_MAZE_H

#include "Solve_Maze.h"
#include "Terrain_Array.h"

#include <mcpp/mcpp.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>

/** Blocks that should be ignored */
const std::vector<mcpp::BlockType> odd_blocks = {mcpp::Blocks::AIR,
                                                 mcpp::Blocks::STILL_WATER,
                                                 mcpp::Blocks::STILL_LAVA,
                                                 mcpp::Blocks::FLOWING_WATER,
                                                 mcpp::Blocks::FLOWING_LAVA,
                                                 mcpp::Blocks::LILY_PAD,
                                                 mcpp::Blocks::TALL_GRASS,
                                                 mcpp::Blocks::ACACIA_LEAVES,
                                                 mcpp::Blocks::ACACIA_SAPLING,
                                                 mcpp::Blocks::ACTIVATOR_RAIL,
                                                 mcpp::Blocks::BIRCH_LEAVES,
                                                 mcpp::Blocks::BIRCH_SAPLING,
                                                 mcpp::Blocks::CAKE_BLOCK,
                                                 mcpp::Blocks::COBWEB,
                                                 mcpp::Blocks::DARK_OAK_LEAVES,
                                                 mcpp::Blocks::DARK_OAK_SAPLING,
                                                 mcpp::Blocks::DEAD_BUSH,
                                                 mcpp::Blocks::DEAD_SHRUB,
                                                 mcpp::Blocks::DETECTOR_RAIL,
                                                 mcpp::Blocks::FLOWER_POT,
                                                 mcpp::Blocks::GRASS_PATH,
                                                 mcpp::Blocks::JUNGLE_LEAVES,
                                                 mcpp::Blocks::JUNGLE_SAPLING,
                                                 mcpp::Blocks::LARGE_FERN,
                                                 mcpp::Blocks::LILAC,
                                                 mcpp::Blocks::OAK_LEAVES,
                                                 mcpp::Blocks::OAK_SAPLING,
                                                 mcpp::Blocks::POWERED_RAIL,
                                                 mcpp::Blocks::RAIL,
                                                 mcpp::Blocks::ROSE_BUSH,
                                                 mcpp::Blocks::SPRUCE_LEAVES,
                                                 mcpp::Blocks::SUNFLOWER};

/** Manages the placement and restoration of a maze */
class Place_Maze {
  public:
    Place_Maze() {};

    /** Checks if terrain is available */
    bool Has_Terrain();
    /** Builds the maze */
    void Build_Maze();
    /** Flattens the terrain */
    void Flatten_Terrain();
    /** Loads the maze from a vector */
    void Load_Maze(std::vector<std::vector<char>> maze_vec, int length,
                   int width);
    /** Restores the original terrain */
    void Restore_Terrain();
    /** Saves the current terrain */
    void Save_Terrain();

    /** Gets the player's build position */
    mcpp::Coordinate Get_Player_Build_Pos();
    /** Sets the player's build position */
    void Set_Player_Build_Pos(mcpp::Coordinate player_pos);

    /** Clears the maze */
    void Clear_Maze();
    /** Prints the maze size */
    void Print_Maze_Size();

    /** Places carpet in the maze */
    void Place_Carpet();

  private:
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate player_position;

    int length = 0;
    int width = 0;

    Terrain_Array terrain;
    Terrain_Array maze;

    mcpp::BlockType player_position_block;

    /* Blocks for placement of carpet */
    mcpp::BlockType carpet_block;
    mcpp::BlockType one_below_carpet_block;
    mcpp::BlockType two_below_carpet_block;
    mcpp::BlockType one_above_carpet_block;
    mcpp::BlockType two_above_carpet_block;

    /* Coordinates for placement of carpet */
    mcpp::Coordinate carpet_coord;
    mcpp::Coordinate one_below_carpet_coord;
    mcpp::Coordinate two_below_carpet_coord;
    mcpp::Coordinate one_above_carpet_coord;
    mcpp::Coordinate two_above_carpet_coord;
};

#endif
