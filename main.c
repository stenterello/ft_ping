#include "ft_ping.h"

int		about_to_quit = 0;

void	print_statistics()
{
	
}

void    print_configuration(t_config *config)
{
    printf("---- CONFIGURATION ----\n");
    printf("verbose: %d\n", config->verbose);
    printf("flood: %d\n", config->flood);
    printf("preload: %d\n", config->preload);
    printf("no_dns: %d\n", config->no_dns);
    printf("deadline: %d\n", config->deadline);
    printf("timeout: %d\n", config->timeout);
    printf("pattern: %s\n", config->pattern);
    printf("bypass_routing: %d\n", config->bypass_routing);
    printf("packet_size: %d\n", config->packet_size);
    printf("timestamp_flag: %d\n", config->timestamp_flag);
    printf("ttl: %d\n", config->ttl);
    printf("ip_timestamp: %d\n", config->ip_timestamp);
    printf("\n\n");
}

void    first_line_info(const t_config *config, char *resolved)
{
    printf("PING %s (%s): %d data bytes\n", config->dst_addr, resolved, config->packet_size);
}

char    *resolve_address(char *dst_addr)
{
    struct addrinfo     *addrinfo = NULL;
    char                addr_buf[INET_ADDRSTRLEN];
    struct addrinfo     hint;

    memset(addr_buf, 0, sizeof(addr_buf));
    memset(&hint, 0, sizeof(hint));

    hint.ai_family = AF_INET;

    getaddrinfo(dst_addr, NULL, &hint, &addrinfo);
    if (addrinfo == NULL)
    {
        fatal("ft_ping: unknown host\n");
    }

    inet_ntop(AF_INET, &((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr, addr_buf, sizeof(addr_buf));

    char *ret = malloc(sizeof(char) * (strlen(addr_buf) + 1));
    if (!ret)
    {
        fatal("Malloc error\n");
    }

    strncpy(ret, addr_buf, strlen(addr_buf));

    freeaddrinfo(addrinfo);
    
    return ret;
}

void    run(const t_config *config)
{
    char                *buffer = craft_packet(config);
    char                *resolved = resolve_address(config->dst_addr);
    struct sockaddr_in  dst_addr;


    memset(&dst_addr, 0, sizeof(dst_addr));

    dst_addr.sin_family = AF_INET;
    inet_aton(resolved, &dst_addr.sin_addr);

    first_line_info(config, resolved);

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
    free(resolved);
}

int main(int argc, char **argv)
{
	handle_sigint();
	
    t_config	config = default_config();

	// apply command line configurations
	apply_configuration(argc, argv, &config);

    // print_configuration(&config);

    run(&config);
    free(config.dst_addr);

	print_statistics();

	return 0;
}


