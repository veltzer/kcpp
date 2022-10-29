""" os level dependencies for this project """

import os

uname=os.uname()[2]
packages=[
    f"linux-headers-{uname}",
]
