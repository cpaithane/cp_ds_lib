all:
	cc -g -O0 -c -o sll_tc.o sll_tc.c
	cc -g -O0 -c -o sll.o sll.c
	cc -g -O0 -c -o sll_prob_1.o sll_prob_1.c
	cc -g -O0 -c -o sll_prob_2.o sll_prob_2.c
clean:
	rm -rf sll *.o

install:
	gcc -g -O0 sll_tc.o sll.o -o sll

