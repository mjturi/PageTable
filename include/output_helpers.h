//Matthew Turi
//ID: 822202323
//03/23/21

#ifndef A3_OUTPUT_MODE_HELPERS_H
#define A3_OUTPUT_MODE_HELPERS_H

#endif


typedef struct {
    bool bitmasks;  /* display bitmaks */
    bool logical2physical;  /* show logical to physical address translation */
    bool page2frame;  /* show mapping between page numbers and frame */
    bool offset; /* show the offset of each address */
    bool summary; /* summary statistics */
} OutputOptionsType;

/*
 * report_logical2physical
 * Given a pair of numbers, output a line: 
 *      src -> dest  
 */
void report_logical2physical(unsigned int  src, unsigned int  dest);

/*
 * report_logical2offselt
 * Given a logical address and its offset, output a line:
 *	logical -> offset
 */
void report_logical2offset(unsigned int  logical, unsigned int  offset);

/*
 * report_summary
 * Write out a mesasge that indicates summary information for the page table.
 * page_size - Number of bytes per page
 * hits - Number of times a page was mapped.
 * frames_used - Number of frames allocated
 * addreses - Number of addresses processed
 * bytes - Total number of bytes needed for data structure.  
 *	   Should include all levels, allocated arrays, etc.
 */
void report_summary(unsigned int page_size, unsigned int hits,
                    unsigned int addresses, unsigned int frames_used,
                    unsigned int bytes);

/*
 * report_bitmasks
 * Write out bitmasks.
 * levels - Number of levels
 * masks - Pointer to array of bitmasks
 */
void report_bitmasks(int levels, std::vector<unsigned int>  bma);

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
                    std::vector<int> pagemap, int frame_number);

