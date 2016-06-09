#! /usr/bin/python

import os
import shutil

SUBDIRS = ["common", "avl_tree", "bst", "dll", "queue", "scll", "sll", "stack",
	   "heap", "graph", "trie", "b_plus_tree", "rb_tree"]

if os.path.exists("bin"):
	shutil.rmtree("bin")

os.mkdir("bin")

for subdir in SUBDIRS:
	os.system("cp -f %s/*.o bin/." % (subdir))

os.system("cp -f main.o bin/.")

