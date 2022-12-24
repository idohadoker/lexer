#include "timer.h"

/*
============================================================================================
General : createJsonTime - get info about time of system
Parameters : jobTime - struct json_object  , timeinfo - struct tm 
Return Value : 0
============================================================================================
*/
short createJsonTime(json_object *jobTime, struct tm *timeinfo)
{
    json_object *tm_info = json_object_new_string(asctime(timeinfo));
    json_object *tm_sec = json_object_new_int(timeinfo->tm_sec);
    json_object *tm_min = json_object_new_int(timeinfo->tm_min);
    json_object *tm_hour = json_object_new_int(timeinfo->tm_hour);
    json_object *tm_mday = json_object_new_int(timeinfo->tm_mday);
    json_object *tm_mon = json_object_new_int(timeinfo->tm_mon + 1);
    json_object *tm_year = json_object_new_int(timeinfo->tm_year + 1900);
    json_object *tm_wday = json_object_new_int(timeinfo->tm_wday);
    json_object *tm_yday = json_object_new_int(timeinfo->tm_yday);
    json_object *tm_isdst = json_object_new_int(timeinfo->tm_isdst);

    json_object_object_add(jobTime, "tm_info", tm_info);
    json_object_object_add(jobTime, "tm_sec", tm_sec);     /* seconds,  range 0 to 59         */
    json_object_object_add(jobTime, "tm_min", tm_min);     /* minutes, range 0 to 59           */
    json_object_object_add(jobTime, "tm_hour", tm_hour);   /* hours, range 0 to 23             */
    json_object_object_add(jobTime, "tm_mday", tm_mday);   /* day of the month, range 1 to 31  */
    json_object_object_add(jobTime, "tm_mon", tm_mon);     /* month, range 0 to 11             */
    json_object_object_add(jobTime, "tm_year", tm_year);   /* The number of years since 1900   */
    json_object_object_add(jobTime, "tm_wday", tm_wday);   /* day of the week, range 0 to 6    */
    json_object_object_add(jobTime, "tm_yday", tm_yday);   /* day in the year, range 0 to 365  */
    json_object_object_add(jobTime, "tm_isdst", tm_isdst); /* daylight saving time             */
    return 0;
}

/*
============================================================================================
General : managerTimeGet -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerTimeGet(QUERY *queryList, unsigned short len)
{
    time_t rawtime;
    struct tm *timeinfo;
    short status;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    json_object *jobTime = json_object_new_object();
    status = createJsonTime(jobTime, timeinfo);
    printf("%s", json_object_to_json_string(jobTime));
    return status;
}

/*
============================================================================================
General : managerTimePost -POST manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerTimePost(QUERY *queryList, unsigned short len)
{
    time_t rawtime;
    // struct timezone tzp;

    struct timeval tp;
    short timeIndex = isNameQueryExist(queryList, len, "sec");
    // short timeIndex = isNameQueryExist(queryList, len, "link");

    // printf("%s", queryList[timeIndex].info);

    // printf(" %ld\n", time(NULL));
    rawtime = strtol(queryList[timeIndex].info, NULL, 10);
    tp.tv_sec = rawtime;
    tp.tv_usec = 0;
    if (!settimeofday(&tp, NULL))
    {
        messageJson("The update time is success ", 10);
    }
    else
    {
        messageJson("The update time is failed", 0);
    }

    return 0;
}

void GetAllTimeZones()
{
}
