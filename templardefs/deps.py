'''
dependencies for this project
'''
import os # for uname

def populate(d):
    uname=os.uname()[2]
    d.packs=[
        'linux-headers-{0}'.format(uname),
    ]

def getdeps():
    return [
        __file__, # myself
    ]
