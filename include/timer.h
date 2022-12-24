#ifndef TIMER_H // TIMER_H
#define TIMER_H

#include <sys/time.h>
#include "commonFun.h"
short createJsonTime(json_object *jobTime, struct tm *timeinfo);
short managerTimeGet(QUERY *queryList, unsigned short len);
short managerTimePost(QUERY *queryList, unsigned short len);

#endif