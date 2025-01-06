#include "t_philosopher.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "t_error.h"
#include "ft_time.h"

t_philosopher philosopher_new(u32 index,
                              pthread_mutex_t* forks,
                              pthread_mutex_t* output_lock,
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
        .output_lock = output_lock,

    };
}

void* thread_routine(void* arg) {
    t_philosopher self = *((t_philosopher*)arg);
    t_instant simulation_start = instant_now();
    t_instant last_meal;
    bool has_eaten = false;
    t_philosopher_state state = THINKING;

    while (1) {
        if (state == THINKING) {
            printf("%05u %u is thinking\n", duration_since(&simulation_start).milliseconds, self.index + 1);
            pthread_mutex_lock(self.first_fork);
            printf("%05u %u has 1 fork\n", duration_since(&simulation_start).milliseconds, self.index + 1);
            pthread_mutex_lock(self.second_fork);
            printf("%05u %u is eating\n", duration_since(&simulation_start).milliseconds, self.index + 1);
            usleep(self.cfg.time_to_eat_us);
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
