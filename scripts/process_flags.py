#!/usr/bin/python

'''
This is a script that processes flags from linux kernel compilation and
produces the flags for c++ compilation
'''

'''
General guide to compiler flags (this is true in MOST cases, not in ALL
cases, check each flag...)
flags starting with -f or -m: affect code generation
flags starting with -W: produce more warnings, don't affect code generation
'''

import subprocess # for check_output
import sys # for argv, exit

##############
# parameters #
##############
doPassKdir=True
doClean=True

#############
# functions #
#############
def remove_begin_with(l, s):
	found=False
	res=[]
	for x in l:
		if x.startswith(s):
			found=True
		else:
			res.append(x)
	if not found:
		raise ValueError('could not find stuff begining with', s)
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

def remove_if_exists(l, s):
	if s in l:
		l.remove(s)
	return l

def find_ends_with(l, ending):
	found_count=0
	for x in l:
		if x.endswith(ending):
			found_count+=1
			found=x
	if found_count==1:
		return found
	raise ValueError('found too many or too little', found_count)

########
# code #
########
if len(sys.argv)!=3:
	print('usage: kdir outfile')
	sys.exit(1)
kdir=sys.argv[1]
outfile=sys.argv[2]

args=[
	'/usr/bin/make',
	'-C','std_module',
	'V=1',
]
if doPassKdir:
	args.append('KDIR={kdir}'.format(kdir=kdir))
if doClean:
	clean_args=list(args)
	clean_args.append('clean')
	output=subprocess.check_output(clean_args)
output=subprocess.check_output(args)
# split into lines and find the line that ends with 'main.c'
lines=output.split('\n')
line=find_ends_with(lines,'main.c')
line=line.strip()
l=line.split()
# remove the last component (which is the source file name, see above)
l=l[:-1]
# header file stuff, does not influence code generation
#l=remove(l,'-nostdinc')
# C++ code does not reference any header files of the kernel,
# operating system, or compiler...
l=remove_begin_with(l,'-I')
# remove macros of the kernel (which we don't use in the cpp layer)
l=remove_begin_with(l,'-D')
# remove the -Wp,MD
l=remove_begin_with(l,'-Wp,-MD')
# remove -include and -isystem (which we don't use in the cpp layer)
l=remove_two_in_a_row(l, '-isystem')
l=remove_two_in_a_row(l, '-include')
l=remove_two_in_a_row(l, '-o')
# if the kernel was compiled with debug and profiling then we don't
# need it
l.remove('-pg')
l.remove('-c')
l.remove('gcc')
#l.remove('-g')
'''
remove flags which are not valid for C++...
cc1plus: warning: command line option '-Wstrict-prototypes' is valid for C/ObjC but not for C++ [enabled by default]
cc1plus: warning: command line option '-Wdeclaration-after-statement' is valid for C/ObjC but not for C++ [enabled by default]
cc1plus: warning: command line option '-Wno-pointer-sign' is valid for C/ObjC but not for C++ [enabled by default]
'''
l=remove_if_exists(l, '-Wstrict-prototypes')
l=remove_if_exists(l, '-Wdeclaration-after-statement')
l=remove_if_exists(l, '-Wno-pointer-sign')
l=remove_if_exists(l, '-Wmissing-prototypes')
l=remove_if_exists(l, '-Wold-style-definition')
l=remove_if_exists(l, '-std=gnu90')
l=remove_if_exists(l, '-std=gnu89')
with open(outfile, 'w') as f:
	f.write(' '.join(l))
