#! /usr/bin/python

import os
import shutil

SUBDIRS = ["bst", "dll", "queue", "scll", "sll", "stack"]

shutil.rmtree("bin")
os.mkdir("bin")

for subdir in SUBDIRS:
	os.system("cp -f %s/*.o bin/." % (subdir))

os.system("cp -f main.o bin/.")
