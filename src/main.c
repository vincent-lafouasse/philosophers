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

static t_error init(t_config cfg, t_state* state);
static t_error run(t_state* state);
static void cleanup(t_state* state);

int main(int ac, char* av[]) {
    t_config cfg;
    t_error err;

    err = load_config(ac, av, &cfg);
    if (err != NO_ERROR) {
        printf("Failed to load config, %s\n", error_repr(err));
        exit(1);
    }
    log_config(cfg);

    t_state state;
    err = init(cfg, &state);
    run(&state);
    track_progress(&state);
    cleanup(&state);
}

static t_error init(t_config cfg, t_state* state) {
    *state =
        (t_state){.philosophers =
                      malloc(cfg.n_philosophers * sizeof(*state->philosophers)),
                  .forks = malloc(cfg.n_philosophers * sizeof(*state->forks)),
                  .messages = malloc(sizeof(*state->messages)),
                  .simulation_start = instant_now(),
                  .cfg = cfg};
    if (!state->philosophers || !state->forks || !state->messages) {
        free(state->philosophers);
        free(state->forks);
        free(state->messages);
        return E_OOM;
    }
    *state->messages = mq_new();

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(state->forks + i, NULL);
    }

    for (u32 i = 0; i < cfg.n_philosophers; i++)
        state->philosophers[i] =
            philosopher_new(i, state->forks, state->messages, cfg);

    return NO_ERROR;
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
