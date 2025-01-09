#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "t_error.h"

typedef uint32_t u32;

typedef struct s_config t_config;
struct s_config {
    u32 n_philosophers;
    u32 time_to_die_us;
    u32 time_to_eat_us;
    u32 time_to_sleep_us;
    u32 n_meals;
    bool track_meals;
};

t_error load_config(int ac, char* av[], t_config* cfg);
[[maybe_unused]] void log_config(t_config cfg);
