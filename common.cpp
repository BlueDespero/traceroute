#include "traceroute.h"

bool check_input(int argc, char const *argv[], int32_t &target_ip)
{
    if (argc != 2)
    {
        printf("Too many arguments provided. This program takes just target IP address.\n");
        return 1;
    }
    

    if (inet_pton(AF_INET, argv[1], &target_ip) != 1)
    {
        printf("IP address you provided is incorrect.\n");
        return 1;
    }
    
    return 0;
}

void print_results(int i, int32_t ip_1, int32_t ip_2, int32_t ip_3, int32_t avr_time)
{
    printf("%d. ", i);

    if (ip_1 != -1)
    {
        char ip[20];
        inet_ntop(AF_INET, &ip_1, ip, sizeof(ip));
        printf("%s ", ip);

        if(ip_2!=-1 && ip_1!=ip_2)
        {
            inet_ntop(AF_INET, &ip_2, ip, sizeof(ip));
            printf("%s ", ip);
        }

        if(ip_3!=-1 && ip_1!=ip_3 && ip_2!=ip_3)
        {
            inet_ntop(AF_INET, &ip_3, ip, sizeof(ip));
            printf("%s ", ip);
        }

        if (avr_time != -1)
            printf("%" PRId32 "ms\n", avr_time);
        else
            printf("???");
    }
    else
        printf("*");
    
    printf("\n");
}

bool make_req(char *target_ip, int i, int32_t &ip_1, int32_t &ip_2, int32_t &ip_3, int32_t &avr_time)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return 1;
	} 

	if(send_req(target_ip, i, sockfd))
        return 1;
    if(rec_req(i, ip_1, ip_2, ip_3, avr_time, sockfd))
        return 1;
    return 0;
}
