# Kernel build description for the kcpp module. Kbuild reads this file when
# rsconstruct runs `make -C <kernel build dir> M=<this dir> modules`; the
# C++ parts are compiled and linked in afterwards by scripts/build_kcpp.py.
obj-m := kcpp.o
kcpp-objs := top.o ser_mem.o ser_print.o ser_empty.o dummy.o
EXTRA_CFLAGS += -Werror -I.
