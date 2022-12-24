#include "interfaces.h"

/*
===========================================================================
General : GetNameNetwork - get all names interface network in system.
Parameters : countNet - unsigned short
Return Value : array of string
===========================================================================
*/
char **GetNameNetwork(unsigned short *countNet)
{
    struct ifaddrs *addrs, *tmp;
    char **ArrNet;
    *countNet = 0;
    getifaddrs(&addrs);
    tmp = addrs;
    ArrNet = (char **)malloc(sizeof(char *));
    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
        {
            (*countNet)++;
            ArrNet[*countNet - 1] = malloc(sizeof(char) * (IFNAMSIZ - 1));
            strncpy(ArrNet[*countNet - 1], tmp->ifa_name, IFNAMSIZ - 1);
        }
        ArrNet = (char **)realloc(ArrNet, sizeof(char *) * ((*countNet) + 1));
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
    return ArrNet;
}

/*
===========================================================================
General : GetIPAddress - get Ip to specific interface .
Parameters : fd - int , ifr -struct ifreq
Return Value : ip in string format
===========================================================================
*/
char *GetIPAddress(int fd, struct ifreq ifr)
{
    bool IsError = F;
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1)
        IsError = T;
    return !IsError ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : "Not Found";
}

/*
===========================================================================
General : GetIPAddress - get netmask to specific interface .
Parameters : fd - int , ifr -struct ifreq
Return Value : netmask in string format else false
===========================================================================
*/
char *GetNetmask(int fd, struct ifreq ifr)
{
    bool IsError = F;
    if (ioctl(fd, SIOCGIFNETMASK, &ifr) == -1)
        IsError = T;
    return !IsError ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : "Not Found";
}

/*
===========================================================================
General : GetBroadcast - get broadcast to specific interface .
Parameters : fd - int , ifr -struct ifreq
Return Value : broadcast in string format else false
===========================================================================
*/
char *GetBroadcast(int fd, struct ifreq ifr)
{
    bool IsError = F;
    if (ioctl(fd, SIOCGIFBRDADDR, &ifr) == -1)
        IsError = T;
    return !IsError ? inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) : "Not Found";
}

/*
===========================================================================
General : GetMetric - get metric to specific interface .
Parameters : fd - int , ifr -struct ifreq
Return Value : metric else false
===========================================================================
*/
int GetMetric(int fd, struct ifreq ifr)
{
    bool IsError = F;
    if (ioctl(fd, SIOCGIFMETRIC, &ifr) == -1)
        IsError = T;
    return !IsError ? ifr.ifr_metric : IsError;
}

/*
===========================================================================
General : GetMTU - get mtu to specific interface .
Parameters : fd - int , ifr -struct ifreq
Return Value : mtu else false
===========================================================================
*/
int GetMTU(int fd, struct ifreq ifr)
{
    bool IsError = F;
    if (ioctl(fd, SIOCGIFMTU, &ifr) == -1)
        IsError = T;
    return !IsError ? ifr.ifr_mtu : IsError;
}

/*
============================================================================================
General : managerInterfacesGet -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerInterfacesGet(QUERY *list, unsigned short len)
{

    struct ifreq ifr;
    unsigned short count;
    DIR *dir;
    long data;
    int index = 0;
    struct dirent *ent;
    char pathStatistics[70], buffer[50], pathFile[80], path[60];
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    char **arr = GetNameNetwork(&count);
    ifr.ifr_addr.sa_family = AF_INET;
    json_object *jobnets = json_object_new_array();
    json_object *jobnet;
    json_object *jNetName;
    json_object *jIPAddress;
    json_object *jNetmask;
    json_object *jBroadcast;
    json_object *jMacAddress;
    json_object *jMetric;
    json_object *jMTU;
    json_object *jOperstate;
    json_object *jtx_queue_len;
    json_object *jField[23];
    for (int i = 0; i < count; i++)
    {
        strncpy(ifr.ifr_name, arr[i], IFNAMSIZ - 1);
        jobnet = json_object_new_object();
        jNetName = json_object_new_string(arr[i]);
        jIPAddress = json_object_new_string(GetIPAddress(fd, ifr));
        jNetmask = json_object_new_string(GetNetmask(fd, ifr));
        jBroadcast = json_object_new_string(GetBroadcast(fd, ifr));
        jMetric = json_object_new_int(GetMetric(fd, ifr));
        jMTU = json_object_new_int(GetMTU(fd, ifr));
        snprintf(path, 60, "/sys/class/net/%s/address", arr[i]);
        GetParamFromFile(path, buffer);
        jMacAddress = json_object_new_string(buffer);
        snprintf(path, 60, "/sys/class/net/%s/operstate", arr[i]);
        GetParamFromFile(path, buffer);
        jOperstate = json_object_new_string(buffer);
        snprintf(path, 60, "/sys/class/net/%s/tx_queue_len", arr[i]);
        GetParamFromFile(path, buffer);
        data = strtol(buffer, NULL, 10);
        jtx_queue_len = json_object_new_int(data);

        json_object_object_add(jobnet, "name_Network", jNetName);
        json_object_object_add(jobnet, "IP_Address", jIPAddress);
        json_object_object_add(jobnet, "Broadcast", jBroadcast);
        json_object_object_add(jobnet, "Netmask", jNetmask);
        json_object_object_add(jobnet, "Macaddress", jMacAddress);
        json_object_object_add(jobnet, "Metric", jMetric);
        json_object_object_add(jobnet, "MTU", jMTU);
        json_object_object_add(jobnet, "Operstate", jOperstate);
        json_object_object_add(jobnet, "tx_queue_len", jtx_queue_len);
        snprintf(pathStatistics, 70, "/sys/class/net/%s/statistics", arr[i]);

        if ((dir = opendir(pathStatistics)) != NULL)
        {
            index = 0;
            readdir(dir);
            readdir(dir);
            while ((ent = readdir(dir)) != NULL)
            {
                snprintf(pathFile, 80, "/sys/class/net/%s/statistics/%s", arr[i], ent->d_name);
                GetParamFromFile(pathFile, buffer);
                if (IsNumber(buffer))
                {
                    data = strtol(buffer, NULL, 10);
                    jField[index] = json_object_new_int(data);
                }
                else
                {
                    jField[index] = json_object_new_string(buffer);
                }
                json_object_object_add(jobnet, ent->d_name, jField[index]);
                index++;
            }
            closedir(dir);
        }
        json_object_array_add(jobnets, jobnet);
    }

    printf("%s\n", json_object_to_json_string(jobnets));

    close(fd);

    return 0;
}

/*
============================================================================================
General : managerInterfacesPost -POST manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerInterfacesPost(QUERY *list, unsigned short len)
{
    short nameConf = isNameQueryExist(list, len, "nameConf");

    if (list[nameConf].info)
    {
        configInterface(list, len, list[nameConf].info);
        messageJson("The update was successful ", 10);
    }

    return 0;
}

void configInterface(QUERY *list, unsigned short len, char *nameconf)
{
    struct ifreq ifr;
    int skfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    ifr.ifr_addr.sa_family = AF_INET;
    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
    short newName = isNameQueryExist(list, len, "newName");
    short metric = isNameQueryExist(list, len, "metric");
    short ipConf = isNameQueryExist(list, len, "ip");
    short netmaskConf = isNameQueryExist(list, len, "netmask");
    short qlenConf = isNameQueryExist(list, len, "qlen");
    short broadcastConf = isNameQueryExist(list, len, "broadcast");
    short flagConf = isNameQueryExist(list, len, "flag");
    short mtu = isNameQueryExist(list, len, "mtu");
    strncpy(ifr.ifr_name, nameconf, IFNAMSIZ);

    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket(PF_INET, SOCK_DGRAM, 0)");
    }

    //Changes the name of the interface specified in ifr_name to ifr_newname.
    // This is a privileged operation. It is only allowed when the interface is not up.
    if (newName != -1)
    {
        strncpy(ifr.ifr_newname, list[newName].info, IFNAMSIZ);
        if (ioctl(skfd, SIOCSIFNAME, &ifr) == -1)
        {
            messageJson("not working change name", 0);
            perror("ioctl(SIOCSIFNAME)");
        }
    }

    if (netmaskConf != -1)
    {
        puts(list[netmaskConf].info);
        inet_pton(AF_INET, list[netmaskConf].info, &addr->sin_addr);
        if (ioctl(skfd, SIOCSIFNETMASK, &ifr) == -1)
        {
            messageJson("not working change netmask", 0);
            perror("ioctl(SIOCSIFNETMASK)");
        }
    }

    if (metric != -1)
    {
        ifr.ifr_metric = strtol(list[metric].info, NULL, 10); //your MTU size here
        if (ioctl(skfd, SIOCSIFMETRIC, (caddr_t)&ifr) < 0)
        {
            messageJson("not working change metric", 0);
            perror("ioctl(SIOCSIFMETRIC)");
        }
    }

    if (mtu != -1)
    {
        ifr.ifr_mtu = strtol(list[mtu].info, NULL, 10); //your MTU size here
        if (ioctl(skfd, SIOCSIFMTU, (caddr_t)&ifr) < 0)
        {
            messageJson("not working change mtu", 0);
            perror("ioctl(SIOCSIFMTU)");
        }
    }

    if (ipConf != -1)
    {
        inet_pton(AF_INET, list[ipConf].info, &addr->sin_addr);
        if (ioctl(skfd, SIOCSIFADDR, &ifr) == -1)
        {
            messageJson("not working change ip", 0);
            perror("ioctl(SIOCSIFADDR)");
        }
    }

    if (broadcastConf != -1)
    {
        puts(list[broadcastConf].info);
        inet_pton(AF_INET, list[broadcastConf].info, &addr->sin_addr);
        if (ioctl(skfd, SIOCSIFHWBROADCAST, &ifr) == -1)
        {
            messageJson("not working change broadcastConf", 0);
            perror("ioctl(SIOCSIFHWBROADCAST)");
        }
    }

    if (qlenConf != -1)
    {
        ifr.ifr_mtu = strtol(list[qlenConf].info, NULL, 10);
        if (ioctl(skfd, SIOCSIFTXQLEN, &ifr) == -1)
        {
            messageJson("not working change qlenConf", 0);
            perror("ioctl(SIOCSIFTXQLEN)");
        }
    }

    if (flagConf != -1)
    {
        short flag = strtol(list[flagConf].info, NULL, 10);
        ifr.ifr_flags = (flag) ? IFF_UP | flag : flag & ~IFF_UP;
        if (ioctl(skfd, SIOCSIFFLAGS, &ifr) == -1)
        {
            messageJson("not working change flagConf", 0);
            perror("ioctl(SIOCSIFFLAGS)");
        }
    }
}
