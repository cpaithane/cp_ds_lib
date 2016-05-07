all:
	cc -g -O0 -c -o heap_tc.o heap_tc.c
	cc -g -O0 -c -o heap.o heap.c
clean:
	rm -rf heap *.o

install:
	gcc -g -O0 heap_tc.o heap.o -o heap

