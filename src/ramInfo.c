#include "ramInfo.h"
void memInfo(struct sysinfo *info)
{
    if (sysinfo(info) < 0)
        messageJson("No info", 0);
}

// void cpuUsage()
// {

// }

short readFileMemInfo()
{
    json_object *jField[34];
    char line[100];
    FILE *statusf;
    char *field, *dataField, *str;
    int index = 0;
    json_object *objram = json_object_new_object();
    statusf = fopen("/proc/meminfo", "r");
    long data;
    if (!statusf)
    {
        printf("Open file is failed");
        exit(0);
    }

    while ((str = fgets(line, 100, statusf)) != '\0')
    {
        field = strtok(str, ": ");
        dataField = strtok(NULL, "kB");
        while ((*dataField) == 32)
        {
            dataField++;
        }
        data = strtol(dataField, NULL, 10);

        jField[index] = json_object_new_int(data);
        json_object_object_add(objram, field, jField[index]);
        index++;
    }
    printf("%s", json_object_to_json_string(objram));
    fclose(statusf);
    return 0;
}

/*
============================================================================================
General : managerRamInfo -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerRamInfo(QUERY *queryList, unsigned short len)
{
    readFileMemInfo();
    // struct sysinfo info;
    // memInfo(&info);
    // json_object *objram = json_object_new_object();
    // json_object *totalram = json_object_new_int(info.totalram / 1024);
    // json_object *freeram = json_object_new_int(info.freeram / 1024);
    // json_object *sharedram = json_object_new_int(info.sharedram / 1024);
    // json_object *bufferram = json_object_new_int(info.bufferram / 1024);
    // json_object *totalswap = json_object_new_int(info.totalswap / 1024);
    // json_object *freeswap = json_object_new_int(info.freeswap / 1024);
    // json_object *procs = json_object_new_int(info.procs);
    // json_object *totalhigh = json_object_new_int(info.totalhigh / 1024);
    // json_object *freehigh = json_object_new_int(info.freehigh / 1024);
    // json_object *mem_unit = json_object_new_int(info.mem_unit / 1024);

    // json_object_object_add(objram, "totalram", totalram);   /* Total usable main memory size */
    // json_object_object_add(objram, "freeram", freeram);     /* Available memory size */
    // json_object_object_add(objram, "sharedram", sharedram); /* Amount of shared memory */
    // json_object_object_add(objram, "bufferram", bufferram); /* Memory used by buffers */
    // json_object_object_add(objram, "totalswap", totalswap); /* Total swap space size */
    // json_object_object_add(objram, "freeswap", freeswap);   /* swap space still available */
    // json_object_object_add(objram, "procs", procs);         /* Number of current processes */
    // json_object_object_add(objram, "totalhigh", totalhigh); /* Total high memory size */
    // json_object_object_add(objram, "freehigh", freehigh);   /* Available high memory size */
    // json_object_object_add(objram, "mem_unit", mem_unit);   /* Memory unit size in bytes */

    // printf("%s", json_object_to_json_string(objram));

    return 0;
}