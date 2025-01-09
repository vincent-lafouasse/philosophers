#include "t_config.h"

#include <stdio.h>
#include <stdlib.h>

t_error checked_atou(const char* s, u32* out);

t_error load_config(int ac, char* av[], t_config* cfg) {
    t_error err;

    if (ac != 5 && ac != 6)
        return E_BADUSAGE;
    err = checked_atou(av[1], &cfg->n_philosophers);
    if (err != NO_ERROR)
        return err;
    err = checked_atou(av[2], &cfg->time_to_die_us);
    if (err != NO_ERROR)
        return err;
    err = checked_atou(av[3], &cfg->time_to_eat_us);
    if (err != NO_ERROR)
        return err;
    err = checked_atou(av[4], &cfg->time_to_sleep_us);
    if (err != NO_ERROR)
        return err;
    if (ac == 6) {
        err = checked_atou(av[5], &cfg->n_meals);
        if (err != NO_ERROR)
            return err;
        cfg->track_meals = true;
    } else {
        cfg->track_meals = false;
    }
    return NO_ERROR;
}

void log_config(t_config cfg) {
    printf("config:\n");
    printf("%u diners\n", cfg.n_philosophers);
    printf("life expectancy %u ms\n", cfg.time_to_die_us / 1000);
    printf("dinner length %u ms\n", cfg.time_to_eat_us / 1000);
    printf("nap length %u ms\n", cfg.time_to_sleep_us / 1000);
    if (cfg.track_meals)
        printf("n meals %u\n", cfg.n_meals);
    printf("\n");
}
