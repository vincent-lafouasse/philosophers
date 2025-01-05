#include "t_config.h"

#include <stdio.h>

t_config load_config(int ac, char* av[]) {
    (void)ac;
    (void)av;

    u32 time_to_sleep_ms = 400;
    u32 time_to_eat_ms = 100;
    u32 time_to_die_ms = 1000;

    return (t_config){
        .n_philosophers = 3,
        .time_to_sleep_us = 1000 * time_to_sleep_ms,
        .time_to_eat_us = 1000 * time_to_eat_ms,
        .time_to_die_us = 1000 * time_to_die_ms,
    };
}

void log_config(const t_config* cfg) {
    if (!cfg)
        return;

    printf("config:\n");
    printf("%u diners\n", cfg->n_philosophers);
    printf("nap length %u ms\n", cfg->time_to_sleep_us / 1000);
    printf("dinner length %u ms\n", cfg->time_to_eat_us / 1000);
    printf("life expectancy %u ms\n", cfg->time_to_die_us / 1000);
    printf("\n");
}
