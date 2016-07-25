OBJS = 2d_arr.o main.o

all:
	cc -g -O0 -c -o 2d_arr.o 2d_arr.c
	cc -g -O0 -c -o main.o main.c

clean:
	rm -rf misc *.o

install: $(OBJS)
	gcc -g -O0 -o $@ $(OBJS) -o misc

