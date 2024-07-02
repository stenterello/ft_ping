#include "ft_ping.h"

void	fatal(const char *error)
{
	write(2, error, strlen(error));
	exit(1);
}

// void	print_packet(struct icmp_packet *packet)
// {
// 	char* ptr = (char*)packet;
// 	for (int i = 0; i < (int)sizeof(packet); i++)
// 	{
// 		if (i % 8 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("|%02x|", ptr[i]);
// 	}
// 	for (int i = 0; i < 67; i++)
// 	{
// 		if (i % 8 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("|%02x|", packet->payload[i]);
// 	}
// 	printf("\n");
// }

void	handler()
{
	about_to_quit = 1;
}

void	handle_sigint()
{
	struct sigaction	act = {
		.sa_handler = handler
	};

	int	ret = sigaction(SIGINT, &act, NULL);

	if (ret < 0)
	{
		perror(strerror(errno));
	}
}