#ifndef _REF_DIFF_H_
#define _REF_DIFF_H_
#include"common_npc.h"
typedef void (*REF_difftest_memcpy)(unsigned long , void *, size_t , bool ) ;
typedef void (*REF_difftest_regcpy)(void *, bool ) ;
typedef void (*REF_difftest_exec)(unsigned long ) ;
typedef void (*REF_difftest_raise_intr)(unsigned long ) ;
extern REF_difftest_memcpy ref_difftest_memcpy;
extern REF_difftest_regcpy ref_difftest_regcpy;
extern REF_difftest_exec ref_difftest_exec;
extern REF_difftest_raise_intr ref_difftest_raise_intr;
typedef void (*REF_difftest_init) (int); 
extern REF_difftest_init  ref_difftest_init ;
void init_difftest(char *);
#endif