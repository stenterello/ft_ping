#include "ft_ping.h"

void	fatal(const char *error)
{
	write(2, error, strlen(error));
	exit(1);
}

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

