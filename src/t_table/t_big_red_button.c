#include "t_big_red_button.h"
#include "t_error/t_error.h"

#include <stdio.h>

t_error big_red_button_init(t_big_red_button *out) {
    out->abort = false;
    if (pthread_mutex_init(&out->abort_guard, NULL))
        return E_MUTEX_INIT;
    else
        return NO_ERROR;
}

bool must_abort(t_big_red_button* button) {
    bool out;

    pthread_mutex_lock(&button->abort_guard);
    out = button->abort;
    pthread_mutex_unlock(&button->abort_guard);
    return out;
}

void big_red_button_press(t_big_red_button* button) {
    printf("ABORT\n");
    pthread_mutex_lock(&button->abort_guard);
    button->abort = true;
    pthread_mutex_unlock(&button->abort_guard);
}
