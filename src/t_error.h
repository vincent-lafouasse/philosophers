#pragma once

typedef enum e_error
{
    NO_ERROR,
    E_OOM,
    E_PTHREAD_CREATE,
    E_PTHREAD_JOIN,
} t_error;
