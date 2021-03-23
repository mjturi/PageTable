//Matthew Turi
//ID: 822202323
//03/23/21

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  /* define bool */
#include <vector>
#include "../include/byutr.h"
#include "../include/output_helpers.h"
#include "../include/helper_methods.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <cstring>


// globals initialized
int FrameNumber = 0;
int hit_count = 0;
unsigned int ins_frame = 0;
std::vector<int> pagemap;


Map * PageLookup(PageTable *PageTable, unsigned int LogicalAddress, bool p2f){
    // returns null unless the address has been previously mapped
    Level* curr_level = PageTable->RootNodePtr;
    Map * curr_map;
    if (p2f){
        pagemap.clear(); // for output use
    }
    for (int i =0; i < PageTable->BitMaskAry.size(); i++){
        unsigned int page = LogicalToPage(LogicalAddress, PageTable->BitMaskAry[i], PageTable->ShiftAry[i]); // get page per level
        if (i == PageTable->BitMaskAry.size()-1){ // if we are at the leaf node
             curr_map = curr_level->MapPtr;
            if (curr_map == NULL)
                return NULL;
            pagemap.push_back(page); // for output help
            if (curr_map->valid[page] != 1) // valid check
                return NULL;
            ins_frame = curr_map->frame[page]; // for output help
            return curr_map;
        }
        curr_level = curr_level->NextLevelPtr[page]; // not at a leaf node
        if (curr_level == NULL){
            return NULL;
        }
        else{
            pagemap.push_back(page); // for output help
        }
    }
    return NULL;
}

void PageInsert(PageTable *PageTable, unsigned int LogicalAddress, int Frame, bool p2f){
    Level* curr_level = PageTable->RootNodePtr;
    Level* temp_level = PageTable->RootNodePtr;
    Map* curr_map = nullptr;
    if (p2f){
        pagemap.clear(); // for output help
    }
    for (int i =0; i < PageTable->BitMaskAry.size(); i++){
        unsigned int page = LogicalToPage(LogicalAddress, PageTable->BitMaskAry[i], PageTable->ShiftAry[i]); // get page for each level
        if (i == PageTable->BitMaskAry.size()-1){ // if at leaf node
            curr_level->NextLevelPtr.clear();
            if (curr_level->MapPtr == nullptr){ // if no map exists, create one
                Map *m = new(Map);
                m->frame = new int[PageTable->EntryCount[curr_level->DepthOfLevel]];
                m->valid = new int[PageTable->EntryCount[curr_level->DepthOfLevel]];
                m->frame[page] = Frame;
                m->valid[page] = 1;
                curr_level->MapPtr = m;
                pagemap.push_back(page);
            }
            else { // otherwise just insert into that map
                curr_map = temp_level->MapPtr;
                int * vptr = curr_map->valid;
                int * fptr = curr_map->frame;
                if (curr_map->frame == NULL or curr_map->valid == NULL){ // check if it was initialized
                    curr_map->frame = new int[PageTable->EntryCount[0]];
                    curr_map->valid = new int[PageTable->EntryCount[0]];
                }
                curr_map->valid[page] = 1;
                fptr[page] = Frame;
                pagemap.push_back(page); // for output help
            }
            return;
        }
        temp_level = curr_level;
        curr_level = curr_level->NextLevelPtr[page];
        if (curr_level == NULL){ // if no level exists, then make one
            temp_level->NextLevelPtr[page] = nullptr;
            Level *l = new(Level);
            l->DepthOfLevel = temp_level->DepthOfLevel + 1;
            l->pageTable = PageTable;
            std::vector<Level *> temp(PageTable->EntryCount[temp_level->DepthOfLevel], NULL);
            l->NextLevelPtr = temp;
            temp_level->NextLevelPtr[page] = l;
            l->MapPtr = nullptr;
            curr_level = temp_level->NextLevelPtr[page];
            temp_level = curr_level;
            pagemap.push_back(page);
        }
        else{ // otherwise, continue traversing
            temp_level = temp_level->NextLevelPtr[page];
            pagemap.push_back(page); // for output help
            continue;
        }
    }
}

std::vector<unsigned int> createBitMask(std::vector<int> level_bits){
    int offset = 0;
    std::vector<unsigned int> bitmask;
    for (int i : level_bits){
        std::string bin = "00000000000000000000000000000000"; // string holding "empty" bitmask
        for (int j = 0; j < i; j++){ // for each level, keep track of prev offset and set the corresponding bits
            bin.at(offset) = '1';
            offset += 1;
        }
        std::bitset<32> set(bin); // string to unsigned int conversion
        std::stringstream output;
        output << "0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << set.to_ulong();
        unsigned int temp = StrtoInt(output.str());
        bitmask.push_back(temp);
    }
    return bitmask;
}

std::vector<int> createShiftAry(std::vector<int> level_bits){
    int sum = 0;
    std::vector<int> sa;
    for (int i : level_bits){ // traverse each level, and append to shift vector
        sum += i;
        sa.push_back(32-sum);
    }
    return sa;
}

std::vector<int> createEntryCount(std::vector<int> level_bits){
    std::vector<int> ec;
    for (int i : level_bits){ // traverse each level, and append to entry count vector
        int curr = pow(2, i);
        ec.push_back(curr);
    }
    return ec;
}

PageTable * init(std::vector<int> level_bits){
    // first, call all helper setup methods
    std::vector<int> ec = createEntryCount(level_bits);
    std::vector<int> sa = createShiftAry(level_bits);
    std::vector<unsigned int> bma = createBitMask(level_bits);
    // This method will be called once we know our user params for the pagetable
    PageTable *pt = new(PageTable);
    pt->LevelCount = level_bits.size();
    pt->BitMaskAry = bma;
    pt->ShiftAry = sa;
    pt->EntryCount = ec;

    // now we initialize a level struct
    // ONLY INIT THE NEXTLVLPTR IF THE LEVEL_BITS>SIZE > 1...else do that for the map
    Level *l0 = new(Level);
    l0->DepthOfLevel = 0;
    l0->pageTable = pt;
    pt->RootNodePtr = l0;
    if (level_bits.size() > 1){
        std::vector<Level *> temp(ec[0], NULL); // array of ec[0] size (2 ^ level 0 bits), all vals set to null
        l0->NextLevelPtr = temp;
        // there will be no MapPtr initialization because it is the first level...
    }
    else {
        // MAP, with arrays initialized...
        Map * m = new(Map);
        int * test = new int[pt->EntryCount[0]];
        int * test2 = new int[pt->EntryCount[0]];
        m->frame = test;
        m->valid = test2;
        l0->MapPtr = m;
    }
    return pt;
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift) {
    unsigned int page = LogicalAddress & Mask; // bitwise and
    page = page >> Shift; // right shift
    return page;
}

