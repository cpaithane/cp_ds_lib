OBJS = stack_tc.o stack.o stack_prob_1.o stack_prob_2.o stack_prob_3.o stack_prob_4.o stack_prob_5.o stack_prob_6.o stack_prob_7.o stack_prob_8.o stack_prob_9.o stack_prob_10.o stack_prob_11.o stack_prob_12.o

all:
	cc -g -O0 -c -o stack_tc.o stack_tc.c
	cc -g -O0 -c -o stack.o stack.c
	cc -g -O0 -c -o stack_prob_1.o stack_prob_1.c
	cc -g -O0 -c -o stack_prob_2.o stack_prob_2.c
	cc -g -O0 -c -o stack_prob_3.o stack_prob_3.c
	cc -g -O0 -c -o stack_prob_4.o stack_prob_4.c
	cc -g -O0 -c -o stack_prob_5.o stack_prob_5.c
	cc -g -O0 -c -o stack_prob_6.o stack_prob_6.c
	cc -g -O0 -c -o stack_prob_7.o stack_prob_7.c
	cc -g -O0 -c -o stack_prob_8.o stack_prob_8.c
	cc -g -O0 -c -o stack_prob_9.o stack_prob_9.c
	cc -g -O0 -c -o stack_prob_10.o stack_prob_10.c
	cc -g -O0 -c -o stack_prob_11.o stack_prob_11.c
	cc -g -O0 -c -o stack_prob_12.o stack_prob_12.c

clean:
	rm -rf stack *.o

install: $(OBJS)
	gcc -g -O0 -o $@ $(OBJS) -o stack

