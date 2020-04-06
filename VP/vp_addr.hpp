#ifndef _VP_TSP_ADDR_H_
#define _VP_TSP_ADDR_H_

#include "convolution.hpp"

const sc_dt::uint64 VP_ADDR_CONV = 0x43C00000;
const sc_dt::uint64 VP_ADDR_CONV_KERNEL= VP_ADDR_CONV + CONV_KERNEL;
const sc_dt::uint64 VP_ADDR_CONV_LINES= VP_ADDR_CONV + CONV_LINES;
const sc_dt::uint64 VP_ADDR_CONV_COLUMNS= VP_ADDR_CONV + CONV_COLUMNS;
const sc_dt::uint64 VP_ADDR_CONV_BEGIN= VP_ADDR_CONV + CONV_BEGIN;

const sc_dt::uint64 VP_ADDR_MEMORY_A = 0x43B00000;
const sc_dt::uint64 VP_ADDR_MEMORY_B = 0x43B00001;
const sc_dt::uint64 VP_ADDR_MEMORY_C = 0x43B00002;



#endif
