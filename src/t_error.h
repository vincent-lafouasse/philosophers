#pragma once

typedef enum e_error {
    NO_ERROR,
    E_OOM,
    E_PTHREAD_CREATE,
    E_PTHREAD_JOIN,
    E_NOT_A_NUMBER,
    E_OVERFLOW,
    E_BADUSAGE,
} t_error;
