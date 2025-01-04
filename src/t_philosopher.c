#include "t_philosopher.h"
#include <stdio.h>
#include "t_error.h"

t_philosopher philosopher_new(u32 index, const t_config* cfg)
{
    return (t_philosopher){.index = index, .cfg = cfg};
}

void* thread_routine(void* arg)
{
    (void)arg;
    printf("thread starting\n");
    return NULL;
}

t_error philosopher_start(t_philosopher* self)
{
    int status = pthread_create(&self->thread, NULL, thread_routine, self);

    if (status == 0)
        return E_PTHREAD_CREATE;

    return NO_ERROR;
}
