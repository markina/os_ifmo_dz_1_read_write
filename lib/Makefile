all:
	gcc -o helpers.o -c -fPIC helpers.c
	gcc -shared -o libhelper.so helpers.o -lm 
	rm helpers.o