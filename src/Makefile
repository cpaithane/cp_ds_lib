SHELL=/bin/bash
SUBDIRS := common bst dll queue scll sll stack avl_tree heap graph trie b_plus_tree \
	rb_tree

.PHONY : all $(SUBDIRS)

all: $(SUBDIRS)
	cc -g -O0 -c -o main.o main.c
$(SUBDIRS):
	make -C $@ clean all

install:
	python create_ds_lib.py
	gcc -g -O0 bin/*.o -o cp_ds_lib
	mv cp_ds_lib bin/.

