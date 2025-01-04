#pragma once

#include <stdint.h>

typedef uint32_t u32;

typedef struct s_config t_config;
struct s_config
{
    u32 n_philosophers;
    u32 time_to_sleep_us;
};

t_config load_config(int ac, char* av[]);
void log_config(const t_config* cfg);
