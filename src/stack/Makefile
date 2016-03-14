OBJS = main.o stack.o stack_prob_1.o stack_prob_2.o stack_prob_3.o stack_prob_4.o stack_prob_6.o 

stack:
	cc -g -O0 -c -o main.o main.c
	cc -g -O0 -c -o stack.o stack.c
	cc -g -O0 -c -o stack_prob_1.o stack_prob_1.c
	cc -g -O0 -c -o stack_prob_2.o stack_prob_2.c
	cc -g -O0 -c -o stack_prob_3.o stack_prob_3.c
	cc -g -O0 -c -o stack_prob_4.o stack_prob_4.c
	cc -g -O0 -c -o stack_prob_6.o stack_prob_6.c

clean:
	rm -rf stack *.o

install: $(OBJS)
	gcc -g -O0 -o $@ $(OBJS) -o stack

