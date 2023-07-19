VERILATOR_FLAGSVERILATOR_FLAGS += -cc --exe -Wall
VERILATOR_FLAGSVERILATOR_FLAGS += -trace
VERILATOR_FLAGSVERILATOR_FLAGS += --build
VERILATOR_FLAGSVERILATOR_FLAGS += --Mdir build
CPATH=~/ysyx-workbench/npc/csrc
VPATH=~/ysyx-workbench/npc/vsrc


all:
	@echo "Write this Makefile by your self."

sim:waveform.vcd
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	gtkwave waveform.vcd

waveform.vcd:Vtop
	./build/Vtop

Vtop:
	verilator $(VERILATOR_FLAGSVERILATOR_FLAGS) $(CPATH)/main.cpp $(VPATH)/top.v

clean:
	rm -rf build
	rm waveform.vcd

include ../Makefile


