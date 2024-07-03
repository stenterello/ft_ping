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

double     calculate_interval(const struct timeval *t1, const struct timeval *t2)
{
    return ((t2->tv_sec * 1000 + t2->tv_usec / 1000) - (t1->tv_sec * 1000 + t1->tv_usec / 1000));
}

double      calculate_interval_micro(const struct timeval *t1, const struct timeval *t2)
{
    return ((t2->tv_sec * 1000000 + t2->tv_usec) - (t1->tv_sec * 1000000 + t1->tv_usec));
}

void    print_received_info(char *res, int len, char *latency_string)
{
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%s ms\n", len, "test", res[SEQ + 1], 1, latency_string);
}

char    *get_latency(struct timeval *last, struct timeval *latency)
{
    printf("last value is: \t\t%ld\n", last->tv_sec * 1000000 + last->tv_usec);
    printf("latency value is: \t%ld\n", latency->tv_sec * 1000000 + latency->tv_usec);
    double ms = calculate_interval_micro(last, latency);

    printf("interval is: %f\n", ms);

    char *ret = malloc(sizeof(char) * 6);
    if (!ret)
    {
        fatal("Malloc error\n");
    }

    sprintf(ret, "%f", ms / 1000);

    printf("microsecs are %s\n", ret);

    return ret;
}



void    run(const t_config *config)
{
    char                *buffer = craft_packet(config);
    struct sockaddr_in  dst_addr;
    struct timeval      now;
    struct timeval      last;
    struct timeval      latency;

    memset(&dst_addr, 0, sizeof(dst_addr));
    memset(&now, 0, sizeof(now));
    memset(&last, 0, sizeof(last));
    memset(&latency, 0, sizeof(latency));

    resolve_address(config, &dst_addr);
    first_line_info(config, &dst_addr.sin_addr);

    int sock = open_socket();

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
            buffer[SEQ + 1] = *(&buffer[SEQ + 1]) + 1;
            printf("sent!\n");
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
            gettimeofday(&latency, NULL);
            char *latency_string = get_latency(&last, &latency);
            print_received_info(tmp_buff, readbytes, latency_string);
            free(latency_string);
        }
    }

	close(sock);
	free(buffer);

    print_statistics(config);
}