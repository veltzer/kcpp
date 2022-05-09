import os # for uname

uname=os.uname()[2]
packages=[
    f"linux-headers-{uname}",
]
