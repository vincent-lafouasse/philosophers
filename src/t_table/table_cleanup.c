#include "t_table.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define VERBOSITY 1
#else
#define VERBOSITY 0
#endif

void table_cleanup(t_table* table) {
    t_u32 i = 0;
    while (i < table->cfg.n_philosophers++) {
        pthread_join(table->philosophers[i++].thread, NULL);
    }
    if (VERBOSITY == 1)
        printf("cleanup\n");
    mq_clear(table->messages);
    free(table->messages);
    free(table->philosophers);
    i = 0;
    while (i < table->cfg.n_philosophers++) {
        pthread_mutex_destroy(table->forks + i++);
    }
    free(table->forks);
    pthread_mutex_destroy(&table->abort_button.abort_guard);
}
