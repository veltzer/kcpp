#!/usr/bin/env python

""" Build the kcpp kernel module, replacing the old Makefile.

The module mixes C (built by Kbuild from the Kbuild file next to this
script's parent) with C++ that Kbuild cannot compile. The old Makefile did
this, and so does this script:
  1. scripts/process_flags.py derives C++ compiler flags from the kernel's
     own C flags into flags.cfg
  2. every .cc file is compiled with g++ using those flags
  3. Kbuild builds the C objects and kcpp.mod.o
  4. the module is relinked with `ld -r` so it also contains the C++ objects

The kernel build directory is the KDIR environment variable if set (as the
Makefile's KDIR could be), else the running kernel's headers if installed,
else the newest kernel headers present under /lib/modules (what the
linux-headers-generic package provides on a CI runner whose own kernel has
no headers package). """

import glob
import os
import subprocess
import sys

NAME = "kcpp"
# dummy.c only gives Kbuild's own link placeholder cpp_init/cpp_exit symbols;
# the real ones come from the C++ objects, so dummy.o is left out of the
# final relink exactly as the Makefile left it out.
C_OBJECTS = ["top.o", "ser_mem.o", "ser_print.o", "ser_empty.o"]
FLAGS = "flags.cfg"


def run(args):
    """ run a command, aborting the build on failure """
    ret = subprocess.call(args)
    if ret != 0:
        sys.exit(ret)


def kernel_build_dir():
    """ pick the kernel headers to build against """
    running = f"/lib/modules/{os.uname().release}/build"
    if os.path.isdir(running):
        return running
    candidates = sorted(d for d in glob.glob("/lib/modules/*/build") if os.path.isdir(d))
    if not candidates:
        sys.exit("no kernel headers found under /lib/modules; install linux-headers-generic")
    return candidates[-1]


def main():
    """ main entry point """
    kdir = os.environ.get("KDIR") or kernel_build_dir()
    run(["scripts/process_flags.py", kdir, FLAGS])
    with open(FLAGS, encoding="utf-8") as handle:
        flags = handle.read().split()
    cc_objects = []
    for source in sorted(glob.glob("*.cc")):
        obj = source[:-3] + ".o"
        run(["g++"] + flags + ["-Wall", "-Werror", "-c", "-o", obj, source])
        cc_objects.append(obj)
    run(["make", "-C", kdir, f"M={os.getcwd()}", "ARCH=x86_64",
         "CROSS_COMPILE=x86_64-linux-gnu-", "modules"])
    ko = f"{NAME}.ko"
    if os.path.exists(f"{NAME}.mod"):
        os.remove(f"{NAME}.mod")
    # newer kernels also link .module-common.o into every module
    common = [o for o in [".module-common.o"] if os.path.exists(o)]
    run(["ld", "-r", "--build-id", "-o", ko]
        + C_OBJECTS + [f"{NAME}.mod.o"] + common + cc_objects)


if __name__ == "__main__":
    main()
