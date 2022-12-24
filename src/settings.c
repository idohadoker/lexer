#include "settings.h"

short getHostName(QUERY *list, unsigned short len, json_object *jObjSettings)
{
    char buf[BUF_SIZE];
    json_object *jHostName;
    if (gethostname(buf, BUF_SIZE) == -1)
    {
        messageJson("The GET was failed ", 0);
    }

    jHostName = json_object_new_string(buf);
    json_object_object_add(jObjSettings, "hostname", jHostName);

    return EXIT_SUCCESS;
}

void setHostName(QUERY *list, unsigned short len)
{
    int fd = open("/etc/hostname", O_WRONLY);
    short hostname = isNameQueryExist(list, len, "hostname");
    char *newhostname = list[hostname].info;
    if (hostname != -1)
    {
        if (fd < 0)
        {
            messageJson("The update was failed,write file ", 0);
        }
        write(fd, newhostname, sizeof(char) * strlen(newhostname));
        close(fd);
        if (sethostname(list[hostname].info, strlen(list[hostname].info)) == -1)
        {
            messageJson("The update was failed ", 0);
        }
    }

    messageJson("The update was successful ", 10);
}

short managerSettingGet(QUERY *list, unsigned short len)
{
    short status = 0;
    json_object *jObjSettings = json_object_new_object();
    status = getHostName(list, len, jObjSettings);
    printf("%s\n", json_object_to_json_string(jObjSettings));
    return status;
}

short managerSettingPost(QUERY *list, unsigned short len)
{
    setHostName(list, len);
    return 0;
}