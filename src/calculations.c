#include <limits.h>
#include <math.h>
#include <ft_ping.h>
#include <stdio.h>

double	calculate_interval(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000 + t2->tv_usec / 1000) - (t1->tv_sec * 1000 + t1->tv_usec / 1000));
}

double	calculate_interval_micro(const struct timeval *t1, const struct timeval *t2)
{
	return ((t2->tv_sec * 1000000 + t2->tv_usec) - (t1->tv_sec * 1000000 + t1->tv_usec));
}

unsigned long long min_of(t_time_record *records)
{
    unsigned long long ret = ULLONG_MAX;

    t_time_record *ptr = records;
    while (ptr)
    {
        if (ptr->time < ret)
        {
            ret = ptr->time;
        }
        ptr = ptr->next;
    }
    return ret;
}

unsigned long long max_of(t_time_record *records)
{
    unsigned long long ret = 0.0;

    t_time_record *ptr = records;
    while (ptr)
    {
        if (ptr->time > ret)
        {
            ret = ptr->time;
        }
        ptr = ptr->next;
    }
    return ret;

}

unsigned long long list_sum(t_time_record *records)
{
    t_time_record *ptr = records;
    unsigned long long ret = 0;

    while (ptr)
    {
        ret += ptr->time;
        ptr = ptr->next;
    }
    return ret;
}

size_t list_len(t_time_record *records)
{
    t_time_record *ptr = records;
    unsigned long long ret = 0;

    while (ptr)
    {
        ret++;
        ptr = ptr->next;
    }
    return ret;
}

unsigned long long avg_of(t_time_record *records)
{
    return list_sum(records) / list_len(records);
}

unsigned long long stddev(t_time_record *records, unsigned long avg)
{
    unsigned long ret = 0;
    t_time_record *ptr = records;

    while (ptr)
    {
        ret += powl(((long double)ptr->time - avg), 2);
        ptr = ptr->next;
    }
    ret /= list_len(records);
    return sqrt(ret);
}

void	calculate_rtt_statistics(t_stats *stats)
{
	if (stats->time_records)
	{
		stats->min = min_of(stats->time_records);
		stats->max = max_of(stats->time_records);
		stats->avg = avg_of(stats->time_records);
        stats->stddev = stddev(stats->time_records, stats->avg);
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
	printf("--- %s ft_ping statistics ---\n", config->dst_addr);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
        stats->tx_num,
        stats->rx_num,
        (int)((100 * ((float)stats->tx_num - (float)stats->rx_num)) / (float)stats->tx_num)
    );
	calculate_rtt_statistics(stats);
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", (float)stats->min * .001, (float)stats->avg * .001, (float)stats->max * .001, (float)stats->stddev * .001);
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
