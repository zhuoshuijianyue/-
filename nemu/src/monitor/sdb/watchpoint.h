#ifndef WATCHPOINT_H
#define WATCHPOINT_H
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char regname[30];
  word_t regnum;
  /* TODO: Add more members if necessary */

} WP;
extern WP *head ;
extern WP *free_;
extern WP* new_wp();
extern void init_wp_pool();
extern void wpprint();
extern void deletewp(int num);
#endif