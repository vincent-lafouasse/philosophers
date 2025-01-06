#include "t_config.h"

#include <stdio.h>

t_config load_config(int ac, char* av[]) {
    (void)ac;
    (void)av;

    u32 n_philosophers = 5;
    u32 time_to_die_ms = 800;
    u32 time_to_eat_ms = 200;
    u32 time_to_sleep_ms = 200;

    return (t_config){
        .n_philosophers = n_philosophers,
        .time_to_sleep_us = 1000 * time_to_sleep_ms,
        .time_to_eat_us = 1000 * time_to_eat_ms,
        .time_to_die_us = 1000 * time_to_die_ms,
    };
}

void log_config(t_config cfg) {
    printf("config:\n");
    printf("%u diners\n", cfg.n_philosophers);
    printf("nap length %u ms\n", cfg.time_to_sleep_us / 1000);
    printf("dinner length %u ms\n", cfg.time_to_eat_us / 1000);
    printf("life expectancy %u ms\n", cfg.time_to_die_us / 1000);
    printf("\n");
}
