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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/vaddr.h>
enum {
  TK_NOTYPE = 256, TK_LEFT=255,TK_RIGHT=254,TK_EQ=253,TK_DEC=252,TK_HEX=251,TK_NEG=250,TK_MUL=249,TK_DIV=248,
  TK_PLUS=247,TK_SUB=246,TK_POINT=245,TK_OR=244,TK_AND=243,TK_NEQ=242,TK_REG=241,TK_REGNAME=240

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},     // spaces
  {"0[xX][0-9,a-f,A-F]+",TK_HEX},  //hex
  {"[0-9]+", TK_DEC},   //dec
  {"\\|\\|",TK_OR},      //or
  {"&&",TK_AND},         //and
  {"==", TK_EQ},        // equal
  {"!=",TK_NEQ},          //not equal
  {"\\+", TK_PLUS},         // plus
  {"-", TK_SUB},           // sub
  {"\\*", TK_MUL},         // multiply
  {"/",TK_DIV},            //div
  {"\\(", TK_LEFT},         //  
  {"\\)", TK_RIGHT},         //
  {"\\$",TK_REG}, //reg
  {"ra|sp|gp|tp|t0|t1|t2|s0|s1|a0|a1|a2|a3|a4|a5|a6|a7|s2|s3|s4|s5|s6|s7|s8|s9|s10|s11|t3|t4|t5|t6|pc",TK_REGNAME} //regname

};
#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
  int pri;
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        /*Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);*/

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

      switch (rules[i].token_type) {
          case TK_DEC:
          {tokens[nr_token].type=TK_DEC;
          memset(tokens[nr_token].str,0,32);
          strncpy(tokens[nr_token].str,substr_start,substr_len);
          tokens[nr_token].pri=31;
          nr_token=nr_token+1;
          break;}
          case TK_HEX:
          {tokens[nr_token].type=TK_HEX;
          memset(tokens[nr_token].str,0,32);
          strncpy(tokens[nr_token].str,substr_start,substr_len);
          tokens[nr_token].pri=31;
          nr_token=nr_token+1;
          break;}
          case TK_PLUS:{
            tokens[nr_token].type=TK_PLUS;
            tokens[nr_token].pri=4;
          nr_token=nr_token+1;
          break;}
          case TK_SUB:{
            tokens[nr_token].type=TK_SUB;
            tokens[nr_token].pri=4;
          nr_token=nr_token+1;
          break;}
          case TK_MUL:{
            tokens[nr_token].type=TK_MUL;
            tokens[nr_token].pri=5;
          nr_token=nr_token+1;
          break;}
          case TK_LEFT:{
            tokens[nr_token].type=TK_LEFT;
            tokens[nr_token].pri=32;
          nr_token=nr_token+1;
          break;}
          case TK_RIGHT:{
            tokens[nr_token].type=TK_RIGHT;
            tokens[nr_token].pri=32;
          nr_token=nr_token+1;
          break;}
          case TK_DIV:{
            tokens[nr_token].type=TK_DIV;
            tokens[nr_token].pri=5;
          nr_token=nr_token+1;
          break;}
          case TK_EQ:{
            tokens[nr_token].type=TK_EQ;
            tokens[nr_token].pri=3;
          nr_token=nr_token+1;
          break;}
          case TK_NEQ:{
            tokens[nr_token].type=TK_NEQ;
            tokens[nr_token].pri=3;
          nr_token=nr_token+1;
          break;}
          case TK_AND:{
            tokens[nr_token].type=TK_AND;
            tokens[nr_token].pri=2;
          nr_token=nr_token+1;
          break;}
          case TK_REG:{
            tokens[nr_token].type=TK_REG;
            tokens[nr_token].pri=30;
          nr_token=nr_token+1;
          break;}
          case TK_REGNAME:{
          memset(tokens[nr_token].str,0,32);
           strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].type=TK_REGNAME;
            tokens[nr_token].pri=31;
          nr_token=nr_token+1;
          break;}
          default: break;
        }break;
      }
    }


    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


static bool check_parentheses(int p, int q){
  int i,cnt = 0;
  if(tokens[p].type != TK_LEFT || tokens[q].type != TK_RIGHT) 
    return false;
  for(i = p; i <= q; i++){
    if(tokens[i].type == TK_LEFT) 
      cnt++;
    else if(tokens[i].type == TK_RIGHT) 
      cnt--;
    if(cnt == 0 && i<q) 
      return false;
  }
  if(cnt != 0) return false;
  return true;
}

sword_t eval(int p,int q){
  int tag=0;
  //int test_num=31;
  int k=p;
  char *str_temp;
  bool bool_temp;
      if(p>q){assert(0);}
      else if(p==q)
      {
        
        if(tokens[p].type==TK_DEC) return atol(tokens[p].str);
        else if(tokens[p].type==TK_HEX){
          return strtol(tokens[p].str,&str_temp,16);
          }
      }
      else if(check_parentheses(p, q) == true)
      {
        return eval(p+1,q-1);
      }
      else {
        for(int i=p;i<=q;i++)
        {
          if(tokens[i].type==TK_LEFT) tag=tag+1;
          else if(tokens[i].type==TK_RIGHT) tag=tag-1;
          else if(tag==0) {
             if(tokens[i].pri<=tokens[k].pri&&!(tokens[k].pri==17&&tokens[i].pri==17))
             {
              k=i;
             }
              }
          }
        
        switch (tokens[k].type) {
          case TK_PLUS: return eval(p,k-1) + eval(k+1,q);break;
          case TK_SUB: return eval(p,k-1) - eval(k+1,q);break;
          case TK_MUL: return eval(p,k-1) * eval(k+1,q);break;
          case TK_DIV: {
            if(eval(k+1,q)==0) {
              assert(0);
            }
            return eval(p,k-1) / eval(k+1,q);break;
            }
          case TK_NEG:return (-eval(k+1,q));break;
          case TK_EQ:return(eval(p,k-1) == eval(k+1,q));break;
          case TK_NEQ:return(eval(p,k-1) != eval(k+1,q));break;
          case TK_AND:return(eval(p,k-1) && eval(k+1,q));break;
          case TK_POINT:return (vaddr_read(eval(k+1,q),4));break;
          case TK_REG:return isa_reg_str2val(tokens[k+1].str,&bool_temp);break;
          
          default: assert(0);break;
        }
      }
     
     
      return 0;
}



word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  for(int i=0;i<=nr_token-1;i++)
  {
    if(tokens[i].type==TK_SUB&&(i==0||(tokens[i-1].type!=TK_DEC && tokens[i-1].type!=TK_HEX&& tokens[i-1].type!=TK_RIGHT)))
    {
      tokens[i].type=TK_NEG;//printf("TK_NEQ at position %d\n",i);
      tokens[i].pri=17;
    }
    if(tokens[i].type==TK_MUL&&(i==0||(tokens[i-1].type!=TK_DEC && tokens[i-1].type!=TK_HEX&& tokens[i-1].type!=TK_RIGHT)))
    {
      tokens[i].type=TK_POINT;//printf("TK_POINT at position %d\n",i);
      tokens[i].pri=30;
    }
  }
   return eval(0,nr_token-1);

  /* TODO: Insert codes to evaluate the expression. */
  TODO();
}
