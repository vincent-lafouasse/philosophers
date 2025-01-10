#pragma once

#include "t_message_queue/t_message_queue.h"
#include "t_philosopher.h"
#include <stdbool.h>

typedef struct s_table {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue* messages;
    t_instant simulation_start;
    bool abort;
    pthread_mutex_t abort_guard;
    t_config cfg;
} t_table;
