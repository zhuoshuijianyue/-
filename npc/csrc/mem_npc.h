#ifndef _MEM_NPC_H_
#define _MEM_NPC_H_
#define MEMBASE 0x80000000
#include "common_npc.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
extern uint8_t *pmem ;
extern char *img_file ;
extern char *diff_so_file;
long load_img();
int parse_args(int, char **);
#endif