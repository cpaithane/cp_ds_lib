all:
	cc -g -O0 -c -o queue_tc.o queue_tc.c
	cc -g -O0 -c -o queue.o queue.c
clean:
	rm -rf queue *.o

install:
	gcc -g -O0 queue_tc.o queue.o -o queue

