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

char    *craft_packet(const t_config *config)
{
    char *packet = malloc(sizeof(char) * (config->packet_size + ICMP_HEADER_SIZE));
    if (!packet)
    {
        fatal("Malloc error\n");
    }

    packet[TYPE] = 8;
    packet[CODE] = 0;
    packet[CHECKSUM] = 0;
    packet[CHECKSUM + 1] = 0;
    short identifier = 0x1234;
    short sequence = 0x0001;
    packet[ID] = identifier << 8;
    packet[ID + 1] = identifier >> 8;
    packet[SEQ] = sequence << 8;
    packet[SEQ + 1] = sequence >> 8;
    
    return packet;
}

void    run(const t_config *config)
{
    char    *buffer = craft_packet(config);

	struct sockaddr_in	dst_addr;

	memset(&dst_addr, 0, sizeof(struct sockaddr_in));

	dst_addr.sin_family = AF_INET;
	inet_aton(config->dst_addr, &dst_addr.sin_addr);

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sock < 0)
	{
		perror("socket");
	}

	socklen_t	size = sizeof(dst_addr);
	int bytes = sendto(sock, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)&dst_addr, size);
	printf("sent %d bytes\n", bytes);


	close(sock);
}

t_config    default_config()
{
    t_config    ret;

    ret.verbose = 0;
    ret.flood = 0;
    ret.preload = 0;
    ret.no_dns = 0;
    ret.deadline = -1;
    ret.timeout = 10;
    ret.pattern = NULL;
    ret.bypass_routing = 0;
    ret.packet_size = 56;
    ret.timestamp_flag = -1;
    ret.ttl = 0;
    ret.ip_timestamp = 0;

    return ret;
}

int main(int argc, char **argv)
{
	handle_sigint();
	
    t_config	config = default_config();

	// apply command line configurations
	apply_configuration(argc, argv, &config);

    print_configuration(&config);

    run(&config);

	print_statistics();

	return 0;
}


