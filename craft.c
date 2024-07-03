#include "ft_ping.h"

t_config    default_config()
{
    t_config    ret;

    ret.verbose = 0;
    ret.flood = 0;
    ret.preload = 3;
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

    packet[TYPE] = 8;
    packet[CODE] = 0;
    packet[CHECKSUM] = 0;
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