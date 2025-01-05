#pragma once

#include <pthread.h>
#include "t_config.h"
#include "t_error.h"

typedef struct s_philosopher t_philosopher;
struct s_philosopher {
    pthread_t thread;
    u32 index;
    t_config cfg;
    pthread_mutex_t* left_fork;
    pthread_mutex_t* right_fork;
    pthread_mutex_t* output_lock;
};

t_philosopher philosopher_new(u32 index,
                              pthread_mutex_t* forks,
                              pthread_mutex_t* output_lock,
                              t_config cfg);
t_error philosopher_start(t_philosopher* self);
