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
void recordExecution(Task *task, int executionTime);
void priorityAlgo(Task queue[], int size);
void SJFAlgo(Task queue[], int size);
void roundRobinAlgo(Task queue[], int size);
void FCFSAlgo(Task queue[], int size);
void printGantt();
void printStatistics(Task tasks[], int size);

char ganttTasks[400][20];
int ganttTimes[401];
int ganttSize = 0;
int currentTime = 0;

int main(int argc, char *argv[]) {

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
    while (fgets(userInput, sizeof(userInput), file) != NULL) {
        if (userInput[0] == '\n') continue;

        token = strtok(userInput, ",");

        if (token != NULL) {
            snprintf(task.taskName, sizeof(task.taskName), "%s", token);
        } else {
            continue;
        }

        int valid = 1;

        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                if (i == 0) {
                    task.priority = atoi(token);
                } else if (i == 1) {
                    task.burstTime = atoi(token);
                } else if (i == 2) {
                    task.queueNum = atoi(token);
                }
            } else {
                valid = 0;
                break;
            }
        }

        if (!valid) continue;

        // DEBUG: printf("Task: %s, Priority: %d, Burst: %d, Queue: %d\n",

        // initialize execution fields for task
        task.remainingTime = task.burstTime;
        task.completionTime = 0;
        task.firstExecutionTime = -1;
        task.arrivalTime = 0;

        // initialize statistics
        task.turnaroundTime = 0;
        task.waitingTime = 0;
        task.responseTime = 0;

        // check that task information is valid
        if (task.priority < 1 || task.priority > 10 ||
            task.burstTime <= 0 ||
            task.queueNum < 1 || task.queueNum > 4) {
            fprintf(stderr, "Invalid task: %s\n", task.taskName);
            fclose(file);
            return 1;
        }

        if (task.queueNum == 1) {
            if (realTimeSize >= 100) {
                fprintf(stderr, "Queue 1 is full.\n");
                fclose(file);
                return 1;
            }
            // inserts task at index then increments counter
            realTimeQueue[realTimeSize++] = task;
        } else if (task.queueNum == 2) {
            if (systemSize >= 100) {
                fprintf(stderr, "Queue 2 is full.\n");
                fclose(file);
                return 1;
            }
            systemQueue[systemSize++] = task;
        } else if (task.queueNum == 3) {
            if (interactiveSize >= 100) {
                fprintf(stderr, "Queue 3 is full.\n");
                fclose(file);
                return 1;
            }
            interactiveQueue[interactiveSize++] = task;
        } else if (task.queueNum == 4) {
            if (batchSize >= 100) {
                fprintf(stderr, "Queue 4 is full.\n");
                fclose(file);
                return 1;
            }
            batchQueue[batchSize++] = task;
        }
    }

    // close the input file
    fclose(file);

    // runs each queue in priority order
    priorityAlgo(realTimeQueue, realTimeSize);
    SJFAlgo(systemQueue, systemSize);

    // run interactive and batch queues
    roundRobinAlgo(interactiveQueue, interactiveSize);
    FCFSAlgo(batchQueue, batchSize);

    // print out gantt chart of queue
    printGantt();

    // print values for every queue
    printf("\n===== Scheduling Statistics =====\n");
    printf("%-10s %-15s %-15s %-15s\n",
           "Task", "Turnaround", "Waiting", "Response");

    printStatistics(realTimeQueue, realTimeSize);
    printStatistics(systemQueue, systemSize);
    printStatistics(interactiveQueue, interactiveSize);
    printStatistics(batchQueue, batchSize);

    // calculate averages for all tasks
    int totalTasks = 0;
    double totalTurnaround = 0;
    double totalWaiting = 0;
    double totalResponse = 0;

    Task *queues[] = {
        realTimeQueue, systemQueue,
        interactiveQueue, batchQueue
    };

    int sizes[] = {
        realTimeSize, systemSize,
        interactiveSize, batchSize
    };

    for (int q = 0; q < 4; q++) {
        for (int i = 0; i < sizes[q]; i++) {
            totalTurnaround += queues[q][i].turnaroundTime;
            totalWaiting += queues[q][i].waitingTime;
            totalResponse += queues[q][i].responseTime;
            totalTasks++;
        }
    }

    if (totalTasks > 0) {
        printf("\nAverage Turnaround Time: %.2f ms\n",
               totalTurnaround / totalTasks);
        printf("Average Waiting Time: %.2f ms\n",
               totalWaiting / totalTasks);
        printf("Average Response Time: %.2f ms\n",
               totalResponse / totalTasks);
    }

    return 0;
}

void priorityAlgo(Task queue[], int size) {
    // sort queue 1
    sortPriority(queue, size);

    // add executions using printGantt helper function
    for (int i = 0; i < size; i++) {
        recordExecution(&queue[i], queue[i].remainingTime);
    }
}

void SJFAlgo(Task queue[], int size) {
    // sort queue 2
    sortBurst(queue, size);

    // add executions using printGantt helper function
    for (int i = 0; i < size; i++) {
        recordExecution(&queue[i], queue[i].remainingTime);
    }
}

// round robin scheduling for queue 3
void roundRobinAlgo(Task queue[], int size) {
    int unfinished = size;

    // keep running until every task finishes
    while (unfinished > 0) {
        for (int i = 0; i < size; i++) {

            if (queue[i].remainingTime > 0) {
                int executionTime;

                // tasks can run for a maximum of 10 ms
                if (queue[i].remainingTime < 10) {
                    executionTime = queue[i].remainingTime;
                } else {
                    executionTime = 10;
                }

                recordExecution(&queue[i], executionTime);

                // check if the task finished
                if (queue[i].remainingTime == 0) {
                    unfinished--;
                }
            }
        }
    }
}

// first come first served for queue 4
void FCFSAlgo(Task queue[], int size) {
    // tasks run in the order they were added
    for (int i = 0; i < size; i++) {
        recordExecution(&queue[i], queue[i].remainingTime);
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

// helper function to record execution of each entry
void recordExecution(Task *task, int executionTime) {
    if (ganttSize >= 400) {
        fprintf(stderr, "Gantt history is full.\n");
        exit(1);
    }

    strcpy(ganttTasks[ganttSize], task->taskName);

    ganttTimes[ganttSize] = currentTime;

    // record when task first starts
    if (task->firstExecutionTime == -1) {
        task->firstExecutionTime = currentTime;
    }

    // simulate CPU execution one millisecond at a time
    for (int i = 0; i < executionTime; i++) {
        task->remainingTime--;
        currentTime++;
    }

    ganttTimes[ganttSize + 1] = currentTime;
    ganttSize++;

    // calculate statistics when task finishes
    if (task->remainingTime == 0) {
        task->completionTime = currentTime;

        task->turnaroundTime =
            task->completionTime - task->arrivalTime;

        task->waitingTime =
            task->turnaroundTime - task->burstTime;

        task->responseTime =
            task->firstExecutionTime - task->arrivalTime;
    }
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

// print statistics for each task
void printStatistics(Task tasks[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%-10s %-15d %-15d %-15d\n",
               tasks[i].taskName,
               tasks[i].turnaroundTime,
               tasks[i].waitingTime,
               tasks[i].responseTime);
    }
}
