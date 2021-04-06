#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cstdint>
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <inttypes.h>


bool check_input(int argc, char const *argv[], int32_t &target_ip);
void print_results(int i, int32_t ip_1, int32_t ip_2, int32_t ip_3, int32_t avr_time);
bool make_req(char *target_ip, int i, int32_t &ip_1, int32_t &ip_2, int32_t &ip_3, int32_t &avr_time);

bool send_req(char *target_ip, int i, int sockfd);

bool rec_req(int i, int32_t &ip_1, int32_t &ip_2, int32_t &ip_3, int32_t &avr_time, int sockfd);