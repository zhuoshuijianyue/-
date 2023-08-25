#include<stdlib.h>
#include<iostream>
#include <assert.h>
#include "mem.h"

//about memory
char *img_file = (char*)"/home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/dummy-riscv32e-npc.bin";
uint8_t *pmem = NULL;
uint8_t* guest_to_host(uint32_t paddr) { return pmem + paddr - MEMBASE; }
uint32_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MEMBASE; }
//static char *img_file = (char*)"/home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/dummy-riscv32e-npc.bin";


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

long load_img() {
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