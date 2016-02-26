queue:
	cc -g -O0 -c -o main.o main.c
	cc -g -O0 -c -o queue.o queue.c
clean:
	rm -rf queue *.o

install:
	gcc -g -O0 main.o queue.o -o queue

