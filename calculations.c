#include "ft_ping.h"

double	calculate_interval(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000 + t2->tv_usec / 1000) - (t1->tv_sec * 1000 + t1->tv_usec / 1000));
}

double	calculate_interval_micro(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000000 + t2->tv_usec) - (t1->tv_sec * 1000000 + t1->tv_usec));
}

void	calculate_rtt_statistics(t_stats *stats)
{
	if (stats->time_records)
	{
		stats->min = stats->time_records->time;
		stats->max = stats->time_records->time;
		stats->avg = stats->time_records->time;
	}

	unsigned long long total = 0;
	unsigned long long counter = 0;

	for (t_time_record *ptr = stats->time_records; ptr; ptr = ptr->next)
	{
		counter++;
		if (ptr->time < stats->min)
		{
			stats->min = ptr->time;
		}
		if (ptr->time > stats->max)
		{
			stats->max = ptr->time;
		}

		total += ptr->time;
	}

	stats->avg = total / counter;
}

void	print_statistics(const t_config *config, t_stats *stats)
{
	printf("--- %s ping statistics ---\n", config->dst_addr);
	printf("%d packets transmitted, %d packets received, %0.3f%% packet loss\n", stats->tx_num, stats->rx_num, ((float)stats->tx_num / (float)stats->rx_num));
	calculate_rtt_statistics(stats);
	printf("round-trip min/avg/max/stddev = %.3ld/%.3ld/%.3ld/%.3f ms", stats->min, stats->avg, stats->max, 0.0);
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

unsigned long	convert_to_microsec(struct timeval *time)
{
	return (time->tv_sec * 100000 + time->tv_usec);
}