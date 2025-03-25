#include "Terrain_Array.h"

#define INITIAL_CAPACITY 10

/* Constructor: Initialize block array */
Terrain_Array::Terrain_Array() : block_count(0), capacity(INITIAL_CAPACITY) {
    blocks = new Block[capacity];
}

/* Destructor: Clear and delete block array */
Terrain_Array::~Terrain_Array() {
    clear();
    delete[] blocks;
}

/* Add block to array, resize if needed */
void Terrain_Array::add_block(int x, int y, int z, mcpp::BlockType block) {
    if (block_count == capacity) {
        resize();
    }
    blocks[block_count++] = {x, y, z, block};
}

/* Get block at index, throw if out of range */
Block Terrain_Array::get_block(int index) const {
    if (index < 0 || index >= block_count) {
        throw std::out_of_range("\033[31mIndex out of range\033[0m");
    }
    return blocks[index];
}

/* Return number of blocks */
int Terrain_Array::size() const { return block_count; }

/* Double array capacity */
void Terrain_Array::resize() {
    capacity *= 2;
    Block* new_blocks = new Block[capacity];
    for (int i = 0; i < block_count; ++i) {
        new_blocks[i] = blocks[i];
    }
    delete[] blocks;
    blocks = new_blocks;
}

/* Print all blocks */
void Terrain_Array::print() const {
    for (int i = 0; i < block_count; ++i) {
        std::cout << "Block " << i << ": (" << blocks[i].x << ", "
                  << blocks[i].y << ", " << blocks[i].z << ") - "
                  << blocks[i].block_type << std::endl;
    }
}

/* Clear all blocks */
void Terrain_Array::clear() {
    for (int i = 0; i < block_count; ++i) {
        blocks[i].~Block();
    }
    block_count = 0;
}
