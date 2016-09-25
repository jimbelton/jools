#!/usr/bin/python

import os
import re
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

# Configure git

email     = False
name      = False
gitconfig = os.path.expanduser("~/.gitconfig")
open(gitconfig, "a").close()    # Create file if not there

with open(gitconfig, "r") as config:
    email = False
    name  = False
    line  = config.readline()

    while line and not re.search(r'\[user\]', line):
	line = config.readline()

    line  = config.readline()

    while line and not re.match(r'\s*\[[^\]]+\]\s*$', line):
	email = email or re.match(r'\s+email\s*=', line)
	name  = name  or re.match(r'\s+name\s*=', line)
	line  = config.readline()

if not email:
    email = raw_input("Enter your email for git:")
    os.system("git config --global user.email '{}'".format(email))

if not name:
    name = raw_input("Enter Your Name for git:")
    os.system("git config --global user.name '{}'".format(name))

