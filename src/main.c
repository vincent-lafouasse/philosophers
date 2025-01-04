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

typedef struct {
    pthread_t* thread;
    u32 index;
    const t_config* cfg;
    // mutex* left_fork;
    // mutex* right_fork;
} Philosopher;

typedef struct
{
    Philosopher* philosophers;
    // mutex* forks ?
    const t_config* cfg;
} t_state;

t_state init(const t_config* cfg)
{
    t_state out;

    out = (t_state){.philosophers = malloc(cfg->n_philosophers * sizeof(Philosopher)), .cfg = cfg};
    if (!out.philosophers)
        return (t_state){0};

    for (u32 i = 0; i < out.cfg->n_philosophers; i++) {
        out.philosophers[i] = (Philosopher){.thread=NULL, .index = i, .cfg = cfg};
    }

    return out;
}

int main(int ac, char* av[])
{
    t_config cfg = load_config(ac, av);
    t_state state = init(&cfg);
}

void sleep_ms(u32 ms)
{
    usleep(1000 * ms);
}
