#ifndef FT_PING_H
#define FT_PING_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <error.h>
#include <time.h>

#define _XOPEN_SOURCE		700

#define ICMP_HEADER_SIZE	8
#define DEFAULT_INTERVAL	1000

/**
 * More flags to do:
 * 	-D	print timestamp
 * 	-[custom flag] print timestamp in human readable format
 */

/*
 * @brief This struct contains all the configuration for the ping command.
 */
typedef struct	s_config
{
	int			verbose;
	long long	count;
	int			flood;
	int			preload;
	int			no_dns;
	int			deadline;
	int			timeout;
	char		*pattern;
	int			bypass_routing;
	int			packet_size;
	int			timestamp_flag;
	int			ttl;
	int			ip_timestamp;
	char		*dst_addr;
}				t_config;

typedef struct	s_time_record
{
	unsigned long			time;
	struct s_time_record	*next;
}				t_time_record;

typedef struct s_stats
{
	int				tx_num;
	int				rx_num;
	unsigned long	min;
	unsigned long	max;
	unsigned long	avg;
    unsigned long   stddev;
	t_time_record	*time_records;
}				t_stats;

/**
 * ICMP packet type section enum
 */
enum Type
{
	ECHO_REPLY = 0,
	DESTINATION_UNREACHABLE = 3,
	SOURCE_QUENCH,
	REDIRECT,
	ECHO_REQUEST = 8,
	ROUTER_ADVISEMENT,
	ROUTER_SELECTION,
	TIME_EXCEEDED,
	PARAMETER_PROBLEM,
	TIMESTAMP,
	TIMESTAMP_REPLY,
	INFORMATION_REQUEST,
	INFORMATION_REPLY,
	ADDRESS_MASK_REQUEST,
	ADDRESS_MASK_REPLY,
	TRACEROUTE
};

enum Sections
{
	TYPE = 0,
	CODE,
	CHECKSUM,
	ID = 4,
	SEQ = 6,
	DATA = 8,
	MAX_VALUE
};

extern int			about_to_quit;

void 				apply_configuration(int argc, char **argv, t_config *config);
void 				fatal(const char *error);
void				handle_sigint();
t_config		    default_config();
char 				*craft_packet(const t_config *config);
void				run(t_config *config);
void				resolve_address(t_config *config, struct sockaddr_in *dst_addr);
int					open_socket();
void				send_ping(int socket, struct sockaddr_in *dst_addr,
								const char *buffer, struct timeval *last,
								const t_config *config);
void				read_reply(int socket, fd_set *set, struct timeval *last, struct in_addr *dst_addr, t_stats *stats, t_config *config);
void				print_received_info(char *res, int len, char *latency_string, struct in_addr *dst_addr);
double				calculate_interval(const struct timeval *t1, const struct timeval *t2);
double				calculate_interval_micro(const struct timeval *t1, const struct timeval *t2);
void				print_statistics(t_config *config, t_stats *stats);
char				*get_latency(struct timeval *last, struct timeval *latency);
unsigned long		convert_to_microsec(struct timeval *time);


#endif // FT_PING_H
