dll:
	cc -g -O0 -c -o main.o main.c
	cc -g -O0 -c -o dll.o dll.c
clean:
	rm -rf dll *.o

install:
	gcc -g -O0 main.o dll.o -o dll

