#include "ft_ping.h"
#include <argp.h>
#include <limits.h>
#include <ctype.h>

// FLAGS
#define VERBOSE_FLAG		'v'
#define FLOOD_FLAG			'f'
#define PRELOAD_FLAG		'l'
#define NO_DNS_FLAG			'n'
#define DEADLINE_FLAG		'w'
#define TIMEOUT_FLAG		'W'
#define PATTERN_FLAG		'p'
#define NO_ROUTING_FLAG		'r'
#define PACKET_SIZE_FLAG	's'
#define TIMESTAMP_FLAG		'T'
#define TTL_FLAG			1
#define IP_TIMESTAMP_FLAG	2

#define OPT_STRING			"vfl:nw:W:p:rs:T:"

#define MAX_PATTERN			16;

/**
 * Flags enum
 */
enum Flags {
	VERBOSE = 0,
	FLOOD,
	PRELOAD,
	NO_DNS,
	DEADLINE,
	TIMEOUT,
	PATTERN,
	NO_ROUTING,
	PACKET_SIZE,
	TIMESTAMP_KEY,
	TTL,
	IP_TIMESTAMP,
	FLAGS_MAX_VALUE
};

static int only_digits(const char* str)
{
	for (int i = 0; str[i]; i++)
	{
		if (!isdigit(str[i]) && str[i] != '-')
		{
			return 0;
		}
	}
	return 1;
}

static void		validate_arg(const char* arg, int key)
{
	char error_string[300];
	const long long value = atoll(arg);

	memset(error_string, 0, sizeof(error_string));

	if (errno)
	{
		sprintf(error_string, "ft_ping: invalid argument: '%s': %s\n", arg, strerror(errno));
		fatal(error_string);
	}

	switch (key)
	{
		case PRELOAD_FLAG:
		{
			if (value < 1 || value > SHRT_MAX)
			{
				sprintf(error_string,
						"ft_ping: invalid argument: '%s': out of range 1 <= value <= 65536\n", arg);
				fatal(error_string);
			}
			else if (value > 3)
			{
				sprintf(error_string,
						"ft_ping: cannot set preload to value greater than 3: %s\n", arg);
				fatal(error_string);
			}
			break;
		}
		case DEADLINE_FLAG:
		{
			if (value < 0 || value > 2147483647)
			{
				sprintf(error_string,
						"ft_ping: invalid argument: '%s': out of range: 0 <= value <= 2147483647\n", arg);
				fatal(error_string);	
			}
			break;
		}
		case TIMEOUT_FLAG:
		{
			if (value < 0 || value > INT_MAX / 1000)
			{
				sprintf(error_string, "ft_ping: bad linger time: %s\n", arg);
				fatal(error_string);
			}
			break;
		}
		case PACKET_SIZE_FLAG:
		{
			if (!only_digits(optarg))
			{
				sprintf(error_string, "ft_ping: invalid argument: '%s'\n", arg);
				fatal(error_string);
			}
			if (value < 0 || value > 2147483647)
			{
				sprintf(error_string,
						"ft_ping: invalid argument: '%s': out of range 0 <= value <= 2147483647\n", arg);
				fatal(error_string);
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

static error_t	parser_function(int key, char *arg, struct argp_state *state)
{
	t_config	*config = state->input;
	
	switch (key)
	{
		case VERBOSE_FLAG:
		{
			config->verbose = 1;
			break;
		}
		case FLOOD_FLAG:
		{
			config->flood = 1;
			break;
		}
		case PRELOAD_FLAG:
		{
			validate_arg(arg, PRELOAD_FLAG);
			config->preload = atoi(arg);
			break;
		}
		case NO_DNS_FLAG:
		{
			config->no_dns = 1;
			break;
		}
		case DEADLINE_FLAG:
		{
			validate_arg(arg, DEADLINE_FLAG);
			config->deadline = atoi(arg);
			break;
		}
		case TIMEOUT_FLAG:
		{
			validate_arg(arg, TIMEOUT_FLAG);
			// add to config
			break;
		}
		case PATTERN_FLAG:
		{
			validate_arg(arg, PATTERN_FLAG);
			// add to config;
			break;
		}
		case NO_ROUTING_FLAG:
		{
			config->bypass_routing = 1;
			break;
		}
		case PACKET_SIZE_FLAG:
		{
			validate_arg(arg, PACKET_SIZE_FLAG);
			// add to config
			break;
		}
		case TIMESTAMP_FLAG:
		{
			validate_arg(arg, TIMESTAMP_FLAG);
			// add to config
			break;
		}
		case TTL_FLAG:
		{
			validate_arg(arg, TTL_FLAG);
			// add to config
			break;
		}
		case IP_TIMESTAMP_FLAG:
		{
			validate_arg(arg, IP_TIMESTAMP_FLAG);
			// add to config
			break;
		}
		case ARGP_KEY_END:
		{
			if (config->dst_addr == NULL)
			{
				fatal("ping: usage error: Destination address required\n");
			}
			break;
		}
		default:
		{
			if (!arg)
			{
				break;
			}
			if (config->dst_addr)
			{
				free(config->dst_addr);
			}
			config->dst_addr = malloc(sizeof(char) * (strlen(arg) + 1));
			if (!config->dst_addr)
			{
				fatal("Malloc error\n");
			}
			strncpy(config->dst_addr, arg, strlen(arg));
			break;
		}
	}

	return 0;
}

static void	define_flag(enum Flags flag_value, struct argp_option *flag)
{
	switch (flag_value)
	{
		case VERBOSE:
		{
			flag->name = NULL;
			flag->key = 'v';
			flag->arg = NULL;
			flag->flags = 0;
			flag->doc = "Verbose output";
			flag->group = 0;
			break;
		}
		case FLOOD:
		{
			flag->name = NULL;
			flag->key = 'f';
			flag->arg = NULL;
			flag->flags = 0;
			flag->doc = "Flood ping";
			flag->group = 0;
			break;
		}
		case PRELOAD:
		{
			flag->name = NULL;
			flag->key = 'l';
			flag->arg = "preload";
			flag->flags = 0;
			flag->doc = "Send <preload> number of packages while waiting replies\n";
			flag->group = 0;
			break;
		}
		case NO_DNS:
		{
			flag->name = NULL;
			flag->key = 'n';
			flag->arg = NULL;
			flag->flags = 0;
			flag->doc = "Do not resolve host addresses\n";
			flag->group = 0;
			break;
		}
		case DEADLINE:
		{
			flag->name = NULL;
			flag->key = 'w';
			flag->arg = "deadline";
			flag->flags = 0;
			flag->doc = "Reply wait <deadline> in seconds\n";
			flag->group = 0;
			break;
		}
		case TIMEOUT:
		{
			flag->name = NULL;
			flag->key = 'W';
			flag->arg = "timeout";
			flag->flags = 0;
			flag->doc = "Time to wait for response\n";
			flag->group = 0;
			break;
		}
		case PATTERN:
		{
			flag->name = NULL;
			flag->key = 'p';
			flag->arg = "pattern";
			flag->flags = 0;
			flag->doc = "Contents of padding byte\n";
			flag->group = 0;
			break;
		}
		case NO_ROUTING:
		{
			flag->name = NULL;
			flag->key = 'r';
			flag->arg = NULL;
			flag->flags = 0;
			flag->doc = "Bypass normal routing table\n";
			flag->group = 0;
			break;
		}
		case PACKET_SIZE:
		{
			flag->name = NULL;
			flag->key = 's';
			flag->arg = "size";
			flag->flags = 0;
			flag->doc = "Use <size> as number of data bytes to be sent\n";
			flag->group = 0;
			break;
		}
		case TIMESTAMP_KEY:
		{
			flag->name = NULL;
			flag->key = 'T';
			flag->arg = "timestamp";
			flag->flags = 0;
			flag->doc = "Define timestamp, can be one of <tsonly|tsandaddr|tsprespec>\n";
			flag->group = 0;
			break;
		}
		case TTL:
		{
			flag->name = "ttl";
			flag->key = 1;
			flag->arg = "time";
			flag->flags = 0;
			flag->doc = "Specify <time> as a time-to-live\n";
			flag->group = 0;
			break;
		}
		case IP_TIMESTAMP:
		{
			flag->name = "ip-timestamp";
			flag->key = 2;
			flag->arg = "FLAG";
			flag->flags = 0;
			flag->doc = "IP timestamp of type FLAG, which is one of \"tsonly\" and \"tsaddr\"\n";
			flag->group = 0;
			break;
		}
		case FLAGS_MAX_VALUE:
		{
			flag->name = 0;
			flag->key = 0;
			flag->arg = 0;
			flag->flags = 0;
			flag->doc = 0;
			flag->group = 0;
			break;
		}
		default:
		{
			break;
		}
	}
}

void	apply_configuration(int argc, char **argv, t_config *config)
{
	struct argp_option flags[FLAGS_MAX_VALUE + 1];

	for (int i = 0; i <= FLAGS_MAX_VALUE; i++)
	{
		define_flag(i, &flags[i]);
	}

	struct argp	parser = {
		.options = flags,
		.parser = &parser_function,
		.args_doc = "<destination>",
		.doc = "\nOptions:",
		.children = NULL,
		.help_filter = NULL,
		.argp_domain = NULL
	};

	error_t res = argp_parse(&parser, argc, argv, 0, NULL, config);
	if (res)
	{
		fatal("add help print");
	}
}
