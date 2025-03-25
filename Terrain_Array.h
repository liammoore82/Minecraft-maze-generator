#ifndef TERRAIN_ARRAY_H
#define TERRAIN_ARRAY_H

#include "Block.h"

#include <iostream>
#include <stdexcept>

/** Manages a dynamic array of Blocks */
class Terrain_Array {
  public:
    Terrain_Array();
    ~Terrain_Array();

    Terrain_Array(const Terrain_Array&) = delete;
    Terrain_Array& operator=(const Terrain_Array&) = delete;

    /** Adds a block at the specified coordinates */
    void add_block(int x, int y, int z, mcpp::BlockType block);
    /** Retrieves a block by index */
    Block get_block(int index) const;
    /** Returns the number of blocks */
    int size() const;

    Block* begin() const { return blocks; }
    Block* end() const { return blocks + block_count; }

    /** Prints all blocks */
    void print() const;

    /** Clears all blocks */
    void clear();

  private:
    /** Resizes the array when capacity is reached */
    void resize();
    Block* blocks;
    int block_count;
    int capacity;
};

#endif // TERRAIN_ARRAY_H
