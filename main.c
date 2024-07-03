#include "ft_ping.h"

// void    print_configuration(t_config *config)
// {
//     printf("---- CONFIGURATION ----\n");
//     printf("verbose: %d\n", config->verbose);
//     printf("flood: %d\n", config->flood);
//     printf("preload: %d\n", config->preload);
//     printf("no_dns: %d\n", config->no_dns);
//     printf("deadline: %d\n", config->deadline);
//     printf("timeout: %d\n", config->timeout);
//     printf("pattern: %s\n", config->pattern);
//     printf("bypass_routing: %d\n", config->bypass_routing);
//     printf("packet_size: %d\n", config->packet_size);
//     printf("timestamp_flag: %d\n", config->timestamp_flag);
//     printf("ttl: %d\n", config->ttl);
//     printf("ip_timestamp: %d\n", config->ip_timestamp);
//     printf("\n\n");
// }

int main(int argc, char **argv)
{
	handle_sigint();
	
    t_config	config = default_config();

	// apply command line configurations
	apply_configuration(argc, argv, &config);

    // print_configuration(&config);

    run(&config);
    free(config.dst_addr);

	return 0;
}


