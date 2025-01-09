#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ft_time.h"
#include "t_config/t_config.h"
#include "t_message_queue.h"
#include "t_philosopher.h"
#include "t_state.h"
#include "track/track.h"

static t_state init(t_config cfg);
static t_error run(t_state* state);
static void cleanup(t_state* state);

int main(int ac, char* av[]) {
    t_config cfg;
    t_error err;

    err = load_config(ac, av, &cfg);
    if (err != NO_ERROR) {
        printf("Failed to load config, error %s\n", "DUMMY");
        exit(1);
    }
    log_config(cfg);

    t_state state = init(cfg);
    run(&state);
    track_progress(&state);
    cleanup(&state);
}

static t_state init(t_config cfg) {
    t_state out;

    out = (t_state){
        .philosophers = malloc(cfg.n_philosophers * sizeof(*out.philosophers)),
        .forks = malloc(cfg.n_philosophers * sizeof(*out.forks)),
        .messages = malloc(sizeof(*out.messages)),
        .simulation_start = instant_now(),
        .cfg = cfg};
    if (!out.philosophers || !out.forks || !out.messages) {
        free(out.philosophers);
        free(out.forks);
        return (t_state){0};
    }
    *out.messages = mq_new();

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(out.forks + i, NULL);
    }

    for (u32 i = 0; i < cfg.n_philosophers; i++)
        out.philosophers[i] = philosopher_new(i, out.forks, out.messages, cfg);

    return out;
}

static t_error run(t_state* state) {
    for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
        philosopher_start(state->philosophers + i);  // fallible
    }

    for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
        pthread_detach(state->philosophers[i].thread);
    }
    return NO_ERROR;
}

static void cleanup(t_state* state) {
    (void)state;
}
