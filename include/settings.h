#ifndef SETTINGS_H // SETTINGS_H
#define SETTINGS_H

#include "commonFun.h"
#include <limits.h>
#include <sys/param.h>
#include <sys/types.h>
#define EOD '\0'
#define BUF_SIZE (MAXHOSTNAMELEN + 1)
short managerSettingGet(QUERY *list, unsigned short len);
short managerSettingPost(QUERY *list, unsigned short len);
short getHostName(QUERY *list, unsigned short len, json_object *jObjSettings);
void setHostName(QUERY *list, unsigned short len);
#endif