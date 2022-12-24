#include "arpTable.h"

/*
============================================================================================
General : GetTitleTable - put the data from table in array.
Parameters : count - num of columns , Buffer - string 
Return Value : Return array of strings
============================================================================================
*/
char **GetTitleTableArpTable(unsigned short *count, char *buffer)
{
    *count = 0;
    char *start = buffer;
    char *tmp = buffer;
    char **ArrTitle = (char **)malloc(sizeof(char *));
    unsigned short countSpace = 0, len = 0, countSpaceTemp = 0;
    unsigned short flagNewField = 0;
    // Loop on raw
    while (*tmp)
    {

        if (((countSpace >= 2) && (*tmp != ' ')) || *(tmp + 1) == '\0')
        {
            flagNewField = 1;
            countSpaceTemp = countSpace;
        }

        if (*tmp == ' ' || *tmp == '\t')
        {
            countSpace++;
        }
        else
        {
            countSpace = 0;
        }

        if (flagNewField)
        {
            len = strlen(start) - countSpaceTemp - strlen(tmp);
            (*count)++;
            ArrTitle[*count - 1] = malloc(sizeof(char) * len);
            strncpy(ArrTitle[*count - 1], start, len);
            start = tmp;
            ArrTitle = (char **)realloc(ArrTitle, sizeof(char *) * ((*count) + 1));
            flagNewField = 0;
        }
        tmp++;
    }
    return ArrTitle;
}

/*
============================================================================================
General : Create_Arp_Table_Json - create json for arp table.
Parameters : fd - open file
Return Value : Return zero
============================================================================================
*/
int Create_Arp_Table_Json(int fd)
{
    char buffer[BUFSIZ];
    readLine(fd, buffer);
    char **arrValues;
    unsigned short count = 0;
    arrValues = GetTitleTableArpTable(&count, buffer);

    json_object *jobArpTables = json_object_new_array();
    json_object *IP_Address;
    json_object *HW_Type;
    json_object *Flags;
    json_object *HW_Address;
    json_object *Mask;
    json_object *Device;

    while (!readLine(fd, buffer))
    {
        json_object *jobArp = json_object_new_object();
        arrValues = GetTitleTableArpTable(&count, buffer);

        IP_Address = json_object_new_string(arrValues[0]);
        HW_Type = json_object_new_string(arrValues[1]);
        Flags = json_object_new_string(arrValues[2]);
        HW_Address = json_object_new_string(arrValues[3]);
        Mask = json_object_new_string(arrValues[4]);
        Device = json_object_new_string(arrValues[5]);

        json_object_object_add(jobArp, "IP_Address", IP_Address);
        json_object_object_add(jobArp, "HW_Type", HW_Type);
        json_object_object_add(jobArp, "Flags", Flags);
        json_object_object_add(jobArp, "HW_Address", HW_Address);
        json_object_object_add(jobArp, "Mask", Mask);
        json_object_object_add(jobArp, "Device", Device);

        json_object_array_add(jobArpTables, jobArp);
        free(arrValues);
    }
    printf("%s", json_object_to_json_string(jobArpTables));

    return 0;
}

/*
============================================================================================
General : managerArpTable - manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerArpTable(QUERY *list, unsigned short len)
{
    short status = 0;
    int fd = open(ARP_CACHE, O_RDONLY);
    if (fd < 0)
    {
        printf("error\n");
        exit(1);
    }
    status = Create_Arp_Table_Json(fd);
    return status;
}