#include "track.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_time.h"
#include "t_big_red_button.h"
#include "t_config/t_config.h"
#include "t_message_queue/t_message_queue.h"

#ifdef VERBOSE
#define VERBOSITY 1
#else
#define VERBOSITY 0
#endif

typedef uint32_t u32;

typedef struct {
    t_message** last_meals;
    u32* n_meals;
} t_tracker;

static bool everyone_is_full(u32* n_meals, t_config cfg);

void log_message(const t_message* message, t_instant start) {
    printf("%06u %u", timestamp_ms(message->timestamp, start),
           message->index + 1);
    if (message->state == THINKING)
        printf(" is thinking\n");
    if (message->state == FORK_HANDED)
        printf(" has taken a fork\n");
    if (message->state == EATING)
        printf(" is eating\n");
    if (message->state == SLEEPING)
        printf(" is sleeping\n");
}

void log_death(u32 philo, t_instant start, t_instant last_meal) {
    if (VERBOSITY == 1)
        printf("%06u %u HAS NOT EATEN SINCE %06u AND FUCKING DIED\n",
               timestamp_ms(instant_now(), start), philo + 1,
               timestamp_ms(last_meal, start));
    else
        printf("%06u %u has died\n", timestamp_ms(instant_now(), start),
               philo + 1);
}

t_error track_progress_inner(t_table* table, t_tracker* tracker);

t_error track_progress(t_table* table) {
    t_tracker tracker = (t_tracker){
        .last_meals = malloc(table->cfg.n_philosophers * sizeof(t_message*)),
        .n_meals = NULL};
    if (table->cfg.track_meals) {
        tracker.n_meals = malloc(table->cfg.n_philosophers * sizeof(u32));
        memset(tracker.n_meals, 0,
               table->cfg.n_philosophers * sizeof(*tracker.n_meals));
    }

    while (1) {
        for (u32 i = 0; i < table->cfg.n_philosophers; i++) {
            t_instant last_meal = tracker.last_meals[i]
                                      ? tracker.last_meals[i]->timestamp
                                      : table->simulation_start;
            if (duration_since(&last_meal).micros >
                table->cfg.time_to_die_ms * 1000) {
                printf("%06u %u HAS NOT EATEN SINCE %06u AND FUCKING DIED\n",
                       timestamp_ms(instant_now(), table->simulation_start),
                       i + 1, timestamp_ms(last_meal, table->simulation_start));
                big_red_button_press(&table->abort_button);
                break;
            }
        }
        if (table->messages->head) {
            t_message* message = mq_pop(table->messages);
            log_message(message, table->simulation_start);
            if (message->state == EATING) {
                free(tracker.last_meals[message->index]);
                tracker.last_meals[message->index] = message;
                if (table->cfg.track_meals) {
                    tracker.n_meals[message->index] += 1;
                    if (everyone_is_full(tracker.n_meals, table->cfg)) {
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
