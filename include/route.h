#ifndef ROUTE_H // ROUTE_H
#define ROUTE_H

#include "commonFun.h"
#include <sys/socket.h>
#include <net/route.h>
#include <sys/ioctl.h>

#define ROUTE_CACHE "/proc/net/route"
#define ROUTE_BUFFER_LEN 1024

int Create_Route_Table_Json(int fd);
char **GetTitleTableRoute(unsigned short *count, char *buffer);
short managerRoute(QUERY *list, unsigned short len);
// unsigned short delNullRoute(long host);
// unsigned short addNullRoute(long host);

#endif