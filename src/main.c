#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ft_time.h"
#include "t_config.h"
#include "t_message_queue.h"
#include "t_philosopher.h"

void sleep_ms(u32 ms);

typedef struct {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue* messages;
    t_instant simulation_start;
    t_config cfg;
} t_state;

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

    t_message** last_meals = malloc(cfg.n_philosophers * sizeof(t_message*));

    while (1) {
        for (u32 i = 0; i < cfg.n_philosophers; i++) {
            t_instant last_meal = last_meals[i] ? last_meals[i]->timestamp
                                                : state.simulation_start;
            if (1000 * duration_since(&last_meal).milliseconds >
                cfg.time_to_die_us) {
                printf(
                    "%06u %u HAS NOT EATEN SINCE %06u AND FUCKING DIED\n",
                    duration_since(&state.simulation_start).milliseconds, i + 1,
                    duration_since(&last_meal).milliseconds -
                        duration_since(&state.simulation_start).milliseconds);
                exit(1);
            }
        }
        if (state.messages->head) {
            t_message* message = mq_pop(state.messages);
            log_message(message, state.simulation_start);
            if (message->state == EATING || message->state == SLEEPING) {
                free(last_meals[message->index]);
                last_meals[message->index] = message;
            } else {
                free(message);
            }
        } else {
            sleep_ms(1);
        }
    }

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
