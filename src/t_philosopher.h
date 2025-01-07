#pragma once

#include <pthread.h>
#include "t_config.h"
#include "t_error.h"

typedef enum e_philosopher_state t_philosopher_state;
enum e_philosopher_state {
    THINKING,
    FORK_HANDED,
    EATING,
    SLEEPING,
    DEAD,
};

typedef struct s_philosopher t_philosopher;
struct s_philosopher {
    pthread_t thread;
    u32 index;
    pthread_mutex_t* first_fork;
    pthread_mutex_t* second_fork;
    pthread_mutex_t* output_lock;
    t_philosopher_state state;
    pthread_mutex_t state_lock;
    t_config cfg;
};

t_philosopher philosopher_new(u32 index, pthread_mutex_t* forks, t_config cfg);
t_error philosopher_start(t_philosopher* self);
