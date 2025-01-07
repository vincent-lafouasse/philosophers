#include "t_config.h"

#include <stdio.h>
#include <stdlib.h>

t_config debug_config(void) {
    u32 n_philosophers = 3;
    u32 time_to_die_ms = 4000;
    u32 time_to_eat_ms = 1000;
    u32 time_to_sleep_ms = 1000;

    return (t_config){
        .n_philosophers = n_philosophers,
        .time_to_die_us = 1000 * time_to_die_ms,
        .time_to_eat_us = 1000 * time_to_eat_ms,
        .time_to_sleep_us = 1000 * time_to_sleep_ms,
    };
}


t_config load_config(int ac, char* av[]) {
    if (ac == 1)
        return debug_config();

    u32 n_philosophers = atoi(av[1]);
    u32 time_to_die_ms = atoi(av[2]);
    u32 time_to_eat_ms = atoi(av[3]);
    u32 time_to_sleep_ms = atoi(av[4]);

    return (t_config){
        .n_philosophers = n_philosophers,
        .time_to_die_us = 1000 * time_to_die_ms,
        .time_to_eat_us = 1000 * time_to_eat_ms,
        .time_to_sleep_us = 1000 * time_to_sleep_ms,
    };
}

void log_config(t_config cfg) {
    printf("config:\n");
    printf("%u diners\n", cfg.n_philosophers);
    printf("life expectancy %u ms\n", cfg.time_to_die_us / 1000);
    printf("dinner length %u ms\n", cfg.time_to_eat_us / 1000);
    printf("nap length %u ms\n", cfg.time_to_sleep_us / 1000);
    printf("\n");
}
