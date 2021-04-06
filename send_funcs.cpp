#include "traceroute.h"

uint16_t compute_icmp_checksum (const uint16_t *buff, int length)
{
	uint32_t sum;
	const uint16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (uint16_t)(~(sum + (sum >> 16)));
}

struct icmp get_header(int ttl)
{
	struct icmp header;
	header.icmp_type = ICMP_ECHO;
	header.icmp_code = 0;
	header.icmp_hun.ih_idseq.icd_id = getpid();
	header.icmp_hun.ih_idseq.icd_seq = ttl;
	header.icmp_cksum = 0;
	header.icmp_cksum = compute_icmp_checksum (
	(u_int16_t*)&header, sizeof(header));

	return header;
}

struct sockaddr_in get_recipient(char * target_ip)
{
	struct sockaddr_in recipient;
	bzero (&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	inet_pton(AF_INET, target_ip, &recipient.sin_addr); 

	return recipient;
}

bool send_data(int sockfd, struct icmp header, struct sockaddr_in recipient)
{
	ssize_t bytes_sent = sendto (
		sockfd,
		&header,
		sizeof(header),
		0,
		(struct sockaddr*)&recipient,
		sizeof(recipient)
	);

	if (bytes_sent == -1)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}

bool send_req(char * target_ip, int ttl, int sockfd)
{
	setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

	struct icmp header = get_header(ttl);
	struct sockaddr_in recipient = get_recipient(target_ip);

	for (int i = 0; i < 3; i++)
		if (send_data(sockfd, header, recipient))
			return 1;

	return 0;
}
