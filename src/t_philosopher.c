#include "t_philosopher.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "ft_time.h"
#include "t_error.h"
#include "t_message_queue.h"

t_philosopher philosopher_new(u32 index,
                              pthread_mutex_t* forks,
                              t_message_queue* messages,
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
    };
}

void philosopher_set_state(t_philosopher* self, t_philosopher_state new_state) {
    pthread_mutex_lock(&self->state_lock);
    self->state = new_state;
    pthread_mutex_unlock(&self->state_lock);
    mq_push(self->messages, new_state, self->index);
}

void* thread_routine(void* arg) {
    t_philosopher* self = (t_philosopher*)arg;

    while (1) {
        if (self->state == THINKING) {
            int delay_us =
                (self->cfg.time_to_die_us - self->cfg.time_to_eat_us -
                 self->cfg.time_to_sleep_us) /
                10;
            if (delay_us > 0)
                checked_sleep(delay_us);
            pthread_mutex_lock(self->first_fork);
            philosopher_set_state(self, FORK_HANDED);
            continue;
        } else if (self->state == FORK_HANDED) {
            pthread_mutex_lock(self->second_fork);
            mq_push(self->messages, FORK_HANDED, self->index);
            philosopher_set_state(self, EATING);
            continue;
        } else if (self->state == EATING) {
            checked_sleep(self->cfg.time_to_eat_us);
            pthread_mutex_unlock(self->first_fork);
            pthread_mutex_unlock(self->second_fork);
            philosopher_set_state(self, SLEEPING);
            continue;
        } else if (self->state == SLEEPING) {
            checked_sleep(self->cfg.time_to_sleep_us);
            philosopher_set_state(self, THINKING);
            continue;
        } else {
            break;
        }
    }
    return NULL;
}

t_error philosopher_start(t_philosopher* self) {
    pthread_mutex_init(&self->state_lock, NULL);

    int status = pthread_create(&self->thread, NULL, thread_routine, self);

    if (status == 0)
        return E_PTHREAD_CREATE;

    return NO_ERROR;
}
