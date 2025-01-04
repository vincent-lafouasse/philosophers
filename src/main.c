#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "t_config.h"
#include "t_philosopher.h"

typedef uint32_t u32;

void sleep_ms(u32 ms);

typedef struct
{
    t_philosopher* philosophers;
    // mutex* forks ?
    const t_config* cfg;
} t_state;

t_state init(const t_config* cfg)
{
    t_state out;

    out = (t_state){
        .philosophers = malloc(cfg->n_philosophers * sizeof(t_philosopher)),
        .cfg = cfg};
    if (!out.philosophers)
        return (t_state){0};

    for (u32 i = 0; i < out.cfg->n_philosophers; i++)
        out.philosophers[i] = philosopher_new(i, cfg);

    return out;
}

t_error start(t_state* state) {
    t_error err;

    for (u32 i = 0; i < state->cfg->n_philosophers; i++)
    {
        err = philosopher_start(state->philosophers + i);
        if (err != NO_ERROR) {
            return err; // should probably cleanup also
        }
    }

    for (u32 i = 0; i < state->cfg->n_philosophers; i++) {
        pthread_join(*state->philosophers[i].thread, NULL);
    }

    return NO_ERROR;
}

int main(int ac, char* av[])
{
    const t_config cfg = load_config(ac, av);
    log_config(&cfg);

    t_state state = init(&cfg);
    start(&state);
}

void sleep_ms(u32 ms)
{
    usleep(1000 * ms);
}
