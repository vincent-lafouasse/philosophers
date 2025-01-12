#pragma once

#include <stdint.h>
#include <sys/time.h>

typedef uint32_t u32;

typedef struct s_duration t_duration;
struct s_duration {
    u32 micros;
};

typedef struct timeval t_instant;

t_instant instant_now(void);
t_duration duration_since(const t_instant* instant);
t_duration duration_difference(const t_instant* to, const t_instant* from);
u32 timestamp_ms(t_instant instant, t_instant start);
void checked_sleep(u32 us);
