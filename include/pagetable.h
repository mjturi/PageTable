//Matthew Turi
//ID: 822202323
//03/23/21

#ifndef A3_PAGETABLE_H
#define A3_PAGETABLE_H

#include <vector>


// Global variables used throughout the pagetable implementation and accessed by these functions
extern int FrameNumber;
extern int hit_count;
extern unsigned int ins_frame;
extern std::vector<int> pagemap; // vector used to store where each address is entered

// Forward declarations
typedef struct Level Level;
typedef struct PageTable PageTable;

/*
 * Map
 * contains: two int pointers which will hold arrays for validity and frame
 */
typedef struct {int *valid; int* frame;} Map;

/*
 * Level
 * contains: level depth, pointer to the pagetable, a vector of level pointers, and a map ptr
 * Only one of the two (MapPtr or NextLevelPtr) will ever be initialized in a given level
 */
struct Level {int DepthOfLevel; PageTable *pageTable; std::vector<Level *> NextLevelPtr; Map *MapPtr;};

/*
 * PageTable
 * base structure for the entire project
 * contains: level count, bitmask vector, pointer to the first (root) level, shift vector, and entry count vector
 */
struct PageTable {int LevelCount; std::vector<unsigned int> BitMaskAry; Level *RootNodePtr; std::vector<int> ShiftAry; std::vector<int>EntryCount;};


/*
 * PageLookup
 * params: Pagetable * PageTable, unsigned int LogicalAddress, and bool p2f
 * p2f is used to know if we need to store where the address gets entered at each level of the table
 * Given a logical address, will calculate the page for each level using the LogicalToPage function
 * Then, traverses each level of the pagetable and checks whether or not the corresponding level exists and has been initialized
 * Traverses down to the Map level, where it will check if the page in the frame array is valid
 * returns: if valid, returns the Map * where it exists. Else NULL
 */
Map * PageLookup(PageTable *PageTable, unsigned int LogicalAddress, bool p2f);

/*
 * PageInsert
 * params: PageTable * PageTable, unsigned int LogicalAddress, int Frame, bool p2f
 * p2f is used to know if we need to store where the address gets entered at each level of the table
 * Given a logical address, and PageLookup returns null, will step through the same as the lookup function
 * If the corresponding Level/Map does not exist, will create it, initialize, and continue
 * Will finally insert into the Map with the given frame number
 * returns: void
 */
void PageInsert(PageTable *PageTable, unsigned int LogicalAddress, int Frame, bool p2f);

/*
 * createBitMask
 * params: std::vector<int> level_bits
 * Takes level_bits, (user entered level sizes), and creates the bitmasks for each level
 * Uses a string containing 32 binary 0s, and will adjust them to 1s for each level
 * Keeps track of the previous offset so it knows where to continue changing bits for the next level
 * returns: std::vector<unsigned int> containing decimal representation of the bitmasks for each level
 */
std::vector<unsigned int> createBitMask(std::vector<int> level_bits);

/*
 * createShiftAry
 * params: std::vector<int> level_bits
 * Takes level_bits, (user entered level sizes), and creates the shift arrays for each level
 * Just keeps a running sum of level_bits, and does 32 - running sum
 * returns: std::vector<int> containing the shift arrays for each level
 */
std::vector<int> createShiftAry(std::vector<int> level_bits);

/*
 * createEntryCount
 * params: std::vector<int> level_bits
 * Takes level_bits, (user entered level sizes), and creates the entry counts for each level
 * Calculates 2^i for each i in level_bits
 * returns: std::vector<int> containing entry counts per level
 */
std::vector<int> createEntryCount(std::vector<int> level_bits);

/*
 * init
 * params: std::vector<int> level_bits
 * Using the level_bits vector, calls the 3 above methods to initialize the bitmasks,shift arrays, and entry counts
 * Then creates a PageTable *, and fills the corresponding values
 * Then will either create a base Level that the RootNodePtr will point to (from the pagetable)
 * If it is a single level pagetable, will create and initialize a Map * with the entry count size, and update the pointer from the level
 * Otherwise, will initialize the first NextLvlPtr with size of EntryCount[0], and update the pointer from the level
 * returns: PageTable * (pointer to the newly created pagetable)
 */
PageTable * init(std::vector<int> level_bits);

/*
 * LogicalToPage
 * params: unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift
 * Given the logical address and the corresponding mask and shift values for that level, performs a bitwise and with the
 * mask and then shifts Shift bits right
 * returns: unsigned int with the page for the given level
 */
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
#endif //A3_PAGETABLE_H