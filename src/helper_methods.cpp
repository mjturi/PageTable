//Matthew Turi
//ID: 822202323
//03/23/21

#include "../include/helper_methods.h"
#include <math.h>
#include <sstream>
#include <iomanip>

unsigned int RecBytes(Level * curr_level){
    unsigned int size = 0;
    if (curr_level->NextLevelPtr.size() <= 2147483648) { //check if it was initialized correctly, or if it is null
        for (int j = 0; j < curr_level->NextLevelPtr.size(); j++) { // traverse for each index of the nextlvl ptr
            if (curr_level->NextLevelPtr[j] != NULL) { // if its initialized, perform size calcs recursively, else continue
                size += sizeof(int);
                size += sizeof(curr_level->MapPtr);
                size += sizeof(curr_level->pageTable);
                size += sizeof(std::vector<Level *>) + (sizeof(Level *) * curr_level->NextLevelPtr.size());
                size += RecBytes(curr_level->NextLevelPtr[j]);
                // call this ^^^ helper func again with the next level, do this until all levels are NULL
            }
        }
    }
    if (curr_level->MapPtr != nullptr){ // traverse maps, and add their size
        size += (sizeof(int) * curr_level->pageTable->EntryCount[curr_level->DepthOfLevel]);
        size += (sizeof(int) * curr_level->pageTable->EntryCount[curr_level->DepthOfLevel]);
    }
    return size;
}

unsigned int numBytesTotal(PageTable * pt){
    Level* curr_level = pt->RootNodePtr;
    unsigned int size = 0;
    // initial calcs for size of PageTable struct
    size += sizeof(pt->LevelCount);
    size += sizeof(pt->RootNodePtr);
    unsigned int temp = sizeof(std::vector<int>) + (sizeof(int) * pt->EntryCount.size());
    size += temp;
    temp = sizeof(std::vector<int>) + (sizeof(int) * pt->ShiftAry.size());
    size += temp;
    temp = sizeof(std::vector<unsigned int>) + (sizeof(unsigned int) * pt->BitMaskAry.size());
    size += temp;
    // Then, call RecBytes to traverse the actual tree
    size += RecBytes(curr_level);
    return size;
}

unsigned int getPageSize(std::vector<int> levels){
    int bit_sum = 0;
    for (int i : levels){
        bit_sum += i;
    }
    bit_sum = 32 - bit_sum; // address size - total level bits
    unsigned int page_size = pow(2, bit_sum);
    return page_size;
}

unsigned int LogicalToPhysical(unsigned int frame_number, std::vector<int> level_bits, unsigned int offset){
    unsigned int page_size = getPageSize(level_bits);
    unsigned int physical = frame_number * page_size;
    physical += offset;
    return physical;
}

unsigned int StrtoInt(std::string hex){
    unsigned int temp;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> temp;
    return temp;
}

std::string LogicalToOffset(unsigned int LogicalAddress, std::vector<int> levels){
    int bit_sum = 0;
    for (int i : levels){
        bit_sum += i;
    }
    // with the bit_sum, perform a left then right shift to set all values besides the offset to 0
    LogicalAddress = LogicalAddress << bit_sum;
    LogicalAddress = LogicalAddress >> bit_sum;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(8) << std::hex << LogicalAddress; // pad and convert to string
    std::string ret = ss.str();
    return ret;
}

