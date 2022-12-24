#include "ping.h"

// Define the Ping Loop
int pingloop = 1;

// Calculating the Check Sum
unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

// Interrupt handler
void intHandler(int dummy)
{
    pingloop = 0;
}

// Performs a DNS lookup
char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
    struct hostent *host_entity;
    char *ip = (char *)malloc(NI_MAXHOST * sizeof(char));

    if ((host_entity = gethostbyname(addr_host)) == NULL)
    {
        // No ip found for hostname
        return NULL;
    }

    //filling up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));

    (*addr_con).sin_family = host_entity->h_addrtype;
    (*addr_con).sin_port = htons(PORT_NO);
    (*addr_con).sin_addr.s_addr = *(long *)host_entity->h_addr;

    return ip;
}

// Resolves the reverse lookup of the hostname
char *reverse_dns_lookup(char *ip_addr, json_object *jobPing)
{
    struct sockaddr_in temp_addr;
    socklen_t len;
    char buf[NI_MAXHOST], *ret_buf;

    temp_addr.sin_family = AF_INET;
    temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *)&temp_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
    {
        messageJson("Could not resolve reverse lookup of hostname", 0);
    }
    ret_buf = (char *)malloc((strlen(buf) + 1) * sizeof(char));
    strcpy(ret_buf, buf);
    return ret_buf;
}

// make a ping request
void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom, char *ping_ip, char *rev_host, json_object *jobPing)
{
    int ttl_val = 64, msg_count = 0, i, flag = 1, msg_received_count = 0;
    socklen_t addr_len;
    struct ping_pkt pckt;
    struct sockaddr_in r_addr;
    struct timespec time_start, time_end, tfs, tfe;
    long double rtt_msec = 0;
    // total_msec = 0;
    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;

    clock_gettime(CLOCK_MONOTONIC, &tfs);

    // set socket options at ip to TTL and value to 64,
    // change to what you want by setting ttl_val
    if (setsockopt(ping_sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0)
    {
        messageJson("Setting socket options to TTL failed!", 0);
    }

    // setting timeout of recv setting
    setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv_out, sizeof tv_out);

    for (int te = 0; te < 20; te++)
    {
        delay(2);

        // flag is whether packet was sent or not
        flag = 1;
        //filling packet
        bzero(&pckt, sizeof(pckt));

        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        for (i = 0; i < sizeof(pckt.msg) - 1; i++)
            pckt.msg[i] = i + '0';

        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

        usleep(PING_SLEEP_RATE);

        //send packet
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        if (sendto(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr *)ping_addr, sizeof(*ping_addr)) <= 0)
        {
            printf("\nPacket Sending Failed!\n");
            flag = 0;
        }

        //receive packet
        addr_len = sizeof(r_addr);

        if ((recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr *)&r_addr, &addr_len) <= 0) && (msg_count > 1))
        {
            printf("\nPacket receive failed!\n");
        }
        else
        {
            clock_gettime(CLOCK_MONOTONIC, &time_end);
            double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
            rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;

            // if packet was not sent, don't receive
            if (flag)
            {
                if (!(pckt.hdr.type == 69 && pckt.hdr.code == 0))
                {
                    printf("Error..Packet received with ICMP type %d code %d\n", pckt.hdr.type, pckt.hdr.code);
                }
                else
                {

                    json_object *message = json_object_new_string("success!");
                    json_object *jStatus = json_object_new_boolean(10);
                    json_object *jBytes = json_object_new_int(PING_PKT_S);
                    json_object *jPingDom = json_object_new_string(ping_dom);
                    json_object *jttl_val = json_object_new_int(ttl_val);
                    json_object *jrtt_msec = json_object_new_double(rtt_msec);

                    json_object_object_add(jobPing, "status", jStatus);
                    json_object_object_add(jobPing, "message", message);
                    json_object_object_add(jobPing, "bytes", jBytes);
                    json_object_object_add(jobPing, "ping_dom", jPingDom);
                    json_object_object_add(jobPing, "ttl", jttl_val);
                    json_object_object_add(jobPing, "rtt", jrtt_msec);
                    printf("%s", json_object_to_json_string(jobPing));

                    //  printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n", PING_PKT_S, ping_dom, rev_host, ping_ip, msg_count, ttl_val, rtt_msec);
                    msg_received_count++;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &tfe);
    // double timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0;

    // total_msec = (tfe.tv_sec - tfs.tv_sec) * 1000.0 + timeElapsed;

    //    printf("\n===%s ping statistics===\n", ping_ip);
    // printf("\n%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n", msg_count, msg_received_count, ((msg_count - msg_received_count) / msg_count) * 100.0, total_msec);
}

/*
============================================================================================
General : managerPing -GET manage the route of arp table.
Parameters : list - struct QUERY  , len - unsigned short
Return Value : return true or false
============================================================================================
*/
short managerPing(QUERY *list, unsigned short len)
{
    int sockfd;
    char *ip_addr, *reverse_hostname;
    struct sockaddr_in addr_con;
    short addrIndex = isNameQueryExist(list, len, "address");
    json_object *jobPing = json_object_new_object();

    ip_addr = dns_lookup(list[addrIndex].info, &addr_con);
    if (ip_addr == NULL)
    {
        messageJson("DNS lookup failed! Could not resolve hostname", 0);
    }
    reverse_hostname = reverse_dns_lookup(ip_addr, jobPing);

    //socket()
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        messageJson("Socket file descriptor not received", 0);
    }

    //send pings continuously
    send_ping(sockfd, &addr_con, reverse_hostname, ip_addr, list[addrIndex].info, jobPing);
    printf("%s", json_object_to_json_string(jobPing));

    return 0;
}