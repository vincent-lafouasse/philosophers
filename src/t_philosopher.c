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

    if (index != cfg.n_philosophers - 1) {
        first_fork = forks + index;
        second_fork = forks + index + 1;
    } else {
        first_fork = forks;
        second_fork = forks + cfg.n_philosophers - 1;
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
}

void* thread_routine(void* arg) {
    t_philosopher* self = (t_philosopher*)arg;

    mq_push(self->messages, THINKING); // test

    while (1) {
        if (self->state == THINKING) {
            pthread_mutex_lock(self->first_fork);
            philosopher_set_state(self, FORK_HANDED);
            continue;
        } else if (self->state == FORK_HANDED) {
            pthread_mutex_lock(self->second_fork);
            philosopher_set_state(self, EATING);
            continue;
        } else if (self->state == EATING) {
            usleep(self->cfg.time_to_eat_us);
            pthread_mutex_unlock(self->first_fork);
            pthread_mutex_unlock(self->second_fork);
            philosopher_set_state(self, SLEEPING);
            continue;
        } else if (self->state == SLEEPING) {
            usleep(self->cfg.time_to_sleep_us);
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
