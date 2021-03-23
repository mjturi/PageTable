//Matthew Turi
//ID: 822202323
//03/23/21

#include <stdio.h>
#include <vector>
#include "../include/output_helpers.h"


/*
 * report_logical2physical
 * Given a pair of numbers, output a line: 
 *      src -> dest  
 */
void report_logical2physical(unsigned int  src, unsigned int  dest) {
    fprintf(stdout, "%08X -> %08X\n", src, dest);
}

/*
 * report_logical2offselt
 * Given a logical address and its offset, output a line:
 *	logical -> offset
 */
void report_logical2offset(unsigned int  logical, unsigned int  offset) {
    fprintf(stdout, "%08X -> %08X\n", logical, offset);
}


/*
 * report_summary
 * Write out a mesasge that indicates summary information for the page table.
 * page_size - Number of bytes per page
 * hits - Number of times a page was mapped.
 * addreses - Number of addresses processed
 * frames_used - Number of frames allocated
 * bytes - Total number of bytes needed for data structure.  
 *	   Should include all levels, allocated arrays, etc.
 */
void report_summary(unsigned int page_size, unsigned int hits,
                    unsigned int addresses, unsigned int frames_used,
                    unsigned int bytes) {
    unsigned int misses;
    double hit_percent;

    printf("Page size: %d bytes\n", page_size);
    /* Compute misses (page faults) and hit percentage */
    misses = addresses - hits;
    hit_percent = (double) hits / addresses * 100.0;
    printf("Addresses processed: %d\n", addresses);
    printf("Hits: %d (%.2f%%), Misses %d (%.2f%%)\n",
           hits, hit_percent, misses, 100 - hit_percent);
    printf("Frames allocated: %d\n", frames_used);
    printf("Bytes used:  %d\n", bytes);
}

/*
 * report_bitmasks
 * Write out bitmasks.
 * levels - Number of levels
 * masks - Pointer to array of bitmasks
 */
void report_bitmasks(int levels, std::vector<unsigned int> bma) {
    printf("Bitmasks\n");
    for (int idx=0; idx < levels; idx++)
        /* show mask entry and move to next */
        printf("level %d mask %08X\n", idx, bma[idx]);
}

/*
 * report_pagemap
 * Write out page numbers and frame number
 *
 * logical_addr - logical address, displayed first (primarily to help
 *                with debugging)
 * levels - specified number of levels in page table
 * pages - pages[idx] is the page number associated with level idx
 *         (0 < idx < levels)
 * frame - page is mapped to specified frame
 */
void report_pagemap(unsigned int  logical_addr, int levels,
                    std::vector<int> pagemap, int frame_number) {
    printf("%08X: ", logical_addr);

    for (int idx=0; idx < levels; idx++)   /* output pages */
        printf("%X ", pagemap[idx]);

    printf("-> %X\n", frame_number); /* output frame */
}
    
