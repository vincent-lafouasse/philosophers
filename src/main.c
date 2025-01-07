#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ft_time.h"
#include "t_config.h"
#include "t_message_queue.h"
#include "t_philosopher.h"

void sleep_ms(u32 ms);

typedef struct {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue messages;
    t_config cfg;
} t_state;

void log_state_change(t_philosopher_state new_state,
                      u32 index,
                      t_instant simulation_start) {
    printf("%06u %u", duration_since(&simulation_start).milliseconds,
           index + 1);
    if (new_state == THINKING)
        printf(" is thinking\n");
    if (new_state == FORK_HANDED)
        printf(" is has taken a fork\n");
    if (new_state == EATING)
        printf(" is eating\n");
    if (new_state == SLEEPING)
        printf(" is sleeping\n");
    if (new_state == DEAD)
        printf(" is ded\n");
}

static t_state init(t_config cfg);
static t_error run(t_state* state);
static void cleanup(t_state* state);

void sleep_ms(u32 ms) {
    usleep(ms * 1000);
}

int main(int ac, char* av[]) {
    const t_config cfg = load_config(ac, av);
    log_config(cfg);

    t_state state = init(cfg);
    run(&state);

    t_instant simulation_start = instant_now();
    t_philosopher_state* philo_states =
        malloc(cfg.n_philosophers * sizeof(*philo_states));
    t_instant* timestamps = malloc(cfg.n_philosophers * sizeof(*timestamps));

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        philo_states[i] = THINKING;
        timestamps[i] = simulation_start;
    }

    while (1) {
        for (u32 i = 0; i < cfg.n_philosophers; i++) {
            pthread_mutex_lock(&state.philosophers[i].state_lock);
            t_philosopher_state new_state = state.philosophers[i].state;
            pthread_mutex_unlock(&state.philosophers[i].state_lock);

            if (new_state == philo_states[i])
                continue;
            log_state_change(new_state, i, simulation_start);
            philo_states[i] = new_state;
            timestamps[i] = instant_now();
        }
    }

    cleanup(&state);
}

static t_state init(t_config cfg) {
    t_state out;

    out = (t_state){
        .philosophers = malloc(cfg.n_philosophers * sizeof(*out.philosophers)),
        .forks = malloc(cfg.n_philosophers * sizeof(*out.forks)),
        .messages = mq_new(),
        .cfg = cfg};
    if (!out.philosophers || !out.forks) {
        free(out.philosophers);
        free(out.forks);
        return (t_state){0};
    }

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(out.forks + i, NULL);
    }

    for (u32 i = 0; i < cfg.n_philosophers; i++)
        out.philosophers[i] = philosopher_new(i, out.forks, cfg);

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
