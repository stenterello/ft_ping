# ft_ping

This project aim is to replicate some of the behaviours of ping command (based on inetutils-2.0).
Currently, I'm interested in handle this set of options:

- -f                         Flood ping
        --ip-timestamp=FLAG    IP timestamp of type FLAG, which is one of
                               "tsonly" and "tsaddr"

- -l preload                 Send <preload> number of packages while waiting
                               replies

- -n                         Do not resolve host addresses

- -p pattern                 Contents of padding byte

- -r                         Bypass normal routing table

- -s size                    Use <size> as number of data bytes to be sent

        --ttl=time             Specify <time> as a time-to-live

- -T timestamp               Define timestamp, can be one of
                               <tsonly|tsandaddr|tsprespec>

- -v                         Verbose output
- -w deadline                Reply wait <deadline> in seconds

- -W timeout                 Time to wait for response

- -?, --help                 Give this help list
      --usage                Give a short usage message

Actually supporting:
- -l {*preload*}


Next *todos*:

- craft checksum before sending packet;
- check identifier and sequence correctness;
- handle flood with -f option { too many packets are sent compared to ping? };
- fix packet loss percentage in print_statistics;
