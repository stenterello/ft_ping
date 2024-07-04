#include "ft_ping.h"

double	calculate_interval(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000 + t2->tv_usec / 1000) - (t1->tv_sec * 1000 + t1->tv_usec / 1000));
}

double	calculate_interval_micro(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000000 + t2->tv_usec) - (t1->tv_sec * 1000000 + t1->tv_usec));
}

void	print_statistics(const t_config *config, const t_stats *stats)
{
	printf("\n--- %s ping statistics ---\n", config->dst_addr);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n", 0, 0, 0);
	printf("round-trip min/avg/max/stddev = %0.3f/%0.3f/%0.3f/%0.3f ms", 0.0, 0.0, 0.0, 0.0);
	(void)stats;
}

char	*get_latency(struct timeval *last, struct timeval *latency)
{
	// printf("last value is: \t\t%ld\n", last->tv_sec * 1000000 + last->tv_usec);
	// printf("latency value is: \t%ld\n", latency->tv_sec * 1000000 + latency->tv_usec);
	double ms = calculate_interval_micro(last, latency);

	// printf("interval is: %f\n", ms);

	char *ret = malloc(sizeof(char) * 6);
	if (!ret)
	{
		fatal("Malloc error\n");
	}

	sprintf(ret, "%0.3f", ms / 1000);

	// printf("microsecs are %s\n", ret);

	return ret;
}