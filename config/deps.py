import os # for uname

packages=[

uname=os.uname()[2]
packages=[
    f"linux-headers-{uname}",
]
