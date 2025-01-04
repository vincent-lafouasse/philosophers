#include "t_config.h"

t_config load_config(int ac, char* av[])
{
    (void)ac;
    (void)av;

    return (t_config){.n_philosophers = 3, .time_to_sleep_us = 1000000};
}
