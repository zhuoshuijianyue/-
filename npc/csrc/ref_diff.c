#include <dlfcn.h>
#include "common_npc.h"
#include <assert.h>
#include "ref_diff.h"
//void (*ref_difftest_memcpy)(unsigned long addr, void *buf, size_t n, bool direction) = NULL;
//void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
//void (*ref_difftest_exec)(unsigned long n) = NULL;
REF_difftest_raise_intr ref_difftest_raise_intr = NULL;

static bool is_skip_ref = false;
static int skip_dut_nr_inst = 0;

void difftest_skip_ref() {
  is_skip_ref = true;
  skip_dut_nr_inst = 0;
}


void init_difftest(char *str) {
  assert(str != NULL);

  void *handle;
  handle = dlopen(str, RTLD_LAZY);
  assert(handle);

  /*ref_difftest_memcpy = dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);*/

  ref_difftest_raise_intr = (REF_difftest_raise_intr)dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  /*void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);*/

}


