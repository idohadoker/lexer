#include "route.h"

// bool addNullRoute(long host)
// {
//     // create the control socket.
//     int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

//     struct rtentry route;
//     memset(&route, 0, sizeof(route));

//     // set the gateway to 0.
//     struct sockaddr_in *addr = (struct sockaddr_in *)&route.rt_gateway;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = 0;

//     // set the host we are rejecting.
//     addr = (struct sockaddr_in *)&route.rt_dst;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = htonl(host);

//     // Set the mask. In this case we are using 255.255.255.255, to block a single
//     // IP. But you could use a less restrictive mask to block a range of IPs.
//     // To block and entire C block you would use 255.255.255.0, or 0x00FFFFFFF
//     addr = (struct sockaddr_in *)&route.rt_genmask;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = 0xFFFFFFFF;

//     // These flags mean: this route is created "up", or active
//     // The blocked entity is a "host" as opposed to a "gateway"
//     // The packets should be rejected. On BSD there is a flag RTF_BLACKHOLE
//     // that causes packets to be dropped silently. We would use that if Linux
//     // had it. RTF_REJECT will cause the network interface to signal that the
//     // packets are being actively rejected.
//     route.rt_flags = RTF_UP | RTF_HOST | RTF_REJECT;
//     route.rt_metric = 0;

//     // this is where the magic happens..
//     if (ioctl(fd, SIOCADDRT, &route))
//     {
//         close(fd);
//         return 0;
//     }

//     // remember to close the socket lest you leak handles.
//     close(fd);
//     return 1;
// }

// bool delNullRoute(long host)
// {
//     int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

//     struct rtentry route;
//     memset(&route, 0, sizeof(route));

//     struct sockaddr_in *addr = (struct sockaddr_in *)&route.rt_gateway;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = 0;

//     addr = (struct sockaddr_in *)&route.rt_dst;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = htonl(host);

//     addr = (struct sockaddr_in *)&route.rt_genmask;
//     addr->sin_family = AF_INET;
//     addr->sin_addr.s_addr = 0xFFFFFFFF;

//     route.rt_flags = RTF_UP | RTF_HOST | RTF_REJECT;
//     route.rt_metric = 0;

//     // this time we are deleting the route:
//     if (ioctl(fd, SIOCDELRT, &route))
//     {
//         close(fd);
//         return 0;
//     }

//     close(fd);
//     return 1;
// }

char **GetTitleTableRoute(unsigned short *count, char *buffer)
{
    char **ArrTitle;
    *count = 0;
    char *start = buffer;
    char *tmp = buffer;
    ArrTitle = (char **)malloc(sizeof(char *));
    unsigned short countSpace = 0, len = 0, countSpaceTemp = 0;
    unsigned short flagNewField = 0;
    while (*tmp)
    {
        if (((countSpace >= 1) && (*tmp != ' ')) || (*(tmp + 1) == '\0'))
        {
            flagNewField = 1;
            countSpaceTemp = countSpace;
        }

        if (*tmp == '\t')
        {
            countSpace++;
        }
        else
        {
            countSpace = 0;
        }

        if (flagNewField)
        {
            len = strlen(start) - countSpaceTemp - strlen(tmp);
            (*count)++;
            ArrTitle[*count - 1] = malloc(sizeof(char) * len);
            strncpy(ArrTitle[*count - 1], start, len);
            start = tmp;
            ArrTitle = (char **)realloc(ArrTitle, sizeof(char *) * ((*count) + 1));
            flagNewField = 0;
        }
        tmp++;
    }
    return ArrTitle;
}

int Create_Route_Table_Json(int fd)
{
    char buffer[BUFSIZ];
    readLine(fd, buffer);

    char **arrValues;
    unsigned short count = 0;
    arrValues = GetTitleTableRoute(&count, buffer);

    json_object *jobRouteTables = json_object_new_array();

    json_object *Iface;
    json_object *Destination;
    json_object *Getway;
    json_object *Flags;
    json_object *RefCnt;
    json_object *Metric;
    json_object *Mask;
    json_object *MTU;
    json_object *Window;
    json_object *IRTT;

    while (!readLine(fd, buffer))
    {
        json_object *jobRoute = json_object_new_object();
        arrValues = GetTitleTableRoute(&count, buffer);

        Iface = json_object_new_string(arrValues[0]);
        Destination = json_object_new_string(arrValues[1]);
        Getway = json_object_new_string(arrValues[2]);
        Flags = json_object_new_string(arrValues[3]);
        RefCnt = json_object_new_string(arrValues[4]);
        Metric = json_object_new_string(arrValues[5]);
        Mask = json_object_new_string(arrValues[6]);
        MTU = json_object_new_string(arrValues[7]);
        Window = json_object_new_string(arrValues[8]);
        IRTT = json_object_new_string(arrValues[9]);

        json_object_object_add(jobRoute, "Iface", Iface);
        json_object_object_add(jobRoute, "Destination", Destination);
        json_object_object_add(jobRoute, "Getway", Getway);
        json_object_object_add(jobRoute, "Flags", Flags);
        json_object_object_add(jobRoute, "RefCnt", RefCnt);
        json_object_object_add(jobRoute, "Metric", Metric);
        json_object_object_add(jobRoute, "Mask", Mask);
        json_object_object_add(jobRoute, "MTU", MTU);
        json_object_object_add(jobRoute, "Window", Window);
        json_object_object_add(jobRoute, "IRTT", IRTT);

        json_object_array_add(jobRouteTables, jobRoute);
        free(arrValues);
    }
    printf("%s", json_object_to_json_string(jobRouteTables));
    return 0;
}

/*
============================================================================================
General : managerRoute -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerRoute(QUERY *list, unsigned short len)
{
    int fd = open(ROUTE_CACHE, O_RDONLY);

    int status = 0;
    if (fd < 0)
    {
        printf("error\n");
        exit(1);
    }
    status = Create_Route_Table_Json(fd);
    return status;
}