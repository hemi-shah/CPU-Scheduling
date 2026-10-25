#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    FILE *file;
        
    char userInput[1024];
    char taskName[1024];
    int priority = 0;
    int burstTime = 0;
    int queueNum = 0;
    char *token;

    // stores the number of arguments entered
    int nargs = argc - 1;

    // check to see if input file is provided
    if (nargs == 1) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Could not open file.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Please provide an input filename.\n");
        return 1;
    }

    // parse through user input
    while (fgets(userInput, sizeof(userInput), file) != NULL){
        token = strtok(userInput, ",");

        if (token != NULL) {
            snprintf(taskName, sizeof(taskName), "%s", token);
        }

        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                if (i == 0){
                    priority = atoi(token);
                } else if (i == 1){
                    burstTime = atoi(token);
                } else if (i == 2) {
                    queueNum = atoi(token);
                }
            }
        }
        printf("Task: %s, Priority: %d, Burst: %d, Queue: %d\n", taskName, priority, burstTime, queueNum);
    }
}
