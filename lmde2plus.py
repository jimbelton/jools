#!/usr/bin/python

import os
import sys

def install(package):
    if os.system("sudo apt-get -y install " + package):
	sys.exit("sudo apt-get -y install {} failed; have you set up the wireless password?".format(package))
    
upgrade = False

if len(sys.argv) > 1 and sys.argv[1] == "-u":
   sys.argv = sys.argv[1:]
   upgrade  = True

if len(sys.argv) != 1:
    sys.exit("./lmde2plus.py [-u] idempotently configures lmde2 and installs packages")

os.system("sudo apt-get update")

if upgrade:
    os.system("sudo apt-get upgrade") == 0 or sys.exit("sudo apt-get upgrade failed; have you set up the wireless password?")

install("geany")
install("git")
