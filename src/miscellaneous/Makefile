OBJS = pattern_detect.o 2d_arr.o resize_arr.o drop_duplicates.o bitwise.o main.o

all:
	cc -g -O0 -c -o 2d_arr.o 2d_arr.c
	cc -g -O0 -c -o pattern_detect.o pattern_detect.c
	cc -g -O0 -c -o resize_arr.o resize_arr.c
	cc -g -O0 -c -o drop_duplicates.o drop_duplicates.c
	cc -g -O0 -c -o bitwise.o bitwise.c
	cc -g -O0 -c -o main.o main.c

clean:
	rm -rf misc *.o

install: $(OBJS)
	gcc -g -O0 -o $@ $(OBJS) -o misc

