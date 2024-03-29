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
#include "local-include/reg.h"

const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
const char *pc_reg = "pc";
void isa_reg_display()
{
  bool *reg_success;
  bool success_temp = false;
  reg_success = &success_temp;
  for (int i = 0; i < 32; i = i + 4)
    printf("%-3s  : 0x%08x	;%-3s  : 0x%08x	;%-3s  : 0x%08x	;%-3s  : 0x%08x\n", regs[i], isa_reg_str2val(regs[i], reg_success), regs[i + 1],
           isa_reg_str2val(regs[i + 1], reg_success), regs[i + 2], isa_reg_str2val(regs[i + 2], reg_success), regs[i + 3], isa_reg_str2val(regs[i + 3], reg_success));
  printf("%-3s  : 0x%08x	;\n", pc_reg, cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success)
{
  word_t reg2val=0;
  *success = false;
  for (int i = 0; i < 32; i++)
  {
    if (strcmp(s, regs[i]) == 0)
    {
      *success = true;
      reg2val = gpr(i);
    }
  }
  if (strcmp(s, pc_reg) == 0)
  {
    *success = true;
    reg2val = cpu.pc;
  }
  return reg2val;
}
