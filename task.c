#include "task.h"
Task createtask(){
    Task task;
    char new_task[100];
    int c;
    scanf(" %99[a-zA-Z0-9 ]", new_task);
    while ((c = fgetc(stdin)) != '\n' && c != EOF)
        ;
    if (strncmp(new_task, "exit", 5) == 0)
    {
        strncpy(task.buffer, "-1", 5);

        printf("leaving program\n");
    }
    else
    {
        strncpy(task.buffer, new_task, 100);
        task.state = 0;
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strncpy(task.time, asctime(timeinfo), 50);
    }
    return task;
}

int deletetasknumber(){

    int task;
    printf("\n Enter task to delete (press 0 to leave)\n");
    scanf("%d", &task);

    return task;
}