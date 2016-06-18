all:
	cc -g -O0 -c -o rb_tree_tc.o rb_tree_tc.c
	cc -g -O0 -c -o rb_tree.o rb_tree.c
	cc -g -O0 -c -o rb_tree_insert.o rb_tree_insert.c
clean:
	rm -rf rb_tree *.o

install:
	gcc -g -O0 rb_tree_tc.o rb_tree.o -o rb_tree

