#include "t_big_red_button.h"
#include "t_error/t_error.h"

t_error big_red_button_init(t_big_red_button *out) {
    out->abort = false;
    if (pthread_mutex_init(&out->abort_guard, NULL))
        return E_MUTEX_INIT;
    else
        return NO_ERROR;
}
