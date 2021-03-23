//Matthew Turi
//ID: 822202323
//03/23/21

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../include/byutr.h"
#include "../include/output_helpers.h"
#include "../include/pagetable.h"
#include "../include/helper_methods.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>


int main(int argc, char **argv) {
    int opt = 0;
    int num = -1;
    char * output_method = "";
    while ((opt = getopt(argc, argv, "n:o:")) != -1) {
        switch (opt) {
            case 'n': /* Number of addresses to process */
                num = atoi(optarg);
                break;
                // optarg will contain the string following -n
                // Process appropriately (e.g. convert to integer atoi(optarg))
            case 'o':/* produce map of pages */
                output_method = optarg;
                // optarg contains the output method...
                break;
            default:
                // print something about the usage and die...
                std::cout << "INCORRECT FLAGS" << std::endl;
                exit(-1);
        }
    }
    /* first mandatory argument, optind is defined by getopt */
    int idx = optind; // first arg will be trace file location, then after is page table level sizes
    char * file = "";
    std::vector<int> level_bits;
    for (; idx < argc; idx++) {
        if (idx == optind)
            file = argv[idx];
        else{
            level_bits.push_back(atoi(argv[idx]));
            //need to parse the level sizes and do something with that...maybe make a vector to handle this?
        }
    }
    int bitsum = 0; // check if they entered valid level bits
    for (int i : level_bits)
        bitsum += i;
    if (bitsum > 32){
        std::cout << "LEVELS ARE TOO BIG. MAX ADDRESS SPACE IS 32" << std::endl;
        exit(-1);
    }

    PageTable* pt = init(level_bits); // intialize pagetbale

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        /* couldn't open, bail gracefully */
        printf("FAILED TO OPEN FILE\n");
        exit(-1);
    }
    bool l2p = false, off = false, sum = false, p2f = false;
    if (output_method != ""){ // set flags for output methods
        if (std::strcmp(output_method, "logical2physical") == 0){
            l2p = true;
        }
        else if (std::strcmp(output_method, "offset") == 0){
            off = true;
        }
        else if (std::strcmp(output_method, "summary") == 0){
            sum = true;
        }
        else if (std::strcmp(output_method, "bitmasks") == 0){ // just report bitmasks and exit
            report_bitmasks(pt->LevelCount, pt->BitMaskAry);
            exit(0);
        }
        else if (std::strcmp(output_method, "page2frame") == 0){
            p2f = true;
        }
        else{ // unknown or no method specified
            std::cout << "Unknown output method specified. Defaulting to summary" << std::endl;
        }
    }
    else
        sum = true;
    unsigned int curr_off = 0;
    /* Start reading addresses */
    p2AddrTr trace_item;  /* Structure with trace information */
    bool done = false;
    int read_count = 0;
    while (! done) {
        if (num != -1){
            if (read_count == num) // for setting num addresses to read
                break;
        }
        // Grab the next address
        int bytesread = NextAddress(fp, &trace_item);
        // Check if we actually got something`
        done = bytesread == 0;
        if (! done){
            read_count++;
            unsigned int logical_address = trace_item.addr;
            Map * m = PageLookup(pt, logical_address, p2f); // page lookup
            if (off or l2p){ // output modes
                std::string temp_off = LogicalToOffset(logical_address, level_bits);
                curr_off = StrtoInt(temp_off); //get offset, need for both output methods
                if (off)
                    report_logical2offset(logical_address, curr_off);
            }
            if (m == NULL){ // if null, we need to create a new frame and insert
                PageInsert(pt, logical_address, FrameNumber, p2f);
                if (p2f) // output methods
                    report_pagemap(logical_address, pt->LevelCount, pagemap, FrameNumber);
                else if (l2p){
                    unsigned int physical_add = LogicalToPhysical(FrameNumber, level_bits, curr_off);
                    report_logical2physical(logical_address, physical_add);
                }
                FrameNumber++;
            }
            else{
                hit_count ++;
                if (p2f) // output methods
                    report_pagemap(logical_address, pt->LevelCount, pagemap, ins_frame);
                else if (l2p){
                    unsigned int physical_add = LogicalToPhysical(ins_frame, level_bits, curr_off);
                    report_logical2physical(logical_address, physical_add);
                }
            }
        }
    }
    if (sum)
        report_summary(getPageSize(level_bits), hit_count, read_count, FrameNumber, numBytesTotal(pt));
}
