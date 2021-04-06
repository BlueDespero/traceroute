#include "traceroute.h"

fd_set get_descriptors(int sockfd)
{
	fd_set descriptors;
	FD_ZERO (&descriptors);
	FD_SET (sockfd, &descriptors);
	
	return descriptors;
}

struct icmp* get_icmp_header(struct sockaddr_in sender, u_int8_t *buffer)
{
	char sender_ip_str[20];
	inet_ntop (
	AF_INET,
	&(sender.sin_addr),
	sender_ip_str,
	sizeof(sender_ip_str)
	);

	struct ip* ip_header = (struct ip*) buffer;
	u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
	struct icmp* icmp_header = (struct icmp*) icmp_packet;

	return icmp_header;
}

bool rec_req(int i, int32_t &ip_1, int32_t &ip_2, int32_t &ip_3, int32_t &avr_time, int sockfd)
{
	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer [IP_MAXPACKET];

	fd_set descriptors = get_descriptors(sockfd);
	struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;

	int collected = 0, time = 0;
	while ((tv.tv_sec != 0 || tv.tv_usec != 0 ) && collected < 3)
	{
		int ready = select (sockfd+1, &descriptors, NULL, NULL, &tv);
		if (ready>0)
		{
			if(recvfrom (
			sockfd,
			buffer,
			IP_MAXPACKET,
			MSG_DONTWAIT,
			(struct sockaddr*)&sender,
			&sender_len
			) == -1)
			{
				fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
				return 1;
			};

			struct icmp* icmp_header = get_icmp_header(sender, buffer);
			if (icmp_header->icmp_hun.ih_idseq.icd_id == getpid() && icmp_header->icmp_hun.ih_idseq.icd_seq == i)
			{
				collected++;
				time += ((1000000 - tv.tv_usec - tv.tv_sec*1000000)/1000);
				switch (collected)
				{
				case 1:
					ip_1 = sender.sin_addr.s_addr;
					break;
				case 2:
					ip_2 = sender.sin_addr.s_addr;
					break;
				case 3:
					ip_3 = sender.sin_addr.s_addr;
					avr_time = time/3;
					break;
				}
			}
		}
	}

	return 0;
}
