#!/usr/bin/env python3

# a script that rerun the command when it fails

import os
from time import sleep as sl

a = input("type the commend: ")

while True:
    status = os.system(a)
    return_value = os.waitstatus_to_exitcode(status)
    if return_value == 0:
        print(f"return_value is {return_value}, done.")
        break
    print(f"return_value is {return_value}.")
    sl(1/3)
