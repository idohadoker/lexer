#include "services.h"

/*
============================================================================================
General : managerServicesGet -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerServicesGet(QUERY *list, unsigned short len)
{
    struct servent *service;
    short IndexServe = isNameQueryExist(list, len, "serv_name");
    short IndexPort = isNameQueryExist(list, len, "port");
    short IndexPorto = isNameQueryExist(list, len, "porto");
    int data = strtol(list[IndexPort].info, NULL, 10);

    printf("%s %d \n", list[IndexServe].info, data);

    service = getservbyname(list[IndexServe].info, list[IndexPorto].info);

    printf("services\n");

    // while ((service = getservent()) != NULL)

    // {

    printf("%s %d %s\n", service->s_name, service->s_port, service->s_proto);
    // }
    return 0;
}