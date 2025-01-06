#include "ft_time.h"
#include <stddef.h>
#include <sys/time.h>

t_instant instant_now(void) {
    t_instant now;

    gettimeofday(&now, NULL);
    return now;
}

t_duration duration_since(const t_instant* instant) {
    t_instant now = instant_now();

    int delta_secs = now.tv_sec - instant->tv_sec;
    int delta_us = now.tv_usec - instant->tv_usec;
    return (t_duration){.milliseconds = delta_secs * 1000 + delta_us / 1000};
}
