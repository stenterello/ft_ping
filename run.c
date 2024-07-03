#include "ft_ping.h"

int		about_to_quit = 0;

void	print_statistics()
{
	
}

void    first_line_info(const t_config *config, struct in_addr *resolved)
{
    char to_print[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, resolved, to_print, INET_ADDRSTRLEN);

    printf("PING %s (%s): %d data bytes\n", config->dst_addr, to_print, config->packet_size);
}

void    run(const t_config *config)
{
    char                *buffer = craft_packet(config);
    struct sockaddr_in  dst_addr;

    memset(&dst_addr, 0, sizeof(dst_addr));

    resolve_address(config, &dst_addr);

    first_line_info(config, &dst_addr.sin_addr);

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock < 0)
	{
		perror("socket");
	}

	socklen_t	size = sizeof(dst_addr);
	int bytes = sendto(sock, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)&dst_addr, size);

	printf("sent %d bytes\n", bytes);


	close(sock);

	free(buffer);

    print_statistics();
}