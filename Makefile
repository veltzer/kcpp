##############
# parameters #
##############

# fill in the object files which are part of the module
obj-m:=kcpp.o
kcpp-objs:=top.o ser_mem.o ser_print.o
# fill in any extra compiler flags
EXTRA_CFLAGS+=-Werror -I.
# fill in the name of the module
name:=kcpp
# fill in the name of the genrated ko file
ko-m:=$(name).ko
# fill in the version of the kernel for which you want the module compiled to
KVER?=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR:=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
V?=0

##############
# code start #
##############
ifeq ($(V),1)
Q=
# we are not silent in this branch
else # DO_MKDBG
Q=@
#.SILENT:
endif # DO_MKDBG

#SOURCES_ALL:=$(shell git ls-files)
SOURCES_ALL:=$(filter-out %.mod.c,$(shell find . -name "*.cc" -or -name "*.c"))
CC_SOURCES:=$(filter %.cc,$(SOURCES_ALL))
#CC_SOURCES:=$(shell find . -name "*.cc")
CC_OBJECTS:=$(addsuffix .o,$(basename $(CC_SOURCES)))
#C_SOURCES:=$(shell find . -name "*.c")
C_SOURCES:=$(filter %.c,$(SOURCES_ALL))
C_OBJECTS:=$(addsuffix .o,$(basename $(C_SOURCES)))
#top-objs:=$(C_OBJECTS)

# this rule was also taken from running with V=1
$(ko-m): top.o top.mod.o $(CC_OBJECTS) checkpatch
	$(Q)ld -r -m elf_i386 --build-id -o $(ko-m) $(C_OBJECTS) kcpp.mod.o $(CC_OBJECTS)
# how was this monstrosity created?
# I ran the build with V=1 and registered the command to compile via gcc.
# picked the same version g++ and gave it the entire flag set (especially the -f stuff).
# removed all -D, preprocessor and
# -ffreestanding -Wno-pointer-sign -Wdeclaration-after-statement
# -Werror-implicit-function-declaration -Wstrict-prototypes
# which are not relevant to C++ (the compiler told me so!)
%.o: %.cc
	$(Q)g++ -nostdinc -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -fno-stack-protector -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -fomit-frame-pointer -Werror -c -o $@ $<
top.o top.mod.o: top.c
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules
	$(Q)-rm -f top.ko
.PHONY: modules
modules:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules
.PHONY: modules_install
modules_install:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules_install
.PHONY: clean
clean:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
.PHONY: help
help:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) help
.PHONY: insmod
insmod:
	$(Q)sudo insmod $(ko-m) 
.PHONY: lsmod
lsmod:
	$(Q)sudo lsmod | grep $(name)
.PHONY: rmmod
rmmod:
	$(Q)sudo rmmod $(name)
.PHONY: modinfo
modinfo:
	$(Q)sudo modinfo $(ko-m)
.PHONY: last
last:
	$(Q)sudo tail /var/log/kern.log
.PHONY: log
log:
	$(Q)sudo tail -f /var/log/kern.log
.PHONY: cleanlog
cleanlog:
	$(Q)sudo dmesg -c > /dev/null
.PHONY: halt
halt:
	$(Q)sudo halt
.PHONY: reboot
reboot:
	$(Q)sudo reboot
.PHONY: tips
tips:
	$(Q)echo "do make V=1 [target] to see more of what is going on"
	$(Q)echo
	$(Q)echo "in order for the operational targets to work you need to"
	$(Q)echo "make sure that can do 'sudo', preferably with no password."
	$(Q)echo "one way to do that is to add yourself to the 'sudo' group"
	$(Q)echo "and add to the /etc/sudoers file, using visudo, the line:"
	$(Q)echo "%sudo ALL=NOPASSWD: ALL"
	$(Q)echo
	$(Q)echo "you can compile your module to a different kernel version"
	$(Q)echo "like this: make KVER=2.6.13 [target]"
	$(Q)echo "or edit the file and permanently change the version"
.PHONY: debug
debug:
	$(info V is $(V))
	$(info CURDIR is $(CURDIR))
	$(info KVER is $(KVER))
	$(info KDIR is $(KDIR))
	$(info SOURCES_ALL is $(SOURCES_ALL))
	$(info CC_SOURCES is $(CC_SOURCES))
	$(info CC_OBJECTS is $(CC_OBJECTS))
	$(info C_SOURCES is $(C_SOURCES))
	$(info C_OBJECTS is $(C_OBJECTS))
.PHONY: checkpatch
checkpatch:
	$(info doing [$@])
	$(Q)scripts/wrapper.py ~/install/linux-3.6.3/scripts/checkpatch.pl --file top.c --root ~/install/linux-3.6.3
	$(Q)scripts/wrapper.py ~/install/linux-3.6.3/scripts/checkpatch.pl --file ser_mem.c --root ~/install/linux-3.6.3
	$(Q)scripts/wrapper.py ~/install/linux-3.6.3/scripts/checkpatch.pl --file ser_print.c --root ~/install/linux-3.6.3
