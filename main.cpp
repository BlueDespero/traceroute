#include "traceroute.h"

int main(int argc, char const *argv[])
{
    int32_t target_ip, ip_ans_1 = -1, ip_ans_2 = -1, ip_ans_3 = -1;
    int32_t avr_time = -1;
    char *ip_str = strdup(argv[1]);

    if (check_input(argc, argv, target_ip))
        return 1;


    for (int i = 1; i <= 255 && ip_ans_1 != target_ip && ip_ans_2 != target_ip && ip_ans_3 != target_ip; i++)
    {
        ip_ans_1 = -1; ip_ans_2 = -1; ip_ans_3 = -1;
        if (make_req(ip_str, i, ip_ans_1, ip_ans_2, ip_ans_3, avr_time))
            return 1;
        print_results(i, ip_ans_1, ip_ans_2, ip_ans_3, avr_time);
    }

    return 0;
}
