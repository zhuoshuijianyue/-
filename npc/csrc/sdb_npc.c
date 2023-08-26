#include <stdio.h>
#include <stdlib.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include"common_npc.h"

const char *rs32regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
const char *pc_reg="pc";
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
static int is_batch_mode=0;

void npc_regdisplay(){
for (int i = 0; i < 32; i = i + 4)
    printf("%-3s  : 0x%08x	;%-3s  : 0x%08x	;%-3s  : 0x%08x	;%-3s  : 0x%08x\n", rs32regs[i], dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[i], rs32regs[i + 1],
           dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[i+1], rs32regs[i + 2], dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[i+2], rs32regs[i + 3], 
           dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[i+3]);
  printf("%-3s  : 0x%08x	;\n", pc_reg, dut.pc);
  //printf("sp :%08x\n",dut.rootp->ysyx_23060020_top__DOT__u_ysyx_23060020_rf__DOT__regs[2]);
}


static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(NPC) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  single_cycle(-1);
  printf("command : c\n");
  return 0;
}

static int cmd_q(char *args)
{
  return -1;
}

static int cmd_help(char *args);
static int cmd_info(char *args)
{
  char *info_arg = strtok(NULL, " ");
  if (info_arg == NULL)
  {
    printf("Wrong argument\n");
  }
  else if (strcmp(info_arg, "r") == 0)
  {
    npc_regdisplay();
  }
  else
  {
    printf("Wrong argument\n");
  }

  return 0;
}

static int cmd_si(char *args)
{
  single_cycle(1);
  if(!ebreak_bool) printf("ALL DONE \n");
  return 0;
}

static int cmd_x(char *args)
{
  char *x_arg = strtok(NULL, " ");
  int x_len=0;
  int num_temp=0;
  char *str_temp;
  if (x_arg == NULL)
  {
    printf("Wrong argument\n");
  }
  else
  {
    if (!isdigit(*x_arg))
    {
      printf("%s:Wrong argument\n", x_arg);
    }
    else
    {
      x_len = atoi(x_arg);
      x_arg = strtok(NULL, " ");
      if (x_arg == NULL)
      {
        printf("Wrong argument\n");
      }

      else
      {
        num_temp=strtol(x_arg,&str_temp,16);
        if (*str_temp != '\0') {
        assert(0);
    }
        for (int i = 0; i < x_len; i = i + 1)
        {
          printf("0x%08x:   %08x\n", num_temp+4*i, pmem_read(num_temp+ 4 * i));
        }
      }
    }
  }
  return 0;
}



static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display information about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"info", "Print information", cmd_info},
    {"si", "Single run", cmd_si},
    {"x", "Explore memory", cmd_x}
    /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode()
{
  is_batch_mode = 1;
}

void sdb_mainloop()
{
  if (is_batch_mode)
  {
    //cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;)
  {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }



    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}
