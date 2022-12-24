#ifndef PING_H // PING_CONF_H
#define PING_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include "commonFun.h"
#include <pthread.h>

// Define the Packet Constants
// ping packet size
#define PING_PKT_S 64

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1

// Define the Ping Loop

// ping packet structure
struct ping_pkt
{
    struct icmphdr hdr;
    char msg[PING_PKT_S - sizeof(struct icmphdr)];
};

// Calculating the Check Sum
unsigned short checksum(void *b, int len);

// Interrupt handler
void intHandler(int dummy);

// Resolves the reverse lookup of the hostname
char *reverse_dns_lookup(char *ip_addr, json_object *jobPing);

// make a ping request
void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom, char *ping_ip, char *rev_host, json_object *jobPing);

// Performs a DNS lookup
char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con);

short managerPing(QUERY *list, unsigned short len);
#endif