#include "t_message_queue.h"

#include <stdio.h>
#include <stdlib.h>

void log_message(const t_message* message, t_instant start) {
    printf("%06u %u", duration_since(&start).milliseconds, message->index + 1);
    if (message->state == THINKING)
        printf(" is thinking\n");
    if (message->state == FORK_HANDED)
        printf(" is has taken a fork\n");
    if (message->state == EATING)
        printf(" is eating\n");
    if (message->state == SLEEPING)
        printf(" is sleeping\n");
    if (message->state == DEAD)
        printf(" is ded\n");
}

t_message_queue mq_new(void) {
    t_message_queue out;

    out.head = NULL;
    out.tail = NULL;
    pthread_mutex_init(&out.guard, NULL);
    return out;
}

bool mq_push(t_message_queue* mq, t_philosopher_state state, u32 index) {
    t_message* message = malloc(sizeof(*message));
    if (!message)
        return false;
    *message = (t_message){.state = state,
                           .next = NULL,
                           .prev = NULL,
                           .timestamp = instant_now(),
                           .index = index};

    pthread_mutex_lock(&mq->guard);
    if (!mq->head) {
        mq->head = message;
        mq->tail = message;
    } else {
        message->prev = mq->tail;
        mq->tail->next = message;
        mq->tail = message;
    }
    pthread_mutex_unlock(&mq->guard);
    return true;
}

t_message* mq_pop(t_message_queue* mq) {
    pthread_mutex_lock(&mq->guard);
    if (!mq->head) {
        pthread_mutex_unlock(&mq->guard);
        return NULL;
    }
    t_message* out = mq->head;

    if (out->next != NULL)
        out->next->prev = NULL;
    mq->head = out->next;
    pthread_mutex_unlock(&mq->guard);

    return out;
}
