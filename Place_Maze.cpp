#include "Place_Maze.h"
#include <algorithm>
#include <vector>

#define TERRAIN_HEIGHT_OFFSET 2
#define TERRAIN_WIDTH_OFFSET 3
#define TERRAIN_DEPTH_OFFSET 1
#define SLEEP_DURATION_MS 50
#define MAZE_HEIGHT 3
#define CARPET_HEIGHT_OFFSET 2
#define SUPPORT_BLOCK_HEIGHT_OFFSET 1

/* Save terrain blocks to array */
void Place_Maze::Save_Terrain() {
    mcpp::Coordinate temp = player_position;

    mcpp::Coordinate first_corner(temp.x, temp.y - TERRAIN_HEIGHT_OFFSET,
                                  temp.z);
    mcpp::Coordinate second_corner(
        temp.x + length - 1, temp.y + TERRAIN_WIDTH_OFFSET, temp.z + width - 1);

    mcpp::Chunk terrain_chunk = mc.getBlocks(first_corner, second_corner);

    mcpp::BlockType block_type;
    mcpp::Coordinate coord;

    /* Only put non-air blocks into the terrain array */
    for (int x = 0; x < terrain_chunk.x_len(); ++x) {
        for (int y = 0; y < terrain_chunk.y_len(); ++y) {
            for (int z = 0; z < terrain_chunk.z_len(); ++z) {
                block_type = terrain_chunk.get(x, y, z);
                if (block_type != mcpp::Blocks::AIR) {
                    coord = terrain_chunk.base_pt() + mcpp::Coordinate(x, y, z);
                    terrain.add_block(coord.x, coord.y, coord.z, block_type);
                }
            }
        }
    }
}

/* Restore terrain from saved blocks */
void Place_Maze::Restore_Terrain() {
    mcpp::Coordinate temp = player_position;
    mcpp::Coordinate first_corner(temp.x, temp.y - TERRAIN_DEPTH_OFFSET,
                                  temp.z);
    mcpp::Coordinate second_corner(temp.x + length - 1,
                                   temp.y + TERRAIN_WIDTH_OFFSET + 1,
                                   temp.z + width - 1);

    mcpp::Chunk terrain_chunk = mc.getBlocks(first_corner, second_corner);

    /* Clear the second bottom row first */
    for (int x = 0; x < terrain_chunk.x_len(); ++x) {
        for (int z = 0; z < terrain_chunk.z_len(); ++z) {
            if (terrain_chunk.get(x, TERRAIN_DEPTH_OFFSET, z) !=
                mcpp::Blocks::AIR) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(SLEEP_DURATION_MS));
                mc.setBlock(terrain_chunk.base_pt() +
                                mcpp::Coordinate(x, TERRAIN_DEPTH_OFFSET, z),
                            mcpp::Blocks::AIR);
            }
        }
    }

    /* Clear the last y-axis */
    for (int x = 0; x < terrain_chunk.x_len(); ++x) {
        for (int z = 0; z < terrain_chunk.z_len(); ++z) {
            if (terrain_chunk.get(x, terrain_chunk.y_len() - 1, z) !=
                mcpp::Blocks::AIR) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(SLEEP_DURATION_MS));
                mc.setBlock(
                    terrain_chunk.base_pt() +
                        mcpp::Coordinate(x, terrain_chunk.y_len() - 1, z),
                    mcpp::Blocks::AIR);
            }
        }
    }

    /* Clear the rest of the blocks */
    for (int x = 0; x < terrain_chunk.x_len(); ++x) {
        for (int y = 0; y < terrain_chunk.y_len(); ++y) {
            for (int z = 0; z < terrain_chunk.z_len(); ++z) {
                if (terrain_chunk.get(x, y, z) != mcpp::Blocks::AIR) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(SLEEP_DURATION_MS));
                    mc.setBlock(terrain_chunk.base_pt() +
                                    mcpp::Coordinate(x, y, z),
                                mcpp::Blocks::AIR);
                }
            }
        }
    }

    /* Restore (member variable) terrain saved */
    for (auto it = terrain.begin(); it != terrain.end(); ++it) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(SLEEP_DURATION_MS));
        mc.setBlock(mcpp::Coordinate(it->x, it->y, it->z), it->block_type);
    }

    /* Load carpet and supporting blocks */
    mc.setBlock(carpet_coord, carpet_block);
    mc.setBlock(one_below_carpet_coord, one_below_carpet_block);
    mc.setBlock(two_below_carpet_coord, two_below_carpet_block);
    mc.setBlock(one_above_carpet_coord, one_above_carpet_block);
    mc.setBlock(two_above_carpet_coord, two_above_carpet_block);
}

/* Flatten terrain under player */
void Place_Maze::Flatten_Terrain() {
    mcpp::Coordinate temp = player_position;

    /* Retrieve the block ID beneath the player's position and store it */
    player_position_block =
        (std::find(odd_blocks.begin(), odd_blocks.end(),
                   mc.getBlock(player_position)) != odd_blocks.end())
            ? mcpp::Blocks::BRICKS
            : mc.getBlock(player_position);

    for (int y = temp.y - TERRAIN_DEPTH_OFFSET;
         y >= temp.y - TERRAIN_DEPTH_OFFSET; --y) {
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
                if (std::find(odd_blocks.begin(), odd_blocks.end(),
                              mc.getBlock(mcpp::Coordinate(temp.x + i, y,
                                                           temp.z + j))) !=
                    odd_blocks.end()) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(SLEEP_DURATION_MS));
                    mc.setBlock(mcpp::Coordinate(temp.x + i, y, temp.z + j),
                                mcpp::Blocks::BEDROCK);
                }
            }
        }
    }

    for (int y = temp.y; y >= temp.y; --y) {
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(SLEEP_DURATION_MS));
                mc.setBlock(mcpp::Coordinate(temp.x + i, y, temp.z + j),
                            player_position_block);
            }
        }
    }

    temp = player_position;

    for (int y = temp.y + 1; y <= temp.y + TERRAIN_WIDTH_OFFSET + 1; ++y) {
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(SLEEP_DURATION_MS));
                mc.setBlock(mcpp::Coordinate(temp.x + i, y, temp.z + j),
                            mcpp::Blocks::AIR);
            }
        }
    }
}

/* Load maze blocks from vector */
void Place_Maze::Load_Maze(std::vector<std::vector<char>> maze_vec, int length,
                           int width) {
    this->length = length;
    this->width = width;

    int z = 0;

    for (const auto& line : maze_vec) {
        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == 'x') {
                for (int y = 0; y < MAZE_HEIGHT; ++y) {
                    maze.add_block(x, y, z, mcpp::Blocks::ACACIA_WOOD_PLANK);
                }
            }
        }
        ++z;
    }
}

/* Build maze from saved blocks */
void Place_Maze::Build_Maze() {
    mcpp::Coordinate temp = player_position;
    temp.y += 1;

    for (auto it = maze.begin(); it != maze.end(); ++it) {
        mcpp::Coordinate pt = temp + mcpp::Coordinate(it->x, it->y, it->z);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(SLEEP_DURATION_MS));
        mc.setBlock(pt, it->block_type);
    }

    temp = player_position;
    temp.y += 1;

    carpet_coord = Solve_Maze::findExitCoords(width, length, temp);

    mcpp::Coordinate alterCoords(1, 0, 1);
    carpet_coord = carpet_coord - alterCoords;

    if (maze.size() > 0) {
        Place_Carpet();
    }
}

/* Check if terrain is saved */
bool Place_Maze::Has_Terrain() {
    bool has_terrain = (terrain.size() > 0) ? true : false;
    return has_terrain;
}

/* Clear maze blocks */
void Place_Maze::Clear_Maze() { maze.clear(); }

/* Print maze size */
void Place_Maze::Print_Maze_Size() { std::cout << maze.size(); }

/* Get player build position */
mcpp::Coordinate Place_Maze::Get_Player_Build_Pos() { return player_position; }

/* Set player build position */
void Place_Maze::Set_Player_Build_Pos(mcpp::Coordinate player_pos) {
    this->player_position = player_pos;
}

/* Place carpet and supporting blocks */
void Place_Maze::Place_Carpet() {
    /* Block one above the upper block */
    two_above_carpet_coord = carpet_coord;
    two_above_carpet_coord.y += CARPET_HEIGHT_OFFSET;

    /* Block above the blue carpet */
    one_above_carpet_coord = carpet_coord;
    one_above_carpet_coord.y += SUPPORT_BLOCK_HEIGHT_OFFSET;

    /* Block under the blue carpet */
    one_below_carpet_coord = carpet_coord;
    one_below_carpet_coord.y -= SUPPORT_BLOCK_HEIGHT_OFFSET;

    /* Block under the supporting block */
    two_below_carpet_coord = one_below_carpet_coord;
    two_below_carpet_coord.y -= SUPPORT_BLOCK_HEIGHT_OFFSET;

    /* Save the blocks to restore them */
    carpet_block = mc.getBlock(carpet_coord);
    one_below_carpet_block = mc.getBlock(one_below_carpet_coord);
    two_below_carpet_block = mc.getBlock(two_below_carpet_coord);
    one_above_carpet_block = mc.getBlock(one_above_carpet_coord);
    two_above_carpet_block = mc.getBlock(two_above_carpet_coord);

    /* Set supporting block if necessary */
    if (std::find(odd_blocks.begin(), odd_blocks.end(),
                  two_below_carpet_block) != odd_blocks.end()) {
        mc.setBlock(two_below_carpet_coord, mcpp::Blocks::BEDROCK);
    }

    /* Set supporting block under carpet if necessary */
    if (std::find(odd_blocks.begin(), odd_blocks.end(),
                  one_below_carpet_block) != odd_blocks.end()) {

        if (std::find(odd_blocks.begin(), odd_blocks.end(),
                      player_position_block) != odd_blocks.end()) {
            mc.setBlock(one_below_carpet_coord, mcpp::Blocks::DIAMOND_BLOCK);
        } else {
            mc.setBlock(one_below_carpet_coord, player_position_block);
        }
    }

    /* Set blue carpet after supporting blocks placed */
    mc.setBlock(carpet_coord, mcpp::Blocks::BLUE_CARPET);

    /* Set upper air blocks after supporting carpet placed */
    mc.setBlock(one_above_carpet_coord, mcpp::Blocks::AIR);
    mc.setBlock(two_above_carpet_coord, mcpp::Blocks::AIR);
}
