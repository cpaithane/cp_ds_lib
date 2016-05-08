all:
	cc -g -O0 -c -o graph_tc.o graph_tc.c
	cc -g -O0 -c -o graph.o graph.c
clean:
	rm -rf graph *.o

install:
	gcc -g -O0 graph_tc.o graph.o -o graph

