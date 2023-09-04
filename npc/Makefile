VERILATOR_FLAGSVERILATOR_FLAGS += -cc --exe -Wall -O3
VERILATOR_FLAGSVERILATOR_FLAGS += --build
VERILATOR_FLAGSVERILATOR_FLAGS += --Mdir build
VERILATOR_FLAGSVERILATOR_FLAGS +=--x-assign unique --x-initial unique

LDFLAGS+=-lreadline
CPATH=~/ysyx-workbench/npc/csrc
VPATH=~/ysyx-workbench/npc/vsrc
IMG = /home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/build/$(ALL)-riscv32e-npc.bin
DFF =--diff=/home/gwc/ysyx-workbench/nemu/build/riscv32-nemu-interpreter-so
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
BINPATH =/home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests
ALL =$(basename $(notdir $(shell find /home/gwc/ysyx-workbench/am-kernels/tests/cpu-tests/tests/. -name "*.c")))
sim:Vysyx_23060020_top bin
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@./build/Vysyx_23060020_top $(DFF) $(IMG)
	@-rm -rf ./build/Vysyx_23060020_top 

vrun:Vysyx_23060020_top bin
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@valgrind  --track-origins=yes --leak-check=full ./build/Vysyx_23060020_top $(DFF) $(IMG)
	@-rm -rf ./build/Vysyx_23060020_top 

gdb:Vysyx_23060020_top
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@gdb ./build/Vysyx_23060020_top --quiet
	@-rm -rf ./build/Vysyx_23060020_top 

bin:
	$(MAKE) -C $(BINPATH) ARCH=riscv32e-npc ALL=$(ALL)

Vysyx_23060020_top:
	@verilator $(VERILATOR_FLAGSVERILATOR_FLAGS) $(CSRCS) --top-module ysyx_23060020_top $(VSRCS) -LDFLAGS $(LDFLAGS)
	@$(MAKE) -C $(NEMU_HOME)

clean:
	-rm -rf build

include ../Makefile


