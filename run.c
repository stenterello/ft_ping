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

int     calculate_interval(const struct timeval *t1, const struct timeval *t2)
{
    return ((t2->tv_sec * 1000 + t2->tv_usec / 1000) - (t1->tv_sec * 1000 + t1->tv_usec / 1000));
}

void    run(const t_config *config)
{
    char                *buffer = craft_packet(config);
    struct sockaddr_in  dst_addr;
    struct timeval      now;
    struct timeval      last;

    memset(&dst_addr, 0, sizeof(dst_addr));
    memset(&now, 0, sizeof(now));
    memset(&last, 0, sizeof(last));

    resolve_address(config, &dst_addr);

    first_line_info(config, &dst_addr.sin_addr);

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock < 0)
	{
		perror("socket");
	}

    // gettimeofday(&last, NULL);
    // int bytes = sendto(sock, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)&dst_addr, size);
    // if (bytes < 0)
    // {
    //     perror("sendto");
    // }

	socklen_t	size = sizeof(dst_addr);
    for ( ; about_to_quit == 0 ; )
    {
        if ((now.tv_sec == 0 && now.tv_usec == 0) || calculate_interval(&last, &now) > DEFAULT_INTERVAL)
        {
            gettimeofday(&last, NULL);
	        int bytes = sendto(sock, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)&dst_addr, size);
            if (bytes < 0)
            {
                perror("sendto");
            }
            printf("sent!\n");
        }
        gettimeofday(&now, NULL);
    }


	// printf("sent %d bytes\n", bytes);


	close(sock);

	free(buffer);

    print_statistics();
}