#include<stdlib.h>
#include<iostream>
#include<verilated_vcd_c.h>
#include <assert.h>
#include "verilated.h"
#include "test.h"

#include "Vysyx_23060020_top.h"
#include <Vysyx_23060020_top___024root.h>
#include "svdpi.h"
#include "Vysyx_23060020_top__Dpi.h"
#define MAX_SIM_TIME 40
vluint64_t sim_time=0;
static bool ebreak_bool=true;
void ebreak(){
  ebreak_bool=false;
}
static Vysyx_23060020_top dut ;//例化verilog模块



//about memory
#define MEMBASE 0x80000000
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
static uint8_t *pmem = NULL;

uint8_t* guest_to_host(uint32_t paddr) { return pmem + paddr - MEMBASE; }
uint32_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MEMBASE; }
static char *img_file = (char*)"/home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/dummy-riscv32e-npc.bin";


static inline uint32_t host_read(void *addr) {
  return *(uint32_t *)addr;
}

extern "C" int pmem_read(int raddr) {
  return host_read(guest_to_host((unsigned)raddr));
}
extern "C" void pmem_write(int waddr, int wdata) {
  *(uint32_t *)guest_to_host((unsigned)waddr)=(unsigned)wdata;
}

/*extern  unsigned pmem_read(unsigned paddr){
  return host_read(guest_to_host(paddr));
}

extern  void pmem_write(unsigned paddr,unsigned pdata){
  *(uint32_t *)guest_to_host(paddr)=pdata;
}*/

static long load_img() {
  FILE *fp = fopen(img_file, "rb");

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(guest_to_host(MEMBASE), size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}
//

static void single_cycle() {
  dut.clk = 0; dut.instw=pmem_read(dut.pc);dut.eval();
  dut.clk = 1; dut.instw=pmem_read(dut.pc);dut.eval();
}

void init_cpu(int n){
  dut.rst=1;
  //dut.pc=0x80000000;
  dut.clk=0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
  while(n!=0) {single_cycle();n--;}
  dut.rst=0;
}


  int main(int argc, char** argv) {
        int a;
        int b;
      //VerilatedContext* contextp = new VerilatedContext;
      //contextp->commandArgs(argc, argv);


    //Verilated::traceEverOn(true);
    //VerilatedVcdC *m_trace=new VerilatedVcdC;//波形仿真
    //dut->trace(m_trace,99);
    //m_trace->open("waveform.vcd");
      pmem=(unsigned char*)malloc(MEMBASE/8);
      init_cpu(10);
      pmem_write(0x80000000,0x00000413);
      pmem_write(0x80000004,0x00009117);
      pmem_write(0x80000008,0xffc10113);
      pmem_write(0x8000000c,0x00100073);
      load_img();
      do 
      { 
    
        //m_trace->dump(sim_time);
        //printf("pc = %08x , instw = %08x\n",dut.pc,dut.instw);
       printf("sim_times %ld\n",sim_time);
       printf("pc : 0x%08x  instw : 0x%08x\n",dut.pc,pmem_read(dut.pc));
       single_cycle(); 
       sim_time++;
      }while(ebreak_bool);
      //m_trace->close();
      if(dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[10]==0) 
      printf("\e[1;34m npc: \e[1;32mHIT GOOD TRAP\e[0m at \e[0m at pc = 0x%08x\n",dut.pc-4);
     else printf("\e[1;34m npc: \e[1;31mHIT BAD TRAP\e[0m at \e[0m at pc = 0x%08x\n",dut.pc-4);
      free(pmem);
      exit(EXIT_SUCCESS);
  }
//verilator -trace --cc --exe --build -Wall -j 0 main.cpp top.v
//make -C obj_dir -f Vtop.mk Vtop
//./obj_dir/Vtop