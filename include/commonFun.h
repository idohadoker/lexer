#ifndef COMMON_H // COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define EOD '\0'

typedef struct Query
{
    char *name;
    char *info;
} QUERY;

int readLine(int Fd, char *Buffer);
char *copy_file(FILE *fp);
unsigned short IsNumber(char *str);
QUERY *queryList(char *queryString, unsigned short *count);
short managerRequest(QUERY *queryList, unsigned short len);
short isNameQueryExist(QUERY *queryList, unsigned short len, char *name);
void messageJson(char *message, unsigned short flagError);
int GetParamFromFile(char *path, char *buffer);
void delay(int number_of_seconds);
#endif