#ifndef INTERFACES_H // INTERFACES_H
#define INTERFACES_H

#include "commonFun.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <dirent.h>
#include <linux/netlink.h>
#include <bits/sockaddr.h>
#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>

#include <linux/sockios.h>
#include <net/route.h>
#include <errno.h>

#ifndef _BOOL
#define BOOL
#define T 1
#define F 0
#define bool unsigned short
#endif

char **GetNameNetwork(unsigned short *countNet);
char *GetIPAddress(int fd, struct ifreq ifr);
char *GetNetmask(int fd, struct ifreq ifr);
char *GetBroadcast(int fd, struct ifreq ifr);
int GetMetric(int fd, struct ifreq ifr);
int GetMTU(int fd, struct ifreq ifr);
short managerInterfacesGet(QUERY *queryList, unsigned short len);
void configInterface(QUERY *list, unsigned short len, char *config);
short managerInterfacesPost(QUERY *list, unsigned short len);
#endif