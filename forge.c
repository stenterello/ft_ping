#include "ft_ping.h"

struct icmp_packet  default_packet()
{
    struct icmp_packet ret;

    ret.type = ECHO_REQUEST;
    ret.code = BAD_SPI;
    ret.checksum = 0;
    ret.identifier = 0;
    ret.sequence_id = 1;
    ret.pointer = 0;
    ret.payload = malloc(sizeof(char) * 67);
    if (!ret.payload)
    {
        fatal("Malloc error\n");
    }

    for (int i = 0; i < 64; i++)
    {
        ret.payload[i + 3] = i;
    }

    return ret;
}