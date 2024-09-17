#include <ft_ping.h>

void    resolve_address(t_config *config, struct sockaddr_in *dst_addr)
{
    struct addrinfo     *addrinfo = NULL;
    char                addr_buf[INET_ADDRSTRLEN];
    struct addrinfo     hint;

    memset(addr_buf, 0, sizeof(addr_buf));
    memset(&hint, 0, sizeof(hint));

    hint.ai_family = AF_INET;

    if (!strncmp(config->dst_addr, "localhost", 9))
    {
        free(config->dst_addr);
        config->dst_addr = malloc(sizeof(char) * 16);
        if (!config->dst_addr)
        {
            fatal("Malloc error\n");
        }
        strncpy(config->dst_addr, "127.0.0.1", 16);
    }

	if (getaddrinfo(config->dst_addr, NULL, &hint, &addrinfo) != 0)
	{
		error(EXIT_FAILURE, 0, "unknown host");
    }

    inet_ntop(AF_INET, &((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr, addr_buf, sizeof(addr_buf));

    char *addr = malloc(sizeof(char) * (strlen(addr_buf) + 1));
    if (!addr)
    {
        fatal("Malloc error\n");
    }

    strncpy(addr, addr_buf, strlen(addr_buf) + 1);

    freeaddrinfo(addrinfo);

    dst_addr->sin_family = AF_INET;
    inet_aton(addr, &dst_addr->sin_addr);

    free(addr);
}
