#pragma once

#include <pthread.h>
#include <stdbool.h>
#include "ft_time.h"
#include "t_philosopher.h"

typedef struct s_message t_message;
struct s_message {
    t_philosopher_state state;
    t_instant timestamp;
    u32 index;
    t_message* next;
    t_message* prev;
};

typedef struct s_message_queue t_message_queue;
struct s_message_queue {
    t_message* head;
    t_message* tail;
    pthread_mutex_t guard;
};

t_message_queue mq_new(void);
bool mq_push(t_message_queue* mq, t_philosopher_state state, u32 index);
t_message* mq_pop(t_message_queue* mq);
