#include "t_philosopher.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "ft_time.h"
#include "t_error.h"

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
    t_instant death_timer = instant_now();
    t_philosopher_state state = THINKING;

    while (1) {
        if (state == THINKING) {
            printf("%05u %u is thinking\n",
                   duration_since(&simulation_start).milliseconds,
                   self.index + 1);
            pthread_mutex_lock(self.first_fork);
            if (duration_since(&death_timer).milliseconds * 1000 >
                self.cfg.time_to_die_us)
                printf("%05u %u HAS DIED\n",
                       duration_since(&simulation_start).milliseconds,
                       self.index + 1);
            printf("%05u %u has 1 fork\n",
                   duration_since(&simulation_start).milliseconds,
                   self.index + 1);
            pthread_mutex_lock(self.second_fork);
            if (duration_since(&death_timer).milliseconds * 1000 >
                self.cfg.time_to_die_us)
                printf("%05u %u HAS DIED\n",
                       duration_since(&simulation_start).milliseconds,
                       self.index + 1);
            state = EATING;
            continue;
        } else if (state == EATING) {
            printf("%05u %u is eating\n",
                   duration_since(&simulation_start).milliseconds,
                   self.index + 1);
            usleep(self.cfg.time_to_eat_us);
            death_timer = instant_now();
            pthread_mutex_unlock(self.first_fork);
            pthread_mutex_unlock(self.second_fork);
            state = SLEEPING;
            continue;
        } else {
            printf("%05u %u is sleeping\n",
                   duration_since(&simulation_start).milliseconds,
                   self.index + 1);
            usleep(self.cfg.time_to_sleep_us);
            if (duration_since(&death_timer).milliseconds * 1000 >
                self.cfg.time_to_die_us)
                printf("%05u %u HAS DIED\n",
                       duration_since(&simulation_start).milliseconds,
                       self.index + 1);
            state = THINKING;
            continue;
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
