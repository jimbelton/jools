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
install("qbittorrent")
install("vim")

# Configure bash

#bashrc = os.path.expanduser("~/.bashrc")
#open(bashrc, "a").close()    # Create file if not there

# Configure git

if os.system("git config --global user.email > /dev/null"):
    email = raw_input("Enter your email for git:")
    os.system("git config --global user.email '{}'".format(email))

if os.system("git config --global user.name > /dev/null"):
    name = raw_input("Enter your first and last name for git:")
    os.system("git config --global user.name '{}'".format(name))
    
if os.system("git config --global core.editor > /dev/null"):
    os.system("git config --global core.editor vim")
    
# Make qbittorrent the default for magnet links. Should really check for
# an existing setting first.

os.system("gvfs-mime --set x-scheme-handler/magnet qBittorrent.desktop")

