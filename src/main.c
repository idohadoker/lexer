#include "arpTable.h"
#include "route.h"
#include "processes.h"
#include "ping.h"
#include "ramInfo.h"
#include "timer.h"
#include "interfaces.h"
#include "services.h"
#include "storage.h"
#include "settings.h"
int main()
{

    char *query_string = getenv("QUERY_STRING");
    char *method = getenv("REQUEST_METHOD");
    // char *lenstr = getenv("CONTENT_LENGTH");
    QUERY *list = NULL;
    unsigned short len = 0;
    short locfile = -1;

    list = queryList(query_string, &len);
    int status;

    if (!strcmp(method, "GET"))
    {
        locfile = managerRequest(list, len);
        switch (locfile)
        {
        case 1:
            status = managerArpTable(list, len);
            break;
        case 2:
            status = managerProcesses(list, len);
            break;
        case 3:
            status = managerRoute(list, len);
            break;
        case 4:
            status = managerPing(list, len);
            break;
        case 5:
            status = managerRamInfo(list, len);
            break;
        case 6:
            status = managerTimeGet(list, len);
            break;
        case 7:
            status = managerInterfacesGet(list, len);
            break;
        case 8:
            status = managerServicesGet(list, len);
            break;
        case 9:
            status = managerStorageGet(list, len);
            break;
        case 10:
            status = managerSettingGet(list, len);
            break;
        default:
            free(list);
            messageJson("No file", 0);
            break;
        }
    }

    if (!strcmp(method, "POST"))
    {
        locfile = managerRequest(list, len);
        switch (locfile)
        {
        case 6:
            status = managerTimePost(list, len);
            break;
        case 7:
            status = managerInterfacesPost(list, len);
            break;
        case 10:
            status = managerSettingPost(list, len);
            break;
        default:
            free(list);
            messageJson("No file", 0);
            break;
        }
    }

    free(list);
    return status;
}
