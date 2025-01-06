#include "t_philosopher.h"
#include <stdio.h>
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
    printf("thread %u starting\n", self.index);
    return NULL;
}

t_error philosopher_start(t_philosopher* self) {
    int status = pthread_create(&self->thread, NULL, thread_routine, self);

    if (status == 0)
        return E_PTHREAD_CREATE;

    return NO_ERROR;
}
