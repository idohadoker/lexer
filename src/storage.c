#include "storage.h"

char *fieldNameJson[] = {"d_ino", "d_name", "st_dev", "st_mode",
                         "st_nlink", "st_uid", "st_gid",
                         "st_rdev ", "st_size", " st_blksize", "st_blocks",
                         "st_atime", "st_mtime", "st_ctime"};

void listdir(const char *name)
{
    DIR *directory;
    struct dirent *d_file; // a file in *directory
    directory = opendir(name);
    json_object *jobStorageFile = json_object_new_array();
    json_object *jField[15];
    uint64_t sum = 0;
    while ((d_file = readdir(directory)) != 0)
    {
        struct stat filestat;
        char *abPath = malloc(1024);
        memset(abPath, 0, 1024);
        strcpy(abPath, name);
        strcat(abPath, "/");
        strcat(abPath, d_file->d_name);
        stat(abPath, &filestat);
        json_object *jobStorge = json_object_new_object();

        if (strcmp(".", d_file->d_name) && strcmp("..", d_file->d_name))
        {
            jField[0] = json_object_new_int(d_file->d_ino);
            jField[1] = json_object_new_string(d_file->d_name);
            jField[2] = json_object_new_int(filestat.st_dev);
            jField[3] = json_object_new_string(getModeFile(filestat.st_mode));
            jField[4] = json_object_new_int(filestat.st_nlink);
            jField[5] = json_object_new_int(filestat.st_uid);
            jField[6] = json_object_new_int(filestat.st_gid);
            jField[7] = json_object_new_int(filestat.st_rdev);
            jField[8] = json_object_new_int(filestat.st_size);
            jField[9] = json_object_new_int(filestat.st_blksize);
            jField[10] = json_object_new_int(filestat.st_blocks);
            jField[11] = json_object_new_int(filestat.st_atime);
            jField[12] = json_object_new_int(filestat.st_mtime);
            jField[13] = json_object_new_int(filestat.st_ctime);
            sum += filestat.st_size / 1024;
            // printf("%lu\n", filestat.st_size);
            for (int i = 0; i < 14; i++)
            {
                json_object_object_add(jobStorge, fieldNameJson[i], jField[i]);
            }
            json_object_array_add(jobStorageFile, jobStorge);
        }
        free(abPath);
    }
    // printf("%llu\n", sum);

    printf("%s\n", json_object_to_json_string(jobStorageFile));
    closedir(directory);
}

char *getModeFile(unsigned short mode)
{
    char *modestr;
    switch (mode & S_IFMT)
    {
    case S_IFDIR:
    {
        modestr = "S_IFDIR";
        break;
    }
    case S_IFREG:
    {
        modestr = "S_IFREG";
        break;
    }
    case S_IFSOCK:
    {
        modestr = "S_IFSOCK";
        break;
    }
    case S_IFLNK:
    {
        modestr = "S_IFLNK";
        break;
    }
    case S_IFBLK:
    {
        modestr = "S_IFBLK";
        break;
    }
    case S_IFCHR:
    {
        modestr = "S_IFCHR";
        break;
    }
    case S_IFIFO:
    {
        modestr = "S_IFIFO";
        break;
    }
    }
    return modestr;
}

unsigned long byteSizePath(const char *name)
{
    DIR *dir;
    unsigned long sum = 0;
    struct dirent *entry;
    struct stat *buffer = malloc(sizeof(struct stat));
    if (!(dir = opendir(name)))
        return 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            stat(entry->d_name, buffer);
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            sum += byteSizePath(path);
        }
        else
        {
            stat(entry->d_name, buffer);
        }
        sum += buffer->st_size;
    }
    closedir(dir);
    return sum;
}

void listdirRec(const char *name, int indent, unsigned long sum)
{
    DIR *dir;
    struct dirent *entry;
    struct stat *buffer = malloc(sizeof(struct stat));
    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            stat(entry->d_name, buffer);
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            // printf("%*s[%s] %lu kb\n", indent, "", entry->d_name, buffer->st_size);
            listdirRec(path, indent + 2, buffer->st_size + sum);
        }
        else
        {
            stat(entry->d_name, buffer);

            // printf("%*s- %s %lu kb\n", indent, "", entry->d_name, buffer->st_size);
        }
        sum += buffer->st_size;
    }
    // printf("%ld\n", sum / 1024);
    closedir(dir);
}

void fileSystemSizeMemory(char *name)
{
    struct statvfs buffer;
    int ret = statvfs(name, &buffer);
    json_object *jobStorge = json_object_new_object();
    if (!ret)
    {
        const double total = (double)(buffer.f_blocks * buffer.f_frsize) / 1024 / 1024;
        const double available = (double)(buffer.f_bavail * buffer.f_frsize) / 1024 / 1024;
        const double used = total - available;
        const double usedPercentage = (double)(used / total) * (double)100;
        json_object *totalJson = json_object_new_double(total);
        json_object *availableJson = json_object_new_double(available);
        json_object *usedJson = json_object_new_double(used);
        json_object *usedPercentageJson = json_object_new_double(usedPercentage);
        json_object_object_add(jobStorge, "total", totalJson);
        json_object_object_add(jobStorge, "available", availableJson);
        json_object_object_add(jobStorge, "used", usedJson);
        json_object_object_add(jobStorge, "usedPercentage", usedPercentageJson);
        printf("%s\n", json_object_to_json_string(jobStorge));
    }
    else
    {
        messageJson("the request is failed", 0);
    }
}

/*
============================================================================================
General : managerStorageGet -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerStorageGet(QUERY *queryList, unsigned short len)
{
    short nameIndex = isNameQueryExist(queryList, len, "namefile");
    char *nameFile = queryList[nameIndex].info;
    short storgeIndex = isNameQueryExist(queryList, len, "statfs");
    if (!strcmp("df", queryList[storgeIndex].info))
    {
        fileSystemSizeMemory(nameFile);
    }

    if (!strcmp("du", queryList[storgeIndex].info))
    {
        listdir(nameFile);
    }
    return 0;
}
