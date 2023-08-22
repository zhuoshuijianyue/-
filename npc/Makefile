VERILATOR_FLAGSVERILATOR_FLAGS += -cc --exe -Wall -O3
#VERILATOR_FLAGSVERILATOR_FLAGS += -trace
VERILATOR_FLAGSVERILATOR_FLAGS += --build
VERILATOR_FLAGSVERILATOR_FLAGS += --Mdir build
VERILATOR_FLAGSVERILATOR_FLAGS +=--x-assign unique --x-initial unique
#VERILATOR_FLAGSVERILATOR_FLAGS += -I /home/gwc/ysyx-workbench/npc/vsrc
CPATH=~/ysyx-workbench/npc/csrc
VPATH=~/ysyx-workbench/npc/vsrc

VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
all:
	@echo "Write this Makefile by your self."

sim:Vysyx_23060020_top
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@./build/Vysyx_23060020_top 5
	@-rm -rf ./build/Vysyx_23060020_top 



Vysyx_23060020_top:
	verilator $(VERILATOR_FLAGSVERILATOR_FLAGS) $(CSRCS) --top-module ysyx_23060020_top $(VSRCS) 

clean:
	-rm -rf build

include ../Makefile


