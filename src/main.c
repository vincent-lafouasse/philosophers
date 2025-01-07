#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ft_time.h"
#include "t_config.h"
#include "t_philosopher.h"

typedef struct {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    pthread_mutex_t output_lock;
    t_config cfg;
} t_state;

void log_state_change(t_philosopher_state new_state, u32 index, t_instant simulation_start) {
    printf("%010u %u",
           duration_since(&simulation_start).milliseconds,
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

void* logging_thread_routine(void* arg) {
    t_state* state = (t_state*)arg;
    t_instant start = instant_now();
    t_philosopher_state* philo_states = malloc(state->cfg.n_philosophers * sizeof(*philo_states));
    t_instant* timestamps = malloc(state->cfg.n_philosophers * sizeof(*timestamps));

    for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
        philo_states[i] = THINKING;
        timestamps[i] = start;
    }

    while (1) {
        t_instant frame_start = instant_now();
        for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
            pthread_mutex_lock(&state->philosophers->state_lock);
            t_philosopher_state new_state = state->philosophers->state;
            pthread_mutex_unlock(&state->philosophers->state_lock);

            if (new_state == philo_states[i])
                continue;
            log_state_change(new_state, i, start);
            philo_states[i] = new_state;
        }

        t_duration frame_length = duration_since(&frame_start);
        if (frame_length.milliseconds < 20)
            usleep(20000 - frame_length.milliseconds * 1000);
    }

    return NULL;
}

static t_state init(t_config cfg);
static t_error start(t_state* state);
static void cleanup(t_state* state);

void sleep_ms(u32 ms) {
    usleep(ms * 1000);
}

int main(int ac, char* av[]) {
    const t_config cfg = load_config(ac, av);
    log_config(cfg);

    t_state state = init(cfg);
    start(&state);
    cleanup(&state);

    t_instant start = instant_now();

    for (int i = 0; i < 5; i++) {
        printf("%u ms\n", duration_since(&start).milliseconds);
        if (i % 2)
            sleep_ms(1000);
        else
            sleep_ms(500);
    }
}

static t_state init(t_config cfg) {
    t_state out;

    out = (t_state){
        .philosophers = malloc(cfg.n_philosophers * sizeof(*out.philosophers)),
        .forks = malloc(cfg.n_philosophers * sizeof(*out.forks)),
        .cfg = cfg};
    if (!out.philosophers || !out.forks) {
        free(out.philosophers);
        free(out.forks);
        return (t_state){0};
    }

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(out.forks + i, NULL);
    }
    pthread_mutex_init(&out.output_lock, NULL);

    for (u32 i = 0; i < cfg.n_philosophers; i++)
        out.philosophers[i] =
            philosopher_new(i, out.forks, &out.output_lock, cfg);

    return out;
}

static t_error start(t_state* state) {
    for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
        philosopher_start(state->philosophers + i);  // fallible
    }

    for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
        pthread_join(state->philosophers[i].thread, NULL);  // fallible
    }

    return NO_ERROR;
}

static void cleanup(t_state* state) {
    (void)state;
}
