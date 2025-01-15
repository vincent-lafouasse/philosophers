#include "t_philosopher.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "t_table/t_big_red_button.h"
#include "time/ft_time.h"
#include "t_message_queue/t_message_queue.h"

#ifdef DEBUG
#define VERBOSITY 1
#else
#define VERBOSITY 0
#endif

t_philosopher philosopher_new(t_u32 index,
                              pthread_mutex_t* forks,
                              t_message_queue* messages,
                              t_big_red_button* abort_button,
                              t_config cfg) {
    pthread_mutex_t* first_fork;
    pthread_mutex_t* second_fork;

    if (index % 2) {
        first_fork = forks + index;
        second_fork = forks + (index + 1) % cfg.n_philosophers;
    } else {
        first_fork = forks + (index + 1) % cfg.n_philosophers;
        second_fork = forks + index;
    }

    return (t_philosopher){
        .index = index,
        .cfg = cfg,
        .first_fork = first_fork,
        .second_fork = second_fork,
        .state = THINKING,
        .messages = messages,
        .abort_button = abort_button,
    };
}

void philosopher_set_state(t_philosopher* self, t_state new_state) {
    self->state = new_state;
    if (mq_push(self->messages, new_state, self->index) != NO_ERROR)
    {
        big_red_button_press(self->abort_button);
        printf("Out of memory\n");
    }
}

void* thread_routine(void* arg) {
    t_philosopher* self = (t_philosopher*)arg;

    while (1) {
        if (must_abort(self->abort_button)) {
            if (VERBOSITY == 1)
                printf("philo %u shutdown\n", self->index + 1);
            if (self->state == FORK_HANDED1) {
                pthread_mutex_unlock(self->first_fork);
            }
            if (self->state == FORK_HANDED2 || self->state == EATING) {
                pthread_mutex_unlock(self->first_fork);
                pthread_mutex_unlock(self->second_fork);
            }
            return NULL;
        }
        if (self->state == THINKING) {
            int delay_ms =
                ((int)self->cfg.time_to_die_ms - (int)self->cfg.time_to_eat_ms -
                 (int)self->cfg.time_to_sleep_ms) /
                10;
            if (delay_ms > 0)
                checked_sleep(delay_ms * 1000);
            pthread_mutex_lock(self->first_fork);
            philosopher_set_state(self, FORK_HANDED1);
            continue;
        } else if (self->state == FORK_HANDED1) {
            pthread_mutex_lock(self->second_fork);
            philosopher_set_state(self, FORK_HANDED2);
            continue;
        } else if (self->state == FORK_HANDED2) {
            philosopher_set_state(self, EATING);
            continue;
        } else if (self->state == EATING) {
            checked_sleep(self->cfg.time_to_eat_ms * 1000);
            pthread_mutex_unlock(self->first_fork);
            pthread_mutex_unlock(self->second_fork);
            philosopher_set_state(self, SLEEPING);
            continue;
        } else if (self->state == SLEEPING) {
            checked_sleep(self->cfg.time_to_sleep_ms * 1000);
            philosopher_set_state(self, THINKING);
            continue;
        } else {
            break;
        }
    }
    return NULL;
}

t_error philosopher_start(t_philosopher* self) {
    int status = pthread_create(&self->thread, NULL, thread_routine, self);

    if (status == 0)
        return E_PTHREAD_CREATE;

    return NO_ERROR;
}
