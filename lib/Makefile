all:
	gcc -std=c99 -o helpers.o -c -fPIC helpers.c
	gcc -shared -o libhelpers.so helpers.o -lm 
	rm helpers.o
	gcc -std=c99 -o bufio.o -c -fPIC bufio.c
	gcc -shared -o libbufio.so bufio.o -lm 
	rm bufio.o
helpers:
	gcc -std=c99 -o helpers.o -c -fPIC helpers.c
	gcc -shared -o libhelpers.so helpers.o -lm 
	rm helpers.o
bufio_with_debug:
	gcc -DDEBUG -std=c99 -o bufio.o -c -fPIC bufio.c
	gcc -shared -o libbufio.so bufio.o -lm 
	rm bufio.o
rm:
	rm libhelpers.so
	rm libbufio.so