//Matthew Turi
//ID: 822202323
//03/23/21

#include "pagetable.h"

#ifndef A3_HELPER_METHODS_H
#define A3_HELPER_METHODS_H
#include <vector>
#include <string>

/*
 * RecBytes
 * params: Level * to the current level we are calculating size of
 * Will recursively call itself until the entire tree has been traversed.
 * For each level, if it is a leaf node, will also traverse the map and calculate its size.
 * returns: integer holding a total size of all parts of the tree except the pagetable base structure
 */
unsigned int RecBytes(Level * curr_level);

/*
 * numBytesTotal
 * params: Pagetable * to our pagetable in use
 * Will first calculate the size of the base pagetable struct.
 * Then calls RecBytes with the RootNodePtr, and adds that return value to the size already calculated.
 * returns: unsigned integer holding the total size of the entire page tree
 */
unsigned int numBytesTotal(PageTable * pt);

/*
 * getPageSize
 * params: std::vector<int> containing user input values for the size of each level
 * Will calculate the sum of the user input level sizes, then subtract that from the preset address size (32)
 * returns: unsigned integer holding 2^(32-sum(level_bits)) which is the page size
 */
unsigned int getPageSize(std::vector<int> levels);

/*
 * LogicalToPhysical
 * params: unsigned int frame_number, std::vector<int> level_bits, unsigned int offset
 * Will first call getPageSize with the level_bits vector
 * Then, given the frame number for the current address, will calculate frame_number * page_size
 * Then adds the offset for the given address
 * returns: unsigned int holding the physical address
 */
unsigned int LogicalToPhysical(unsigned int frame_number, std::vector<int> level_bits, unsigned int offset);

/*
 * StrtoInt
 * params: std::string holding a hex string
 * Using stringstreams, converts the hex string to an unsigned int
 * returns: unsigned int holding decimal representation of the hex string
 */
unsigned int StrtoInt(std::string hex);

/*
 * LogicalToOffset
 * params: unsigned int LogicalAddress, std::vector<int> levels containing user input level params
 * Will calculate the sum of levels, then performs of a left then right shift of the logical address to that sum
 * Then converts this to a hex string with padding
 * returns: std::string holding the padded hex offset for the address
 */
std::string LogicalToOffset(unsigned int LogicalAddress, std::vector<int> levels);
#endif //A3_HELPER_METHODS_H

