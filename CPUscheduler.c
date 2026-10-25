#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char taskName[20];
    int priority;
    int burstTime;
    int remainingTime;
    int queueNum;
    int completionTime;
    int firstExecutionTime;
} Task;

int main(int argc, char *argv[]){

    FILE *file;
        
    char userInput[1024];
    Task task;
    char *token;

    // queue 1
    Task realTimeQueue[100];
    // queue 2
    Task systemQueue[100];
    // queue 3
    Task interactiveQueue[100];
    // queue 4
    Task batchQueue[100];

    // counters for each queue
    int realTimeSize = 0;
    int systemSize = 0;
    int interactiveSize = 0;
    int batchSize = 0;

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
            snprintf(task.taskName, sizeof(task.taskName), "%s", token);
        }

        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                if (i == 0){
                    task.priority = atoi(token);
                } else if (i == 1){
                    task.burstTime = atoi(token);
                } else if (i == 2) {
                    task.queueNum = atoi(token);
                }
            }
        }
        printf("Task: %s, Priority: %d, Burst: %d, Queue: %d\n", task.taskName, task.priority, task.burstTime, task.queueNum);

        // initialize execution fields for task
        task.remainingTime = task.burstTime;
        task.completionTime = 0;
        task.firstExecutionTime = -1;

        if (task.queueNum == 1) {
            // inserts task at index then increments counter
            realTimeQueue[realTimeSize++] = task;
        } else if (task.queueNum == 2) {
            systemQueue[systemSize++] = task;
        } else if (task.queueNum == 3) {
            interactiveQueue[interactiveSize++] = task;
        } else if (task.queueNum == 4) {
            batchQueue[batchSize++] = task;
        }
    }

    // THIS PART IS AI GENERATED JUST TO VERIFY THE OUTPUTS AND WILL NOT BE INCLUDED IN THE FINAL SUBMISSION **TEMP ONLY**
    // print out each queues tasks
    printf("\n===== Real-Time Queue (Queue 1) =====\n");
    for (int i = 0; i < realTimeSize; i++) {
        printf("Task: %s, Priority: %d, Burst: %d, Remaining: %d\n",
            realTimeQueue[i].taskName,
            realTimeQueue[i].priority,
            realTimeQueue[i].burstTime,
            realTimeQueue[i].remainingTime);
    }

    printf("\n===== System Queue (Queue 2) =====\n");
    for (int i = 0; i < systemSize; i++) {
        printf("Task: %s, Priority: %d, Burst: %d, Remaining: %d\n",
            systemQueue[i].taskName,
            systemQueue[i].priority,
            systemQueue[i].burstTime,
            systemQueue[i].remainingTime);
    }

    printf("\n===== Interactive Queue (Queue 3) =====\n");
    for (int i = 0; i < interactiveSize; i++) {
        printf("Task: %s, Priority: %d, Burst: %d, Remaining: %d\n",
            interactiveQueue[i].taskName,
            interactiveQueue[i].priority,
            interactiveQueue[i].burstTime,
            interactiveQueue[i].remainingTime);
    }

    printf("\n===== Batch Queue (Queue 4) =====\n");
    for (int i = 0; i < batchSize; i++) {
        printf("Task: %s, Priority: %d, Burst: %d, Remaining: %d\n",
            batchQueue[i].taskName,
            batchQueue[i].priority,
            batchQueue[i].burstTime,
            batchQueue[i].remainingTime);
    }
    // END OF AI GENERATED CODE

    // close the input file
    fclose(file);
    return 0;
}
