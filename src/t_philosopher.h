#pragma once

#include <pthread.h>
#include "t_config/t_config.h"
#include "t_error/t_error.h"

struct s_message_queue;
typedef struct s_message_queue t_message_queue;

typedef enum e_state t_state;
enum e_state {
    THINKING,
    FORK_HANDED,
    EATING,
    SLEEPING,
    DEAD,
};

typedef struct s_philosopher t_philosopher;
struct s_philosopher {
    pthread_t thread;
    t_u32 index;
    pthread_mutex_t* first_fork;
    pthread_mutex_t* second_fork;
    pthread_mutex_t* output_lock;
    t_state state;
    pthread_mutex_t state_lock;
    t_message_queue* messages;
    t_config cfg;
};

t_philosopher philosopher_new(t_u32 index,
                              pthread_mutex_t* forks,
                              t_message_queue* messages,
                              t_config cfg);
t_error philosopher_start(t_philosopher* self);
