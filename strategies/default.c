#include "ft_ping.h"

void    print_buffer(char *buffer)
{
    char* ptr = buffer;
    for (int i = 0; i < (int)sizeof(buffer); i++)
	{
		if (i % 8 == 0)
		{
			printf("\n");
		}
		printf("|%02x|", ptr[i]);
	}
}
/*
void	set_checksum(char *buffer, struct icmp_packet *packet) {
*//*	char *tmp_buffer;
	size_t len;*//*

	*//*if ((sizeof(buffer) + sizeof(packet->payload)) % 2 != 0) {
		tmp_buffer = malloc(sizeof(buffer) + sizeof(packet->payload) + 1);
		if (!tmp_buffer) {
			fatal("Malloc error\n");
		}
		len = sizeof(buffer) + sizeof(packet->payload) + 1;
		memcpy(tmp_buffer, buffer, sizeof(buffer));
		memcpy(&tmp_buffer[sizeof(buffer)], packet->payload, sizeof(packet->payload));
		memcpy(&tmp_buffer[sizeof(buffer) + sizeof(packet->payload) + 1], "0", 1);
	}
	else
	{
		tmp_buffer = malloc(sizeof(buffer) + sizeof(packet->payload));
		if (!tmp_buffer) {
			fatal("Malloc error\n");
		}
		len = sizeof(buffer) + sizeof(packet->payload);
		memcpy(tmp_buffer, buffer, sizeof(buffer));
		memcpy(&tmp_buffer[sizeof(buffer)], packet->payload, sizeof(packet->payload));
	}*//*

*//*	uint16_t	checksum = 0;

	for (int i = 0; i < (int)len; i++)
	{
		uint16_t	word = (tmp_buffer[i] << 8) + tmp_buffer[i];
		checksum += word;
		checksum = (checksum &0xFFFF) + (checksum >> 16);
	}

	checksum = ~checksum & 0xFFFF;

	free(tmp_buffer);*//*
}*/

void    send_echo_request(struct icmp_packet *packet, int fd)
{
	struct sockaddr_in	dst_addr;

	memset(&dst_addr, 0, sizeof(dst_addr));

	dst_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1", &dst_addr.sin_addr);

	char *buffer = malloc(sizeof(char) * (packet->size));
	if (!buffer)
	{
		fatal("Malloc error\n");
	}

	memset(buffer, 0, packet->size);

	buffer[TYPE] = 8;
	buffer[CODE] = 0;
	buffer[CHECKSUM] = 0;
	buffer[CHECKSUM + 1] = 0;
	uint16_t id = 0x1234;
	memcpy(&buffer[ID], &id, 2);
	uint16_t  seq = 0x0001;
	memcpy(&buffer[SEQ], &seq, 2);
	for (int i = 0; i < 11; i++) {
		memcpy(&buffer[DATA + (4 * i)], "Ciao", 4);
	}



	(void)(packet);

	printf("sending data...\n");

	socklen_t addr_len = sizeof(dst_addr);
	if (sendto(fd, buffer, packet->size, 0, (struct sockaddr*)&dst_addr, addr_len) < 0)
	{
		perror("send");
	}


}

void	infinite(struct icmp_packet *packet, t_options *options)
{
	(void)packet;
    struct timeval time;

	gettimeofday(&time, NULL);

    int	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	if (sock < 0)
	{
		fatal("Error opening socket\n");
	}

    for (int i = 0; i < options->preload; i++)
    {
        send_echo_request(packet, sock);
    }

    fd_set  fdset;
    int     fdmax;

    fdmax = sock + 1;

	for ( ; !about_to_quit ; )
	{
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        int s = select(fdmax, &fdset, NULL, NULL, &time);
        if (s < 0)
        {
            perror("select");
        }

	}

    close(sock);
    printf("Socket closed\n");
}
