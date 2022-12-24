#ifndef PROCESSES_H
#define PROCESSES_H

#include "commonFun.h"
#include <linux/limits.h>
#include <sys/times.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>

#define FIELD_STATUS_FILE 45
#define T 1
#define F 0

int read_dir_proc(DIR *dir, json_object *objProcs);
void statusPidFile(long pid, json_object *objProcs, int indexProcess);
char *compareField(char *str, char **strsubArr, int len, int *indexFinish);
short managerProcesses(QUERY *list, unsigned short len);

#endif