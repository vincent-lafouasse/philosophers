#include "track.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_time.h"
#include "t_config/t_config.h"
#include "t_message_queue.h"

typedef uint32_t u32;

typedef struct {
    t_message** last_meals;
    u32* n_meals;
} t_tracker;

static bool everyone_is_full(u32* n_meals, t_config cfg);

t_error track_progress(t_state* state) {
    t_tracker tracker = (t_tracker){
        .last_meals = malloc(state->cfg.n_philosophers * sizeof(t_message*)),
        .n_meals = NULL};
    if (state->cfg.track_meals) {
        tracker.n_meals = malloc(state->cfg.n_philosophers * sizeof(u32));
        memset(tracker.n_meals, 0,
               state->cfg.n_philosophers * sizeof(*tracker.n_meals));
    }

    while (1) {
        for (u32 i = 0; i < state->cfg.n_philosophers; i++) {
            t_instant last_meal = tracker.last_meals[i]
                                      ? tracker.last_meals[i]->timestamp
                                      : state->simulation_start;
            if (duration_since(&last_meal).micros >
                state->cfg.time_to_die_ms * 1000) {
                printf("%06u %u HAS NOT EATEN SINCE %06u AND FUCKING DIED\n",
                       timestamp_ms(instant_now(), state->simulation_start),
                       i + 1, timestamp_ms(last_meal, state->simulation_start));
                exit(0);
            }
        }
        if (state->messages->head) {
            t_message* message = mq_pop(state->messages);
            log_message(message, state->simulation_start);
            if (message->state == EATING) {
                free(tracker.last_meals[message->index]);
                tracker.last_meals[message->index] = message;
                if (state->cfg.track_meals) {
                    tracker.n_meals[message->index] += 1;
                    if (everyone_is_full(tracker.n_meals, state->cfg)) {
                        printf("Everybody is full\n");
                        exit(0);
                    }
                }
            } else {
                free(message);
            }
        }
    }
}

static bool everyone_is_full(u32* n_meals, t_config cfg) {
    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        if (n_meals[i] < cfg.n_meals)
            return false;
    }
    return true;
}
