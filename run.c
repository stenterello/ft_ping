#include "ft_ping.h"

int		about_to_quit = 0;

void	print_statistics(const t_config *config)
{
	printf("\n--- %s ping statistics ---\n", config->dst_addr);
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
    struct sockaddr_in  local_addr;
    struct timeval      now;
    struct timeval      last;

    memset(&dst_addr, 0, sizeof(dst_addr));
    memset(&local_addr, 0, sizeof(local_addr));
    memset(&now, 0, sizeof(now));
    memset(&last, 0, sizeof(last));

    resolve_address(config, &dst_addr);

    first_line_info(config, &dst_addr.sin_addr);

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock < 0)
	{
		perror("socket");
	}

    int one = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
               &one, sizeof(one)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = IP_BIND_ADDRESS_NO_PORT;

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0)
    {
        perror("bind");
        fatal("");
    }

    fd_set   set;

	socklen_t	size = sizeof(dst_addr);
    FD_ZERO(&set);
    FD_SET(sock, &set);
    for ( ; about_to_quit == 0 ; )
    {
        gettimeofday(&now, NULL);
        if ((now.tv_sec == 0 && now.tv_usec == 0) || calculate_interval(&last, &now) > DEFAULT_INTERVAL)
        {
            gettimeofday(&last, NULL);
	        int bytes = sendto(sock, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)&dst_addr, size);
            if (bytes < 0)
            {
                perror("sendto");
            }
            printf("sent!\n");
            buffer[SEQ + 1] = *(&buffer[SEQ + 1]) + 1;
        }
        else
        {
            continue;
        }

        char tmp_buff[1000];
        memset(tmp_buff, 0, 1000);
        
        // TODO: add timeout to select
        int r = select(sock + 1, &set, NULL, NULL, NULL);
        if (r == -1)
        {
            perror("select");
            fatal("");
        }
        else if (r)
        {
            printf("leggendo\n");
            int readbytes = recv(sock, &tmp_buff[0], 1000, 0);
            printf("read bytes: %d\n", readbytes);
            if (readbytes < 0)
            {
                perror("recv");
            }
            for (int i = 0; i < readbytes; i++)
            {
                printf("|%02u|", tmp_buff[i]);
            }
            printf("\n");
            
        }
    }


	// printf("sent %d bytes\n", bytes);


	close(sock);

	free(buffer);

    print_statistics(config);
}