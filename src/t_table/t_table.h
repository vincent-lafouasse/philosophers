#pragma once

#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"
#include "t_big_red_button.h"
#include <stdbool.h>

typedef struct s_table {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue* messages;
    t_instant simulation_start;
    t_big_red_button* abort_button;
    t_config cfg;
} t_table;

t_error table_init(t_config cfg, t_table* table);
void table_cleanup(t_table* table);
