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

char	*default_payload(int size)
{
	char	*ret = malloc(sizeof(char) * size);
	if (!ret)
	{
		fatal("Malloc error\n");
	}

	for (int i = 0; i < size; i++)
	{
		ret[i] = i;
	}
	return ret;
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void print_packet(char *buffer, const t_config *config)
{
    for (int i = 0; i < config->packet_size; i++)
    {
        if (i % 8 == 0)
        {
            printf("\n");
        }
        printf("%u", buffer[i]);
    }
    printf("\n");
}

char    *craft_packet(const t_config *config)
{
    char *packet = malloc(sizeof(char) * (config->packet_size + ICMP_HEADER_SIZE));
    if (!packet)
    {
        fatal("Malloc error\n");
    }

    print_packet(packet, config);
    packet[TYPE] = 8;
    print_packet(packet, config);
    packet[CODE] = 0;
    print_packet(packet, config);
    packet[CHECKSUM] = 0;
    print_packet(packet, config);
    packet[CHECKSUM + 1] = 0;
    print_packet(packet, config);
    short identifier = 0x1234;
    short sequence = 0x0001;
	// packet[SEQ] = sequence;
    // packet[ID] = (char)(identifier << 8);
    // packet[ID + 1] = (char)(identifier >> 8);
	printBits(2, &sequence);
	// sequence >>= 8;
	printBits(2, &sequence);
    packet[SEQ] = sequence >> 8;
    print_packet(packet, config);
    packet[SEQ + 1] = (char)(sequence << 8);
    print_packet(packet, config);
	char *payload = default_payload(config->packet_size);
	strncpy(&packet[DATA], payload, config->packet_size);
	free(payload);
    
	(void)identifier;
	(void)sequence;
    return packet;
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

    // print_configuration(&config);

    run(&config);
    free(config.dst_addr);

	print_statistics();

	return 0;
}


