#include<stdlib.h>
#include<iostream>
#include <assert.h>
#include "mem_npc.h"
#include <getopt.h>
#include"common_npc.h"
#include "ref_diff.h"

//about memory
char *img_file = NULL;
char *diff_so_file=NULL;
uint8_t *pmem = NULL;
uint8_t* guest_to_host(uint32_t paddr) { return pmem + paddr - MEMBASE; }
uint32_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MEMBASE; }
//static char *img_file = (char*)"/home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/dummy-riscv32e-npc.bin";


static inline uint32_t host_read(void *addr) {
  return *(uint32_t *)addr;
}

extern "C" void pmem_read(int raddr,int *rdata) {
  //printf("mem read address/data : 0x%08x,0x%08x\n",(unsigned)raddr,(unsigned)*rdata);
  /*if(raddr>0x87ffffff||raddr<0x80000000) {
    printf("wrong address : 0x%08x\n",raddr);
    assert(0);
    }*/
    /*if(raddr==0) return ;
    else*/ if(raddr>0x87ffffff||raddr<0x80000000) {
      return;}
  else  {*rdata=host_read(guest_to_host((unsigned)raddr));return;}
}
extern "C" void pmem_write(int waddr, int wdata,char wmask) {
  int num_temp=0;
  uint32_t* pmem_temp=(uint32_t*)malloc(sizeof(uint32_t));
  if(waddr>0x87ffffff||waddr<0x80000000) {
    printf("wrong address : 0x%08x\n",waddr);
    assert(0);
    }
  if(wmask==15){
      *(uint32_t *)guest_to_host((unsigned)waddr)=(unsigned)wdata;
      num_temp=4;
    }
  else if(wmask==3){
    //printf("SH\n");
    *(short *)guest_to_host((unsigned)waddr)=(short)wdata;
    num_temp=2;
  }
  else if(wmask==1){
    //printf("SB\n");
    *(uint8_t *)guest_to_host((unsigned)waddr)=(uint8_t)wdata;
    num_temp=1;
  }
  else {*(uint32_t *)guest_to_host((unsigned)waddr)=0;}
  printf("mem write address/data : 0x%08x,0x%08x\n",(unsigned)waddr,(unsigned)wdata);
  //if(waddr<=0x80008bce&&(waddr+12)>=0x80008bce)
  //printf("mem write address/data : 0x%08x,0x%08x\n",(unsigned)waddr,(unsigned)wdata);
  //ref_difftest_memcpy(waddr , (void*)pmem_temp, 4*sizeof(uint32_t) , 1);
  //printf("ref_mem write address/data : 0x%08x,0x%08x\n",(unsigned)waddr,*(unsigned*)pmem_temp);
  //if((uint8_t)wdata!=*(uint8_t*)pmem_temp) {ebreak_bool=0;printf("mem different at 0x%08x\n",waddr);}
}





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
int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"port"     , required_argument, NULL, 'p'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1) {
    switch (o) {
      case 'b':  break;
      case 'p':  break;
      case 'l':  break;
      case 'd': diff_so_file = optarg;printf("diff_so_file is %s\n",diff_so_file) ;break;
      case 1: img_file = optarg ;printf("img_file is %s\n",img_file) ;return 0;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}