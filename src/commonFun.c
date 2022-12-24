#include "commonFun.h"
/*
============================================================================================
General : copy_file  
Parameters : fp - open file 
Return Value : The text of file.
============================================================================================
*/
char *copy_file(FILE *fp)
{
    char *responseData = malloc(sizeof(char));
    char c;
    int index = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        responseData = (char *)realloc(responseData, sizeof(char) * (index + 1));
        responseData[index] = c;
        index++;
    }
    responseData[index - 1] = EOD;
    return responseData;
}

/*
============================================================================================
General : IsNumber  
Parameters : str - string. 
Return Value : Checking if string is number and return true or false.
============================================================================================
*/
unsigned short IsNumber(char *str)
{
    while (*str != '\0')
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

/*
============================================================================================
General : readLine - read raw from file and put this in string .
Parameters : Fd - open file , Buffer - string 
Return Value : return 0 is true else -1 is false.
============================================================================================
*/
int readLine(int Fd, char *Buffer)
{
    if (Fd < 0)
    {
        return -1;
    }

    char ch;
    size_t Read = 0;

    while (read(Fd, (Buffer + Read), 1))
    {
        ch = Buffer[Read];
        if (ch == '\n')
        {
            break;
        }
        Read++;
    }

    if (Read)
    {
        Buffer[Read] = 0;
        return 0;
    }

    return -1;
}

/*
=========================================================================================================
General : queryList - A function which receives a query from the customer and turns it into an object .
Parameters : queryString - string, count - unsigned short pointer
Return Value : return struct QUERY
=========================================================================================================
*/
QUERY *queryList(char *queryString, unsigned short *count)
{
    QUERY *list = malloc(sizeof(QUERY));
    *count = 0;
    char *startQuery = queryString;
    char *endQuery = queryString;
    unsigned short flagFinish = 0;

    while (*startQuery)
    {
        strsep(&endQuery, "=");
        list[*count].name = malloc(sizeof(char) * strlen(startQuery));
        strncpy(list[*count].name, startQuery, strlen(startQuery));
        if (!strchr(endQuery, '&'))
        {
            list[*count].info = malloc(sizeof(char) * strlen(endQuery));
            strncpy(list[*count].info, endQuery, strlen(endQuery));
            flagFinish = 1;
        }
        startQuery = endQuery;

        if (strchr(endQuery, '&') && !flagFinish)
        {
            strsep(&endQuery, "&");
            list[*count].info = malloc(sizeof(char) * strlen(startQuery));
            strncpy(list[*count].info, startQuery, strlen(startQuery));
            startQuery = endQuery;
        }
        else
        {
            *startQuery = '\0';
        }
        (*count)++;
        list = realloc(list, sizeof(QUERY) * (*count + 1));
    }
    return list;
}

short isNameQueryExist(QUERY *queryList, unsigned short len, char *name)
{
    unsigned short index = 0, indexQuery = -1;
    unsigned short flagExist = 0;
    for (index = 0; ((index < len) && (!flagExist)); index++)
    {
        if (!strcmp(queryList[index].name, name))
        {
            indexQuery = index;
            flagExist = 1;
        }
    }
    return indexQuery;
}

/*
=========================================================================================================
General : managerRequest - manager routes.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
=========================================================================================================
*/
short managerRequest(QUERY *queryList, unsigned short len)
{
    short locFile = -1;
    unsigned short indexQuery = 0;

    indexQuery = isNameQueryExist(queryList, len, "file");

    if (queryList[indexQuery].info)
    {
        if (!strcmp(queryList[indexQuery].info, "arpTable"))
        {
            locFile = 1;
        }

        if (!strcmp(queryList[indexQuery].info, "proc"))
        {
            locFile = 2;
        }

        if (!strcmp(queryList[indexQuery].info, "route"))
        {
            locFile = 3;
        }

        if (!strcmp(queryList[indexQuery].info, "ping"))
        {
            locFile = 4;
        }

        if (!strcmp(queryList[indexQuery].info, "ram"))
        {
            locFile = 5;
        }

        if (!strcmp(queryList[indexQuery].info, "time"))
        {
            locFile = 6;
        }

        if (!strcmp(queryList[indexQuery].info, "inter"))
        {
            locFile = 7;
        }

        if (!strcmp(queryList[indexQuery].info, "service"))
        {
            locFile = 8;
        }
        if (!strcmp(queryList[indexQuery].info, "storage"))
        {
            locFile = 9;
        }
        if (!strcmp(queryList[indexQuery].info, "settings"))
        {
            locFile = 10;
        }
    }
    return locFile;
}

/*
=========================================================================================================
General : messageJson - creat json message.
Parameters : message - string  , flagError - unsigned short
Return Value : null
=========================================================================================================
*/

void messageJson(char *message, unsigned short flagError)
{
    json_object *jobstatus = json_object_new_object();
    json_object *jStatus = json_object_new_boolean(flagError);
    json_object *jmessage = json_object_new_string(message);
    json_object_object_add(jobstatus, "status", jStatus);
    json_object_object_add(jobstatus, "message", jmessage);
    printf("%s", json_object_to_json_string(jobstatus));
    exit(1);
}

/*
===========================================================================
General : GetParamFromFile - get data from specific file.
Parameters : path - string  , buffer - string 
Return Value : 0
===========================================================================
*/
int GetParamFromFile(char *path, char *buffer)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        printf("error\n");
        return -1;
    }
    readLine(fd, buffer);
    return 0;
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}