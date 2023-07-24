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
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/vaddr.h>
#include "/home/gwc/ysyx-workbench/nemu/src/monitor/sdb/watchpoint.h"

static int is_batch_mode = false;



void init_regex();


/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args)
{
  nemu_state.state = NEMU_QUIT;
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
    isa_reg_display();
  }
  else if(strcmp(info_arg, "w") == 0)
  {
    wpprint();
  }
  else
  {
    printf("Wrong argument\n");
  }

  return 0;
}

static int cmd_si(char *args)
{
  char *si_arg = strtok(NULL, " ");
  if (nemu_state.state == NEMU_END)
    cpu_exec(1);
  else if (si_arg == NULL)
  {
    cpu_exec(1);
    printf("run 1 times  \n");
  }
  else if (!isdigit(*si_arg))
  {
    printf("%s:Wrong argument\n", si_arg);
  }
  else
  {
    cpu_exec(atoi(si_arg));
    printf("run %d times\n", atoi(si_arg));
  }

  return 0;
}

static int cmd_x(char *args)
{
  char *x_arg = strtok(NULL, " ");
  int x_len=0;
  sword_t num_temp=0;
  bool booltemp;
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
        num_temp=expr(x_arg,&booltemp);
        if(booltemp) 
        for (int i = 0; i < x_len; i = i + 1)
        {
          printf("0x%08x:   %08x\n", num_temp+4*i, vaddr_read(num_temp+ 4 * i, 4));
        }
      }
    }
  }
  return 0;
}

static int cmd_p(char *args)
{
  char *p_arg = strtok(NULL, "");
  bool p_success;
  word_t num_temp =0;
  if (p_arg == NULL)
    printf("Wrong argument\n");
  else num_temp= expr(p_arg, &p_success);
  if (p_success)
    printf("HEX : 0x%08x\nDEC : %d\n", num_temp, num_temp);
   //printf("ra 0x%08x\n",gpr(1));

  return 0;
}

static int cmd_w(char *args)
{
  char *w_arg=strtok(NULL," ");
  WP* wptemp=NULL;
  bool booltemp;
  word_t numtemp=0;
  if(w_arg==NULL) printf("Wrong agrument\n");
  else {
    numtemp=expr(w_arg,&booltemp);
    if(booltemp)
    {wptemp=new_wp();
    wptemp->regnum=numtemp;
    strcpy(wptemp->regname,w_arg);}
  }

  return 0;
}

static int cmd_d(char *args)
{
  char *d_arg=strtok(NULL," ");
  if(d_arg==NULL) printf("wrong agrument\n ");
  else if (!isdigit(*d_arg))
    {
      printf("Wrong argument\n");
    }
  else deletewp(atoi(d_arg));
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
    {"x", "Explore memory", cmd_x},
    {"p", "expr to value", cmd_p},
    {"w", "watchpoints", cmd_w},
    {"d","delete watchpoint",cmd_d}

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
  is_batch_mode = true;
}

void sdb_mainloop()
{
  if (is_batch_mode)
  {
    cmd_c(NULL);
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

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

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

void init_sdb()
{
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
