all:
	cc -g -O0 -c -o dll_tc.o dll_tc.c
	cc -g -O0 -c -o dll.o dll.c
	cc -g -O0 -c -o dll_prob_1.o dll_prob_1.c
clean:
	rm -rf dll *.o

install:
	gcc -g -O0 dll_tc.o dll.o -o dll

