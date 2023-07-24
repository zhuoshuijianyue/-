#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

const char *expr_str[]= {"+","-","*","/","(",")"};
/* type : 0  expr
          1  num
          2  str
          3  (
          4  )
*/

static int stack[65536];
static unsigned str_num=0;

void init_stack(){
  for(int i=0;i<=65366;i++)
  {
    stack[i]=-1;

  }
  stack[0]=0;
  str_num=0;
}


void push(int a){
  for(int i=0;i<=65536;i++)
  {
    if(stack[i]==-1) {
      stack[i]=a;
    break;
    }

}
}

void pop(){
  for(int i=0;i<=65536;i++)
  {
    if(stack[i]==-1) 
    {
      stack[i-1]=-1;
    break;
    }
  
}
}



 static void gen_rand_expr() {
  init_stack();
  int pcnum=0;
  int randnum=0;
  while(pcnum!=-1)
  {
    if(str_num>65530)
  {
    init_stack();
    strcpy(buf,"");
  }
    switch(stack[pcnum]) {
    case 0:{
      randnum=rand()%3;
      if(randnum==0){
        pop();
        str_num+=sprintf(buf+str_num,"%u",rand()%40);
      }
      else if(randnum==1){
        pop();
      push(4);
      push(0);
      push(3);
      pcnum=pcnum+3;
      }
      else if(randnum==2){
        pop();
      push(0);
      push(2);
      push(0);
      pcnum=pcnum+3;
       }
      break;
      }
    case 1:{
      pop();
      str_num+=sprintf(buf+str_num,"%u",rand()%40);
      break;
      }
    case 2:{
      pop();
      str_num+=sprintf(buf+str_num,"%s",expr_str[rand()%4]);
      break;
      }
    case 3:{
      pop();
      str_num+=sprintf(buf+str_num,"%s",expr_str[4]);
      break;
      }
    case 4:{
      pop();
      str_num+=sprintf(buf+str_num,"%s",expr_str[5]);
      break;
      }
    default:{
      break;
      }
    }
    pcnum--;
  }
  
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    strcpy(code_buf,"");
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror /tmp/.code.c -o  /tmp/.expr");
    if (ret != 0) continue;
      


    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
    strcpy(code_buf,"");
  }
  return 0;
}
