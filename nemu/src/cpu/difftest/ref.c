/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>
#define DIFFTEST_TO_DUT true
#define DIFFTEST_TO_REF false
struct dut_gpr {
  word_t gpr[32];
  word_t pc;
};
__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  if(direction) {for(int i=0;i<n;i++)
    *((uint8_t*)buf+i)=*guest_to_host(addr+i);}
  else {for(int j=0;j<n;j++)
    *guest_to_host(addr+j)=*((uint8_t*)buf+j);}
    printf("difftest_memcpy success !\n");
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  struct dut_gpr* dut_temp = (struct dut_gpr*)dut;
  if(direction){
  for(int i=0;i<32;i++){
  dut_temp->gpr[i]=cpu.gpr[i];
  }
  dut_temp->pc=cpu.pc;
  }
  else {
    for(int j=0;j<32;j++)
    {
      cpu.gpr[j]=dut_temp->gpr[j];
    }
    cpu.pc=dut_temp->pc;
    }
}

__EXPORT void difftest_exec(uint64_t n) {
  cpu_exec(n);
}

__EXPORT void difftest_raise_intr(word_t NO) {
  //assert(0);
  printf("diff ref  success\n");
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();
  /* Perform ISA dependent initialization. */
  init_isa();
}
