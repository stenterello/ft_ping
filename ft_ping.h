#ifndef FT_PING_H
#define FT_PING_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define _XOPEN_SOURCE 700

/**
 * More flags to do:
 * 	-D	print timestamp
 * 	-[custom flag] print timestamp in human readable format
 */

/*
 * @brief This struct contains all the configuration for the ping command.
 */
typedef struct	s_options
{
	int		verbose;
	int		flood;
	int		has_preload;
	int		preload;
	int		no_dns;
	int		has_deadline;
	int		deadline;
	int		has_timeout;
	int		timeout;
	int		has_pattern;
	char	*pattern;
	int		bypass_routing;
	int		has_packet_size;
	int		packet_size;
	int		has_timestamp;
	int		timestamp_flag;
	int		has_ttl;
	int		ttl;
	int		has_ip_timestamp;
	int		ip_timestamp;
}				t_options;

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

/**
 * ICMP packet code section enum
 */
enum Code
{
	BAD_SPI = 0,
	AUTHENTICATION_FAILED,
	DECOMPRESSION_FAILED,
	DECRYPTION_FAILED,
	NEED_AUTHENTICATION,
	NEED_AUTHORIZATION
};

/**
 * This struct is used to create the ICMP packet.
 */
struct icmp_packet
{
	char	type;
	char	code;
	short	checksum;
	char	identifier;
	char	sequence_id;
	short	pointer;
	char	*payload;
	size_t	size;
};

struct s_info
{
	struct icmp_packet	*packet;
	t_options			*options;
};


extern int			about_to_quit;

void 				apply_configuration(int argc, char **argv, struct s_info *info);
void 				fatal(const char *error);
struct icmp_packet	default_packet();
void				print_packet(struct icmp_packet *packet);
void				handle_sigint();
void				infinite(struct icmp_packet *packet, t_options *options);

#endif // FT_PING_H
