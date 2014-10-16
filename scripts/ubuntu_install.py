#!/usr/bin/python3

# this script will install all the required packages that you need on
# ubuntu to compile and work with this package.

import subprocess # for check_call
import os # for uname

uname=os.uname()[2]

packs=[
	'linux-headers-{0}'.format(uname),
]

args=['sudo','apt-get','install','--assume-yes']
args.extend(packs)
subprocess.check_call(args)
