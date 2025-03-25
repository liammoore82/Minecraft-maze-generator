#ifndef BLOCK_ARRAY_H
#define BLOCK_ARRAY_H

#include <mcpp/block.h>

#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_Z 0
#define DEFAULT_BLOCK mcpp::Blocks::AIR

/** Represents a block with coordinates and type */
struct Block {
    int x = DEFAULT_X;
    int y = DEFAULT_Y;
    int z = DEFAULT_Z;
    mcpp::BlockType block_type = DEFAULT_BLOCK;
};

#endif // BLOCK_ARRAY_H
