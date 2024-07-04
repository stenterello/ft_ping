#include "ft_ping.h"

int		about_to_quit = 0;

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
	struct timeval      now;
	struct timeval      last;
	t_stats				stats;
	
	memset(&dst_addr, 0, sizeof(dst_addr));
	memset(&now, 0, sizeof(now));
	memset(&last, 0, sizeof(last));
	memset(&stats, 0, sizeof(stats));
	
	resolve_address(config, &dst_addr);
	first_line_info(config, &dst_addr.sin_addr);

	int sock = open_socket();

	fd_set   set;

	FD_ZERO(&set);
	FD_SET(sock, &set);
	for ( ; about_to_quit == 0 ; )
	{
		gettimeofday(&now, NULL);
		if ((now.tv_sec == 0 && now.tv_usec == 0) || calculate_interval(&last, &now) > DEFAULT_INTERVAL)
		{
			send_ping(sock, &dst_addr, buffer, &last, config);
			buffer[SEQ + 1] = *(&buffer[SEQ + 1]) + 1;
			stats.tx_num++;
			// printf("sent! %d\n", buffer[SEQ + 1]);
		}
		else
		{
			continue;
		}
		
		read_reply(sock, &set, &last, &dst_addr.sin_addr, &stats);
	}
	
	close(sock);
	free(buffer);

	print_statistics(config, &stats);
}
