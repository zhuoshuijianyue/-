#ifndef _MEM_H_
#define _MEM_H_
#define MEMBASE 0x80000000
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
extern uint8_t *pmem ;
extern char *img_file ;
long load_img();
#endif