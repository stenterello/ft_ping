#include "ft_ping.h"
#include <stdio.h>

int	about_to_quit = 0;

void    first_line_info(const t_config *config, struct in_addr *resolved)
{
	char to_print[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, resolved, to_print, INET_ADDRSTRLEN);

	fflush(stdout);
	printf("PING %s (%s): %d data bytes\n", config->dst_addr, to_print, config->packet_size);
}

void    prepare_structs(struct sockaddr_in *dst_addr, struct timeval *now, struct timeval *last, t_stats *stats)
{
    memset(dst_addr, 0, sizeof(*dst_addr));
    memset(now, 0, sizeof(*now));
    memset(last, 0, sizeof(*last));
    memset(stats, 0, sizeof(*stats));
}

int time_to_send(const t_config *config, const t_stats *stats, double interval_passed, double interval)
{
    if ((config->preload > 0) 
        || stats->tx_num == 0
        || interval_passed > interval
        || (config->count > 0 && stats->tx_num < config->count))
    {
        return 1;
    }
    return 0;
}

void    run(t_config *config)
{
	char                *buffer = craft_packet(config);
	struct sockaddr_in  dst_addr;
	struct timeval      now;
	struct timeval      last;
	t_stats				stats;
	double				interval = DEFAULT_INTERVAL;

    prepare_structs(&dst_addr, &now, &last, &stats);
	setvbuf(stdout, NULL, _IOLBF, 0);
	resolve_address(config, &dst_addr);
	first_line_info(config, &dst_addr.sin_addr);
	if (config->flood)
	{
		interval = 100;
	}

	int sock = open_socket();

	fd_set   set;
	FD_ZERO(&set);

    for ( ; about_to_quit == 0 ; )
	{
	    FD_SET(sock, &set);
		gettimeofday(&now, NULL);

        if (time_to_send(config, &stats, calculate_interval(&last, &now), interval))
		{
			send_ping(sock, &dst_addr, buffer, &last, config);
			buffer[SEQ + 1] = *(&buffer[SEQ + 1]) + 1;
			stats.tx_num++;
			if (config->preload)
			{
				config->preload--;
			}
		}

        read_reply(sock, &set, &last, &dst_addr.sin_addr, &stats, config);

		if (config->count &&
			((stats.rx_num == config->count))) // TODO: Exit if last reading has exceeded timeout
		{
			about_to_quit = 1;
		}
	}

	close(sock);
	free(buffer);

	print_statistics(config, &stats);
}
