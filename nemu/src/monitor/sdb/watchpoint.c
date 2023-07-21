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

#include "sdb.h"
#include "watchpoint.h"
//#include<assert.h>
#define NR_WP 32


WP *head;
WP *free_;
WP wp_pool[NR_WP] = {};

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}


WP* new_wp()
{
    //WP *temp;
    WP *freetemp=free_;
    int freenum=free_->NO;
    WP *result=free_;
  if(free_==NULL) assert(0);
  if(head==NULL) {
    head=free_;
    free_=free_->next;
    head->next=NULL;
    return head;
  }
  else {
    for(WP *headtemp=head;headtemp!=NULL;headtemp=headtemp->next)
    {
        if((headtemp==head)&&(head->NO>freenum))
        {
            freetemp=free_;
            free_=free_->next;
            freetemp->next=head;
            head=freetemp;
            return result;
        }
        else if(headtemp->next!=NULL){
        if(headtemp->next->NO>freenum)
        {
            freetemp=free_;
            free_=free_->next;
            freetemp->next=headtemp->next;
            headtemp->next=freetemp;
            return result;
        }
        }
        else if((headtemp->next==NULL)&&(headtemp->NO<freenum))
        {
            headtemp->next=free_;
            free_=free_->next;
            headtemp->next->next=NULL;
            return result;
        }
    }

  }
  return NULL;
}

/*word_t numtest()
{
  return gpr(1);
}*/

void free_wp(WP *wp){
  return ;
}

void deletewp(int num)
{
    WP *temp=NULL;
   for(WP *headtemp=head;headtemp!=NULL;headtemp=headtemp->next)
   {
    if(head==headtemp&&headtemp->NO==num){
        head=head->next;
        headtemp->next=free_;
        free_=headtemp;
        return ;
    }
    if(headtemp->next!=0)
    {
        if(headtemp->next->NO==num)
        {
            temp=headtemp->next;
            headtemp->next=headtemp->next->next;
            temp->next=free_;
            free_=temp;
            return ;
        }
    }
    
   }
   return ;
}
/*word_t headnumprint(WP *temp){

  return temp->num
}*/

void wpprint()
{
  for(WP *headtemp=head;headtemp!=NULL;headtemp=headtemp->next)
  {
    printf("watchpoint NO %d : %s\n",headtemp->NO,headtemp->regname);
  }
}