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

void	send_ping(int socket, struct sockaddr_in *dst_addr,
			const char *buffer, struct timeval *last,
			const t_config *config)
{
	socklen_t	size = sizeof(*dst_addr);

	gettimeofday(last, NULL);
	
	int bytes = sendto(socket, buffer, config->packet_size + ICMP_HEADER_SIZE, 0, (struct sockaddr*)dst_addr, size);
	if (bytes < 0)
	{
		perror("sendto");
	}
}

void    print_received_info(char *res, int len, char *latency_string, struct in_addr *addr)
{
    char    addr_string[INET_ADDRSTRLEN];

    memset(addr_string, 0, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, addr, addr_string, INET_ADDRSTRLEN);
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%s ms\n", len, addr_string, res[SEQ + 1], 1, latency_string);
}

void	read_reply(int socket, fd_set *set, struct timeval *last, struct in_addr *dst_addr)
{
	struct timeval      latency;
	char				read_buffer[4096];

	memset(&latency, 0, sizeof(latency));
	memset(read_buffer, 0, 4096);

	// TODO: add timeout to select
	int r = select(socket + 1, set, NULL, NULL, NULL);
	if (r == -1)
	{
		perror("select");
		fatal("");
	}
	else if (r)
	{
		int readbytes = recv(socket, read_buffer, 4096, 0);
		if (readbytes < 0)
		{
			perror("recv");
		}
		gettimeofday(&latency, NULL);
		char *latency_string = get_latency(last, &latency);
		print_received_info(read_buffer, readbytes, latency_string, dst_addr);
		free(latency_string);
		memset(read_buffer, 0, 4096);
	}
}