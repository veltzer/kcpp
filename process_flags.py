#!/usr/bin/python

"""
This is a script that processes flags from linux kernel compilation and
produces the flags for c++ compilation
"""

"""
General guide to compiler flags (this is true in MOST cases, not in ALL
cases, check each flag...)
flags starting with -f or -m: affect code generation
flags starting with -W: produce more warnings, don't affect code generation
"""

def remove(l, flag):
	res=[]
	for x in l:
		if x!=flag:
			res.append(x)
	return res

def remove_begin_with(l, s):
	res=[]
	for x in l:
		if not x.startswith(s):
			res.append(x)
	return res

def remove_two_in_a_row(l, s):
	res=[]
	found=False
	for x in l:
		if x==s:
			found=True
		else:
			if found:
				found=False
			else:
				res.append(x)
	return res

s=open("~/flags.two").read()
s=s.strip()
l=s.split()
# header file stuff, does not influence code generation
#l=remove(l,"-nostdinc")
# C++ code does not reference any header files of the kernel,
# operating system, or compiler...
l=remove_begin_with(l,"-I")
# remove macros of the kernel (which we don't use in the cpp layer)
l=remove_begin_with(l,"-D")
# remove -include and -isystem (which we don't use in the cpp layer)
l=remove_two_in_a_row(l, "-isystem")
l=remove_two_in_a_row(l, "-include")
# if the kernel was compiled with debug and profiling then we don't
# need it
l=remove(l,"-pg")
l=remove(l,"-g")
"""
cc1plus: warning: command line option ‘-Wstrict-prototypes’ is valid for C/ObjC but not for C++ [enabled by default]
cc1plus: warning: command line option ‘-Wdeclaration-after-statement’ is valid for C/ObjC but not for C++ [enabled by default]
cc1plus: warning: command line option ‘-Wno-pointer-sign’ is valid for C/ObjC but not for C++ [enabled by default]
"""
l=remove(l,"-Wstrict-prototypes")
l=remove(l,"-Wdeclaration-after-statement")
l=remove(l,"-Wno-pointer-sign")
print " ".join(l)
#for x in l:
#	print x
