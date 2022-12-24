#include "processes.h"

char *field_Status_File[FIELD_STATUS_FILE] =
    {"Name", "State", "Tgid", "Ngid", "Pid", "PPid", "TracerPid",
     "Uid", "Gid", "FDSize", "Group", "NStgid", "NSpid", "NSpgid",
     "NSsid", "VmPeak", "VmSize", "VmLck", "VmPin", "VmHWM", "VmRSS",
     "VmData", "VmStk", "VmExe", "VmLib", "VmPTE", "VmPMD", "VmSwap",
     "Threads", "SigQ", "SigPnd", "ShdPnd", "SigBlk", "SigIgn",
     "SigCgt", "CapInh", "CapPrm", "CapEff", "CapBnd", "Cpus_allowed",
     "Cpus_allowed_list", "Mems_allowed", "Mems_allowed_list", "voluntary_ctxt_switches",
     "nonvoluntary_ctxt_switches"};

/*
============================================================================================
General : read_dir_proc - read only directory that they are processes.
Parameters : dir - open directory , statusfile - struct json_object
Return Value : index Process.
============================================================================================
*/
int read_dir_proc(DIR *dir, json_object *objProcs)
{
    struct dirent *ent;
    long pid;
    int indexProcess = 0;

    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL)
    {
        if (IsNumber(ent->d_name))
        {
            pid = strtol(ent->d_name, NULL, 10);
            statusPidFile(pid, objProcs, indexProcess);
            indexProcess++;
        }
    }
    return indexProcess;
}

void statusPidFile(long pid, json_object *objProcs, int indexProcess)
{
    char path[30], line[100];
    FILE *statusf;
    char *field, *dataField, *str;
    int indexFinish = 0;
    json_object *jobProc = json_object_new_object();
    json_object *jField[FIELD_STATUS_FILE];

    snprintf(path, 30, "/proc/%ld/status", pid);
    statusf = fopen(path, "r");
    if (!statusf)
    {
        printf("Open file is failed");
        exit(0);
    }

    while ((str = fgets(line, 100, statusf)) != '\0')
    {
        field = compareField(str, field_Status_File, FIELD_STATUS_FILE, &indexFinish);
        strtok(field, ": ");
        dataField = strtok(NULL, "\n");
        dataField++;

        jField[indexFinish] = json_object_new_string(dataField);

        json_object_object_add(jobProc, field_Status_File[indexFinish], jField[indexFinish]);
    }

    json_object_array_add(objProcs, jobProc);
    fclose(statusf);
}

/*
==================================================================================================
General : compareField  - The function check if the field in processes file is exist
Parameters : str - open file ,strsubArr- field Status File ,len - length of array,
             indexFinish - location of field
Return Value : The text of file.
==================================================================================================
*/
char *compareField(char *str, char **strsubArr, int len, int *indexFinish)
{
    unsigned short flag = 0;
    char *field;
    int i = 0;
    while (i < len)
    {
        if (strstr(str, strsubArr[i]) != 0)
        {
            field = str;
            flag = 1;
            (*indexFinish) = i;
        }
        i++;
    }
    return flag ? field : NULL;
}

/*
============================================================================================
General : managerProcesses -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerProcesses(QUERY *list, unsigned short len)
{
    json_object *objProcs = json_object_new_array();
    DIR *dir;
    short sigIndex = isNameQueryExist(list, len, "sig");
    short pidIndex = isNameQueryExist(list, len, "pid");
    unsigned short flag = T;
    long pid;
    char *querySig;

    if ((sigIndex != -1) && (pidIndex != -1))
    {
        printf("%s\n", list[0].info);
        pid = strtol(list[pidIndex].info, NULL, 10);
        querySig = list[sigIndex].info;

        if (!strcmp("SIGKILL", querySig) && kill(pid, SIGKILL) == -1)
        {
            flag = F;
            messageJson("The killed is failed", 0);
        }

        if (!strcmp("SIGTERM", querySig) && kill(pid, SIGTERM) == -1)
        {
            flag = F;
            messageJson("The terminate is failed", 0);
        }

        if (!strcmp("SIGCONT", querySig) && kill(pid, SIGCONT) == -1)
        {
            flag = F;
            messageJson("The continue is failed", 0);
        }

        if (!strcmp("SIGSTOP", querySig) && kill(pid, SIGSTOP) == -1)
        {
            flag = F;
            messageJson("The stop is failed", 0);
        }
        if (flag)
        {
            messageJson("The signal is success", 10);
        }
        return 0;
    }

    if ((dir = opendir("/proc")) != NULL)
    {
        /* print all the files and directories within directory */
        read_dir_proc(dir, objProcs);
        printf("\n%s", json_object_to_json_string(objProcs));
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror(" could not open directory");
        return EXIT_FAILURE;
    }
    return 0;
}
