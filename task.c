#include "task.h"
Task createtask(){

    if (strncmp(new_task, "exit", 5) == 0)
    {
        strncpy(task.buffer, "-1", 5);

        printf("leaving program\n");
    }

}

int deletetasknumber(){

    int task;
    printf("\n Enter task to delete (press 0 to leave)\n");
    scanf("%d", &task);

    return task;
}