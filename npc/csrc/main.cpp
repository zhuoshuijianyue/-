#include<stdlib.h>
#include<iostream>
#include <assert.h>
#include <dlfcn.h>
#include "verilated.h"
#include "Vysyx_23060020_top.h"
#include <Vysyx_23060020_top___024root.h>
#include "svdpi.h"
#include "Vysyx_23060020_top__Dpi.h"


#include"mem_npc.h"
#include"sdb_npc.h"
#include"common_npc.h"
#include "ref_diff.h"
#define MAX_SIM_TIME 40
vluint64_t sim_time=0;
uint32_t *pmem_temp=(uint32_t *)malloc(sizeof(uint32_t));
static unsigned npc_spc;
bool diffregs=1;
CPU_STATE cpu_dut;
CPU_STATE cpu_ref;
bool ebreak_bool=true;
void ebreak(){
  ebreak_bool=false;
}
Vysyx_23060020_top dut ;//例化verilog模块
void init_cpu(int);

static void init_data(){
  sim_time=0;
  pmem=(unsigned char*)malloc(0x8000000);
  
  init_difftest(diff_so_file);
  ref_difftest_init(0);
  ref_difftest_memcpy(MEMBASE , (void*)pmem, load_img() , 0);
  init_cpu(10);
}

bool difftest_checkregs(){
  bool temp=1;
  for(int i=0;i<32;i++)
  {
    if(cpu_ref.gpr[i]!=dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[i])
    {
      printf("regs[%d] different with ref of NEMU at 0x%08x\n",i,npc_spc);temp=0;
    }
  }
  if(cpu_ref.pc!=dut.pc) {printf("PC different with ref of NEMU at 0x%08x\n",npc_spc); temp=0;}
 return temp;
}

void single_cycle(unsigned times) {
  while((times--)&&(ebreak_bool)&&diffregs){
  ref_difftest_exec(1);
  ref_difftest_memcpy(0x80008bce , (void*)pmem_temp, 4*sizeof(uint32_t) , 1);
  printf("ref_mem  address/data : 0x80008bce,0x%08x\n",*(unsigned*)pmem_temp);
  npc_spc=dut.pc;
  pmem_read(dut.pc,(int*)&dut.instw);
  printf("pc : 0x%08x  instw : 0x%08x\n",dut.pc,dut.instw);
  printf("sim time : %ld\n",sim_time);
  dut.clk = 0; 
  dut.eval();
  dut.clk = 1; 
  dut.eval();
  
  ref_difftest_regcpy(&cpu_ref,1 );
  diffregs=difftest_checkregs();
  sim_time++;
  }
  if(!ebreak_bool&&dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[10]==0)
  printf("\e[1;34m NPC: \e[1;32mHIT GOOD TRAP\e[0m \e[0m at pc = 0x%08x\n",npc_spc);
  if(!ebreak_bool&&dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[10]!=0) 
  printf("\e[1;34m NPC: \e[1;31mHIT BAD TRAP\e[0m at \e[0m at pc = 0x%08x\n",npc_spc);
}

void init_cpu(int n){
  dut.rst=1;
  printf("test1");
  dut.clk=0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
  pmem_read(dut.pc,(int*)&dut.instw);
  while(n!=0) {
  dut.clk = 0; dut.eval();
  dut.clk = 1; dut.eval();
  n--;}
  dut.rst=0;
  printf("init_cpu success !\n");
}


  int main(int argc, char** argv) {
      parse_args(argc, argv) ;
      init_data();
      sdb_mainloop(); 
      free(pmem);
      exit(EXIT_SUCCESS);
  }
