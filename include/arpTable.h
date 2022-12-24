#ifndef ARP_TABLE_H // ARP_TABLE_H
#define ARP_TABLE_H

#include "commonFun.h"
#include <sys/stat.h>

#define ARP_CACHE "/proc/net/arp"
#define ARP_BUFFER_LEN 1024
#define ARP_DELIM " "
int Create_Arp_Table_Json(int fd);
char **GetTitleTableArpTable(unsigned short *count, char *buffer);
short managerArpTable(QUERY *list, unsigned short len);

#endif