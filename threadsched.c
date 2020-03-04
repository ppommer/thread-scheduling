#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "list.h"
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define ROUND_ROBIN 1
#define PRIORITY_ROUND_ROBIN 2
#define SHORTEST_REMAINING_TIME_NEXT 3

typedef struct threadInfo {
    int tNum;
    int tPrio;
    int tStart;
    int tRunTime;
    int tFullTime;
} threadInfo_t;

int cmpPointer(const void *p1, const void *p2) {
    return p1 - p2;
}

void printPointer(void *ptr) {
    printf("%p:\n\tThread Num: %d\n\tThread prio: %d\n", ptr, ((threadInfo_t *) ptr)->tNum,
           ((threadInfo_t *) ptr)->tPrio);
}

void print_time_step(int time, int thread_num) {
    static int first_time = 1;
    int i;

    if (first_time) {
        printf("  Time |  1  2  3  4  5  6  7  8  9  10\n");
        printf("-------+--------------------------------\n");
        first_time = 0;
    }
    printf("%06d |", time);
    if (thread_num) {
        for (i = 1; i < thread_num; i++)
            printf("   ");
        printf("  %d\n", thread_num);
    } else
        printf("\n");
}

void sortReadyQueueSRTN(list_t *ready) {
    list_t *resultList = list_init();

    threadInfo_t *currTI;
    threadInfo_t *min;
    struct list_elem *minElem;
    while (ready->first != NULL) {
        min = NULL;
        for (struct list_elem *curr = ready->first; curr != NULL; curr = curr->next) {
            currTI = curr->data;
            if (min == NULL || (currTI->tFullTime - currTI->tRunTime) < (min->tFullTime - min->tRunTime)) {
                min = currTI;
                minElem = curr;
            }
        }
        list_append(resultList, min);
        list_remove(ready, minElem);
    }

    ready->first = resultList->first;
    ready->last = resultList->last;
    free(resultList);

    //list_print(ready, printPointer);
}

list_t *waiting, *ready, *pReadys[10];

threadInfo_t *pollNextThread(int algo) {
    if (algo == ROUND_ROBIN || algo == SHORTEST_REMAINING_TIME_NEXT) {
        struct list_elem *lFirst = ready->first;
        if (lFirst != NULL) {
            threadInfo_t *nextT = lFirst->data;
            list_remove(ready, lFirst);
            return nextT;
        }
        return NULL;
    }
    if (algo == PRIORITY_ROUND_ROBIN) {
        for (int i = 0; i < 10; ++i) {
            struct list_elem *lFirst = pReadys[i]->first;
            if (lFirst != NULL) {
                threadInfo_t *nextT = lFirst->data;
                list_remove(pReadys[i], lFirst);
                return nextT;
            }
        }
        return NULL;
    }
    return NULL;
}

int readyThreads(int time, int algo) {
    int n = 0;
    struct list_elem *next;
    for (struct list_elem *curr = waiting->first; curr != NULL;) {
        threadInfo_t *cThreadInfo = ((threadInfo_t *) curr->data);
        if (cThreadInfo->tStart <= time) {
            next = curr->next;
            list_remove(waiting, curr);
            if (algo == PRIORITY_ROUND_ROBIN) {
                list_append(pReadys[cThreadInfo->tPrio], cThreadInfo);
            } else {
                list_append(ready, cThreadInfo);
            }
            n++;
            curr = next;
            //printf("Starting thread %d at time %6d\n", cThreadInfo->tNum, time);
        } else {
            curr = curr->next;
        }
    }
    if (n > 0 && algo == SHORTEST_REMAINING_TIME_NEXT) {
        sortReadyQueueSRTN(ready);
    }
    return n;
}

int threadLeft(int algo) {
    if (waiting->first != NULL) {
        return 1;
    }
    if (algo == PRIORITY_ROUND_ROBIN) {
        for (int i = 0; i < 10; ++i) {
            if (pReadys[i]->first != NULL) {
                return 1;
            }
        }
    } else if (algo == ROUND_ROBIN || algo == SHORTEST_REMAINING_TIME_NEXT) {
        if (ready->first != NULL) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[], char *envp[]) {

    //printf("Pid: %d", getpid());

    int nVal = -1;
    int timeStepVal = -1;
    int timeQVal = -1;
    int algoVal = -1;

    int c;

    while ((c = getopt(argc, argv, "n:t:q:a:")) != -1) {
        switch (c) {
            case 'n':
                nVal = atoi(optarg);
                break;
            case 't':
                timeStepVal = atoi(optarg);
                break;
            case 'q':
                timeQVal = atoi(optarg);
                break;
            case 'a':
                if (strcmp(optarg, "RR") == 0) {
                    algoVal = ROUND_ROBIN;
                } else if (strcmp(optarg, "PRR") == 0) {
                    algoVal = PRIORITY_ROUND_ROBIN;
                } else if (strcmp(optarg, "SRTN") == 0) {
                    algoVal = SHORTEST_REMAINING_TIME_NEXT;
                } else {
                    fprintf(stderr,
                            "Possible algorithms: \n\tRound-Robin (\"RR\")\n\tPriority Round-Robin (\"PRR\")\n\tShortest Remaining Time Next(\"SRTN\")\n");
                    abort();
                }
                break;
            case '?':
                if (optopt == 'k' || optopt == 'n')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort();
        }
    }

    if (nVal == -1 || timeStepVal == -1 || timeQVal == -1 || algoVal == -1) {
        fprintf(stderr, "usage: threadsched -n <N Threads> -t <time step> -q <time quantum> -a <algorithm>");
        abort();
    }

    if (nVal > 10) {
        fprintf(stderr, "n should not exceed 10");
        abort();
    }


    ready = list_init();
    waiting = list_init();
    for (int k = 0; k < 10; ++k) {
        pReadys[k] = list_init();
    }

    threadInfo_t *threadInfo;

    for (int j = 1; j <= nVal; ++j) {
        threadInfo = malloc(sizeof(threadInfo_t));

        int prio;
        int start;
        int runTime;
        scanf("%d %d %d", &prio, &start, &runTime);
        //printf("%d %d %d\n", prio, start, runTime);

        if (prio < 1 || prio > 10) {
            fprintf(stderr, "Priority should be between 1 and 10");
            abort();
        }
        if (start > 100000) {
            fprintf(stderr, "Start time should not exceed 100000(100s)");
            abort();
        }
        if (runTime > 30000) {
            fprintf(stderr, "Run time should not exceed 30000(30s)");
            abort();
        }

        threadInfo->tNum = j;
        threadInfo->tPrio = prio;
        threadInfo->tStart = start;
        threadInfo->tRunTime = 0;
        threadInfo->tFullTime = runTime;


        list_append(waiting, threadInfo);


    }


    threadInfo_t *cTInfo;
    int thread_num = 0;

    int time = 0;

    while (threadLeft(algoVal) || (thread_num && (cTInfo->tRunTime < cTInfo->tFullTime))) {

        readyThreads(time, algoVal);

        if (thread_num && cTInfo->tRunTime < cTInfo->tFullTime) {
            if (algoVal == PRIORITY_ROUND_ROBIN) {
                list_append(pReadys[cTInfo->tPrio], cTInfo);
            } else {
                list_append(ready, cTInfo);
            }
        }

        if (algoVal == SHORTEST_REMAINING_TIME_NEXT) {
            sortReadyQueueSRTN(ready);
        }

        //printf("Ready: ");

        cTInfo = pollNextThread(algoVal);

        thread_num = 0;
        if (cTInfo != NULL) {
            thread_num = cTInfo->tNum;
        }


        for (int i = 0; i < timeQVal; i += timeStepVal) {
            print_time_step(time, thread_num);
            time += timeStepVal;
            if (thread_num) {
                cTInfo->tRunTime += timeStepVal;
                /*if (cTInfo->tRunTime >= cTInfo->tFullTime) {
                    break;
                }*/
            } else {
                break;
            }
        }


    }

    list_finit(ready);
    list_finit(waiting);
}