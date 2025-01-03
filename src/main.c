#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef uint32_t u32;

void sleep_ms(u32 ms);

typedef struct s_config t_config;
struct s_config
{
    u32 n_philosophers;
    u32 time_to_sleep_us;
};

t_config load_config(int ac, char* av[])
{
    (void)ac;
    (void)av;

    return (t_config){.n_philosophers = 3, .time_to_sleep_us = 1000000};
}

typedef struct s_state t_state;
struct s_state
{
    pthread_t* philosophers;
    // mutex* forks ?
    const t_config cfg;
};

t_state init(t_config cfg)
{
    return (t_state){.philosophers = malloc(cfg.n_philosophers), .cfg = cfg};
}

int main(int ac, char* av[])
{
    t_config cfg = load_config(ac, av);
    t_state state = init(cfg);
}

void sleep_ms(u32 ms)
{
    usleep(1000 * ms);
}
