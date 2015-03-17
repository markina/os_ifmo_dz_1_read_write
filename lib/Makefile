all:
	gcc -std=c99 -o helpers.o -c -fPIC helpers.c
	gcc -shared -o libhelper.so helpers.o -lm 
	rm helpers.o
rm:
	rm libhelper.so