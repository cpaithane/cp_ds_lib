#! /usr/bin/python

import os
import shutil

BIN_PATH = "bin/cp_ds_lib"

SUBDIRS = ["avl_tree", "bst", "dll", "queue", "scll", "sll", "stack",
	   "heap", "graph", "trie", "b_plus_tree"]

def tc_run_valgrind() :

	ret = os.system("which valgrind > /dev/null")
	if ret != 0:
		return

	for subdir in SUBDIRS:
		cmd = "valgrind --leak-check=full %s %s > /tmp/valgrind.leak.%s 2>&1" \
			% (BIN_PATH, subdir, subdir)
		print "Checking for %s" % (subdir)
		print cmd
		os.system(cmd)

def main() :

	tc_run_valgrind()

main()
