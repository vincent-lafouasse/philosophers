#pragma once

#include <pthread.h>
#include <stdbool.h>
#include "t_error/t_error.h"

typedef struct s_big_red_button {
    bool abort;
    pthread_mutex_t abort_guard;
} t_big_red_button;

t_error big_red_button_init(t_big_red_button* out);
bool must_abort(t_big_red_button* button);
void big_red_button_press(t_big_red_button* button);
