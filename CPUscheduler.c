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
    int arrivalTime;

    int turnaroundTime;
    int waitingTime;
    int responseTime;
} Task;

void sortPriority(Task queue[], int size);
void sortBurst(Task queue[], int size);
void recordExecution(char taskName[], int executionTime);
void priorityAlgo(Task queue[], int size);
void SJFAlgo(Task queue[], int size);
void printGantt();

char ganttTasks[400][20];
int ganttTimes[400];
int ganttSize = 0;
int currentTime = 0;

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
        if (userInput[0] == '\n') continue;
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
        // DEBUG: printf("Task: %s, Priority: %d, Burst: %d, Queue: %d\n", task.taskName, task.priority, task.burstTime, task.queueNum);

        // initialize execution fields for task
        task.remainingTime = task.burstTime;
        task.completionTime = 0;
        task.firstExecutionTime = 0;
        task.arrivalTime = 0;

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

    // close the input file
    fclose(file);

    priorityAlgo(realTimeQueue, realTimeSize);
    SJFAlgo(systemQueue, systemSize);

    // print out gantt chart of queue
    printGantt();

    return 0;
}

void priorityAlgo(Task queue[], int size) {
    // sort queue 1
    sortPriority(queue, size);

    // add executions using printGantt helper function
    for (int i = 0; i < size; i++){
        recordExecution(queue[i].taskName, queue[i].remainingTime);
    }
}

void SJFAlgo(Task queue[], int size){
    // sort queue 2
    sortBurst(queue, size);

    // add executions using printGantt helper function
    for (int i = 0; i < size; i++){
        recordExecution(queue[i].taskName, queue[i].remainingTime);
    }
}

// helper function to use insertion sort to dynamically sort the queue by priority
void sortPriority(Task queue[], int size) {
    for (int i = 1; i < size; i++) {
        Task temp = queue[i];
        int j = i - 1;

        while (j >= 0 && queue[j].priority < temp.priority) {
            queue[j + 1] = queue[j];
            j--;
        }

        queue[j + 1] = temp;
    }
}

// helper function to use insertion sort to dynamically sort the queue by burst
void sortBurst(Task queue[], int size) {
    for (int i = 1; i < size; i++) {
        Task temp = queue[i];
        int j = i - 1;

        while (j >= 0 && queue[j].burstTime > temp.burstTime) {
            queue[j + 1] = queue[j];
            j--;
        }

        queue[j + 1] = temp;
    }
}

// helper function to record exeuction of each entry
void recordExecution(char taskName[], int executionTime) {
    if (ganttSize >= 400) {
        fprintf(stderr, "Gantt history is full.\n");
        exit(1);
    }

    strcpy(ganttTasks[ganttSize], taskName);

    ganttTimes[ganttSize] = currentTime;
    currentTime += executionTime;
    ganttTimes[ganttSize + 1] = currentTime;

    ganttSize++;
}

// function to print the gantt chart
void printGantt() {
    printf("CPU Scheduling Gantt Chart:\n\n");

    if (ganttSize == 0) {
        printf("No tasks to display.\n");
        return;
    }

    for (int i = 0; i < ganttSize; i++) {
        printf("| %-2s ", ganttTasks[i]);
    }
    printf("|\n");

    for (int i = 0; i <= ganttSize; i++) {
        printf("%-5d", ganttTimes[i]);
    }
    printf("\n");

    printf("\nTotal CPU Execution Time: %d ms\n",
           ganttTimes[ganttSize] - ganttTimes[0]);
}