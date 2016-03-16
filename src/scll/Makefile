all:
	cc -g -O0 -c -o scll_tc.o scll_tc.c
	cc -g -O0 -c -o scll.o scll.c
clean:
	rm -rf scll *.o

install:
	gcc -g -O0 scll_tc.o scll.o -o scll

