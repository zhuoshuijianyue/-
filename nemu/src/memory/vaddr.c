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
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  #ifdef CONFIG_MTRACE
  log_write("memory read at 0x%x  length %d\nValue : 0x%08x\n", addr,len,paddr_read(addr, len));
  #endif
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  #ifdef CONFIG_MTRACE
  log_write("memory write at 0x%x  length %d\n", addr,len);
  log_write("Old value : 0x%x\n", paddr_read(addr, len));
  #endif
  paddr_write(addr, len, data);
  #ifdef CONFIG_MTRACE
  log_write("NEW value : 0x%x\n", paddr_read(addr, len));
  #endif
}
