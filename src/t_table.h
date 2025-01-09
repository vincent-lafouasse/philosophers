#pragma once

#include "t_message_queue/t_message_queue.h"
#include "t_philosopher.h"

typedef struct s_table {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue* messages;
    t_instant simulation_start;
    t_config cfg;
} t_table;
