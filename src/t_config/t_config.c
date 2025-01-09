#include "t_config.h"

#include <stdio.h>
#include <stdlib.h>
#include "t_error.h"

t_error checked_atou(const char* s, u32* out);

t_config load_config(int ac, char* av[]) {
    //if (ac != 5 && ac != 6)

    u32 n_philosophers = atoi(av[1]);
    u32 time_to_die_ms = atoi(av[2]);
    u32 time_to_eat_ms = atoi(av[3]);
    u32 time_to_sleep_ms = atoi(av[4]);

    u32 n_meals = (ac == 6) ? atoi(av[5]) : 0;

    return (t_config){
        .n_philosophers = n_philosophers,
        .time_to_die_us = 1000 * time_to_die_ms,
        .time_to_eat_us = 1000 * time_to_eat_ms,
        .time_to_sleep_us = 1000 * time_to_sleep_ms,
        .n_meals = n_meals,
    };
}

void log_config(t_config cfg) {
    printf("config:\n");
    printf("%u diners\n", cfg.n_philosophers);
    printf("life expectancy %u ms\n", cfg.time_to_die_us / 1000);
    printf("dinner length %u ms\n", cfg.time_to_eat_us / 1000);
    printf("nap length %u ms\n", cfg.time_to_sleep_us / 1000);
    if (cfg.n_meals)
        printf("n meals %u\n", cfg.n_meals);
    printf("\n");
}
