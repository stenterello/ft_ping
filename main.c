#include "ft_ping.h"

int		about_to_quit = 0;

void	(*decide_strategy(struct icmp_packet *packet, t_options *options))(struct icmp_packet *packet, t_options *options)
{
	(void)packet;
	(void)options;

	return infinite;
}

void	print_statistics()
{
	
}

int main(int argc, char **argv)
{
	handle_sigint();
	
	struct icmp_packet	packet = default_packet();
	t_options			opts;

	struct s_info	info = {
		.packet = &packet,
		.options = &opts
	};

	// apply command line configurations
	apply_configuration(argc, argv, &info);

	print_packet(&packet);

	void	(*strategy)() = decide_strategy(&packet, &opts);

	strategy(&packet, &opts);

	print_statistics();

	return 0;
}
