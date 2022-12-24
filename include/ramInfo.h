#ifndef RAM_INFO_H
#define RAM_INFO_H

#include "commonFun.h"
#include <sys/sysinfo.h>
#include <linux/limits.h>
#include <sys/times.h>

// typedef struct memInfo
// {
//     char *MemTotal; // Total amount of physical RAM, in kilobytes
//     char *MemFree;  // The amount of physical RAM, in kilobytes, left unused by the system.
//     char *MemAvailble;
//     char *Buffers;     // The amount of physical RAM, in kilobytes, used for file buffers.
//     char *Cached;      // The amount of physical RAM, in kilobytes, used as cache memory.
//     char *SwapCached;  // The amount of swap, in kilobytes, used as cache memory.
//     char *Active;      /* The total amount of buffer or page cache memory, in kilobytes,
//                           that is in active use. This is memory that has been recently used and is
//                           usually not reclaimed for other purposes*/
//     char *Inactive;    /* The total amount of buffer or page cache memory,
//                           in kilobytes, that are free and available.
//                           This is memory that has not been recently used and can be reclaimed for other purposes.*/
//     char *Active_Anon; //
//     char *Inactive_Anon;
//     char *Active_File;
//     char *Inactive_File;
//     char *Unevictable;
//     char *Mlocked;
//     char *Swaptotal; // The total amount of swap available, in kilobytes.
//     char *SwapFree;  // The total amount of swap free, in kilobytes.
//     char *Dirty;     // The total amount of memory, in kilobytes, waiting to be written back to the disk.
//     char *Writeback; // The total amount of memory, in kilobytes, actively being written back to the disk.
//     char *AnonPages; //
//     char *Mapped;    /* The total amount of memory,
//                         in kilobytes, which have been used to map devices, files,
//                          or libraries using the mmap command.*/
//     char *Shmem;
//     char *Slab; //  The total amount of memory, in kilobytes, used by the kernel to cache data structures for its own use.
//     char *SReclaimable;
//     char *SUnreclaim;
//     char *KernelStack;
//     char *PageTables; //The total amount of memory, in kilobytes, dedicated to the lowest page table level.
//     char *NFS_Unstable;
//     char *Bounce;
//     char *WritebackTmp;
//     char *CommitLimit;
//     char *Committed_AS;
//     char *VmallocTotal; // The total amount of memory, in kilobytes, of total allocated virtual address space.
//     char *VmallocUsed;  // The total amount of memory, in kilobytes, of used virtual address space.
//     char *VmallocChunk; // The largest contiguous block of memory, in kilobytes, of available virtual address space.
// } MEMINFO;

void memInfo(struct sysinfo *info);
short managerRamInfo(QUERY *queryList, unsigned short len);
short readFileMemInfo();

#endif