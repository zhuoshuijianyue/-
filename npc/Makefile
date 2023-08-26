VERILATOR_FLAGSVERILATOR_FLAGS += -cc --exe -Wall -O3
VERILATOR_FLAGSVERILATOR_FLAGS += --build
VERILATOR_FLAGSVERILATOR_FLAGS += --Mdir build
VERILATOR_FLAGSVERILATOR_FLAGS +=--x-assign unique --x-initial unique

LDFLAGS+=-lreadline
CPATH=~/ysyx-workbench/npc/csrc
VPATH=~/ysyx-workbench/npc/vsrc
IMG = /home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/dummy-riscv32e-npc.bin
DFF =--diff=/home/gwc/ysyx-workbench/nemu/build/riscv32-nemu-interpreter-so
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

sim:Vysyx_23060020_top
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@./build/Vysyx_23060020_top $(DFF) $(IMG)
	@-rm -rf ./build/Vysyx_23060020_top 



Vysyx_23060020_top:
	verilator $(VERILATOR_FLAGSVERILATOR_FLAGS) $(CSRCS) --top-module ysyx_23060020_top $(VSRCS) -LDFLAGS $(LDFLAGS)

clean:
	-rm -rf build

include ../Makefile


