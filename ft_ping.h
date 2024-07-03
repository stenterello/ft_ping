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

#define _XOPEN_SOURCE		700

#define ICMP_HEADER_SIZE	8

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
	int		verbose;
	int		flood;
	int		preload;
	int		no_dns;
	int		deadline;
	int		timeout;
	char	*pattern;
	int		bypass_routing;
	int		packet_size;
	int		timestamp_flag;
	int		ttl;
	int		ip_timestamp;
	char	*dst_addr;
}				t_config;

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

#endif // FT_PING_H
