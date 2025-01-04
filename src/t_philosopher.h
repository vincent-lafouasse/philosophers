#pragma once

#include <pthread.h>
#include "t_config.h"
#include "t_error.h"

typedef struct s_philosopher t_philosopher;
struct s_philosopher
{
    pthread_t* thread;
    u32 index;
    const t_config* cfg;
    // mutex* left_fork;
    // mutex* right_fork;
};

t_philosopher philosopher_new(u32 index, const t_config* cfg);
t_error philosopher_start(t_philosopher* self);
