#pragma once

#include <stdint.h>
#include <sys/time.h>

typedef uint32_t u32;

typedef struct s_duration t_duration;
struct s_duration {
    u32 milliseconds;
};

typedef struct timeval t_instant;

t_instant instant_now(void);
t_duration duration_since(const t_instant* instant);
