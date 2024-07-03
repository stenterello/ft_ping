#include "ft_ping.h"

int     open_socket()
{
    struct sockaddr_in  local_addr;
    memset(&local_addr, 0, sizeof(local_addr));

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

    return sock;
}