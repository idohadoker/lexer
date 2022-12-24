#ifndef STORAGE_H // STORAGE_H
#define STORAGE_H

#include "commonFun.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <stdint.h>

#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>

short managerStorageGet(QUERY *queryList, unsigned short len);
void listdirRec(const char *name, int indent, unsigned long sum);
unsigned long byteSizePath(const char *name);
char *getModeFile(unsigned short mode);
#endif