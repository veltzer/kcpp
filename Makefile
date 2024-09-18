##############
# parameters #
##############
# fill in the name of the module
name:=kcpp
# fill in the object files which are part of the module
obj-m:=$(name).o
$(name)-objs:=top.o ser_mem.o ser_print.o ser_empty.o dummy.o
# fill in any extra compiler flags
EXTRA_CFLAGS+=-Werror -I.
# fill in the name of the genrated ko file
ko-m:=$(name).ko
# fill in the version of the kernel for which you want the module compiled to
KVER:=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR:=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
V:=0
# do you want to use checkpatch?
DO_CHECKPATCH:=1
# flags file
FLAGS:=flags.cfg
# do you want dependency on the Makefile itself ?
DO_ALLDEP:=1
# do you want to check python scripts?
DO_PYLINT:=1

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

ALL:=$(ko-m)

ifeq ($(DO_PYLINT),1)
ALL:=$(ALL) out/pylint.stamp
endif # DO_PYLINT

SOURCES_ALL:=$(filter-out %.mod.c,$(shell find . -maxdepth 1 -name "*.cc" -or -name "*.c"))
CC_SOURCES:=$(filter %.cc,$(SOURCES_ALL))
CC_OBJECTS:=$(addsuffix .o,$(basename $(CC_SOURCES)))
C_SOURCES:=$(filter %.c,$(SOURCES_ALL))
C_SOURCES:=$(filter-out ./dummy.c,$(C_SOURCES))
C_OBJECTS:=$(addsuffix .o,$(basename $(C_SOURCES)))
ALL_PY:=$(shell find . -type f -not -path "./.venv/*" -name "*.py")

# this rule was also taken from running with V=1
KO_ING:=$(C_OBJECTS) $(name).mod.o $(CC_OBJECTS)
KO_DEPS:=$(ko-m) $(KO_ING)
ifeq ($(DO_CHECKPATCH),1)
KO_DEPS:=$(KO_DEPS) checkpatch.stamp
endif # DO_CHECKPATCH

# pattern for compiling the c++ parts
%.o: %.cc $(FLAGS)
	$(info doing [$@])
	$(Q)g++ `cat $(FLAGS)` -Wall -Werror -c -o $@ $<

.PHONY: all
all: $(ALL)
	@true

$(ko-m): $(CC_OBJECTS)
	$(info doing [$@])
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules
	$(Q)$(info relinking the module with the C++ parts)
	$(Q)ld -r --build-id -o $(ko-m) $(KO_ING)

$(FLAGS): scripts/process_flags.py
	$(info doing [$@])
	$(Q)scripts/process_flags.py $(KDIR) $@

checkpatch.stamp: $(C_SOURCES)
	$(info doing [$@])
	$(Q)$(KDIR)/scripts/checkpatch.pl --file $(C_SOURCES) --no-tree > /dev/null 2> /dev/null
	$(Q)touch $@

.PHONY: modules_install
modules_install:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules_install

.PHONY: clean
clean:
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
	$(Q)rm -f checkpatch.stamp $(FLAGS)

.PHONY: clean_hard
clean_hard:
	$(info doing [$@])
	$(Q)git clean -qffxd

.PHONY: help
help:
	$(info doing [$@])
	$(Q)$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) help

.PHONY: insmod
insmod:
	$(info doing [$@])
	$(Q)sudo insmod $(ko-m)

.PHONY: lsmod
lsmod:
	$(info doing [$@])
	$(Q)sudo lsmod | grep $(name)

.PHONY: rmmod
rmmod:
	$(info doing [$@])
	$(Q)sudo rmmod $(name)

.PHONY: modinfo
modinfo:
	$(info doing [$@])
	$(Q)modinfo $(ko-m)

.PHONY: last
last:
	$(info doing [$@])
	$(Q)sudo tail /var/log/kern.log

.PHONY: log
log:
	$(info doing [$@])
	$(Q)sudo tail -f /var/log/kern.log

.PHONY: cleanlog
cleanlog:
	$(info doing [$@])
	$(Q)sudo dmesg -c > /dev/null

.PHONY: halt
halt:
	$(info doing [$@])
	$(Q)sudo halt

.PHONY: reboot
reboot:
	$(info doing [$@])
	$(Q)sudo reboot

.PHONY: tips
tips:
	$(info doing [$@])
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
	$(info doing [$@])
	$(info name is $(name))
	$(info ko-m is $(ko-m))
	$(info V is $(V))
	$(info CURDIR is $(CURDIR))
	$(info KVER is $(KVER))
	$(info KDIR is $(KDIR))
	$(info SOURCES_ALL is $(SOURCES_ALL))
	$(info CC_SOURCES is $(CC_SOURCES))
	$(info CC_OBJECTS is $(CC_OBJECTS))
	$(info C_SOURCES is $(C_SOURCES))
	$(info C_OBJECTS is $(C_OBJECTS))
	$(info KO_DEPS is $(KO_DEPS))
	$(info ALL is $(ALL))
	$(info ALL_PY is $(ALL_PY))

.PHONY: pylint
pylint: out/pylint.stamp
out/pylint.stamp: $(ALL_PY)
	$(Q)pylint --reports=n --score=n $(ALL_PY)
	$(Q)pymakehelper touch_mkdir $@

.PHONY: test_stress_insmod_rmmod
test_stress_insmod_rmmod: $(ko-m)
	$(info doing [$@])
	-sudo rmmod $(name)
	./scripts/test_stress_insmod_rmmod.pl $(ko-m) 1000

##########
# alldep #
##########
ifeq ($(DO_ALLDEP),1)
.EXTRA_PREREQS+=$(foreach mk, ${MAKEFILE_LIST},$(abspath ${mk}))
endif # DO_ALLDEP
