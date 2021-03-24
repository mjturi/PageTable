# PageTable
This project simulates a pagetable for virtual memory storage, using a 32 bit address space

## Usage
./pagetable -n [number of addresses to process] -o [output mode] [level bits]

#### Output Modes
- summary
  - will print out stats after the program finishes parsing addresses
  - stats include:
    - page size (bytes)
    - addresses processed
    - hits 
    - misses
    - frames allocated
    - bytes used (total)
  - Example:
    - Page size: 65536 bytes 
      Addresses processed: 100000
      Hits: 99253 (99.25%), Misses 747 (0.75%)
      Frames allocated: 747
      Bytes used:  150408
- page2frame
  - will print out each level and the frame number allocated for each logical address
  - Example:
    - 0041F760: 0 0 41F -> 0
      0041F780: 0 0 41F -> 0
      0041F740: 0 0 41F -> 0
      11F5E2C0: 1 1 F5E -> 1
      05E78900: 0 5 E78 -> 2
      13270900: 1 3 270 -> 3
      004758A0: 0 0 475 -> 4
      004A30A0: 0 0 4A3 -> 5
      0049E110: 0 0 49E -> 6
      0049E160: 0 0 49E -> 6
- logical2offset
  - will print out the offset for each logical address
  - Example:
    - 0041F760 -> 00000760
      0041F780 -> 00000780
      0041F740 -> 00000740
      11F5E2C0 -> 000002C0
      05E78900 -> 00000900
      13270900 -> 00000900
      004758A0 -> 000008A0
      004A30A0 -> 000000A0
      0049E110 -> 00000110
      0049E160 -> 00000160
- logical2physical
  - will print out the physical address corresponding to each logical address
  - Example: 
    - 0041F760 -> 00000000
      0041F780 -> 00000010
      0041F740 -> 00000020
      11F5E2C0 -> 00000030
      05E78900 -> 00000040
      13270900 -> 00000050
      004758A0 -> 00000060
      004A30A0 -> 00000070
      0049E110 -> 00000080
      0049E160 -> 00000090
- bitmasks
  - will print out the bitmasks for the pagetable, then exit
  - Example:
    - level 0 mask F0000000
      level 1 mask 0F000000
      level 2 mask 00FFF000


## Flow
1. Parse arguments and verify correctness
2. Initialize the page table given user input levels
3. Read addresses and insert into the table if the entry has not already been created
  - if the entry was created, increment a hit counter
  - else, create a new entry and increment the frame number
