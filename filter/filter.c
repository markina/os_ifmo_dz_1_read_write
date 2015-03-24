#include "../lib/helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE_BUF 8192
char DELIMITER = '\n';

int main(int argc, char *argv[]) {
    
    //char* args[] = {"tar", "-cf", "filter.tar", "/bin/sh", NULL};
	//int res = spawn("tar", args);
	//res == 0 <--> ok


	if(argc < 2) {
		printf("USAGE: ./filter [PATH] [OPTION]...\n");
        exit(0);
	}
	char * nameProgram = argv[1];

	for(int i = 0; i < argc - 1; i++) {
		argv[i] = argv[i + 1];
	}




	argv[argc - 1] = "/bin";
	argv[argc + 1] = NULL;

	int res = spawn(nameProgram, argv);

    return 0;
}
