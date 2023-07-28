#include<stdlib.h>
#include<iostream>
#include<verilated_vcd_c.h>
#include <assert.h>
#include "Vtop.h"
#include "verilated.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#define MAX_SIM_TIME 40
int add(int a, int b) { printf("a + b = %d",a+b);return a+b; }
vluint64_t sim_time=0;

  int main(int argc, char** argv) {
        int a;
        int b;
      //VerilatedContext* contextp = new VerilatedContext;
      //contextp->commandArgs(argc, argv);
      Vtop* dut= new Vtop ;//例化verilog模块

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace=new VerilatedVcdC;//波形仿真
    dut->trace(m_trace,99);
    m_trace->open("waveform.vcd");

      while (sim_time<MAX_SIM_TIME) 
      { 
        a = rand() & 1;
        b = rand() & 1;
        dut->a = a;
        dut->b = b;
        dut->eval(); 
        m_trace->dump(sim_time);
        printf("a = %d, b = %d, f = %d\n", a, b, dut->f);
        //printf("sim_time = %ld", sim_time);
       assert(dut->f == (a ^ b));
       sim_time++;
      }
      m_trace->close();
      delete dut;
      exit(EXIT_SUCCESS);
  }
//verilator -trace --cc --exe --build -Wall -j 0 sim_main.cpp top.v
//make -C obj_dir -f Vtop.mk Vtop
//./obj_dir/Vtop