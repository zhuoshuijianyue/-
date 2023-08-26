#ifndef _COMMON_NPC_H_
#define _COMMON_NPC_H_
#include "Vysyx_23060020_top.h"
#include "verilated.h"
#include <Vysyx_23060020_top___024root.h>
#include "svdpi.h"
#include "Vysyx_23060020_top__Dpi.h"
extern Vysyx_23060020_top dut ;//例化verilog模块
extern bool ebreak_bool;
void single_cycle(unsigned); 
int pmem_read(int);

#endif