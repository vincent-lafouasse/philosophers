#include "t_table.h"

#include <stdio.h>

#ifdef DEBUG
#define VERBOSITY 1
#else
#define VERBOSITY 0
#endif

void table_cleanup(t_table* table) {
    (void)table;
    for (t_u32 i = 0; i < table->cfg.n_philosophers; i++) {
        pthread_join(table->philosophers[i].thread, NULL);
    }
    if (VERBOSITY == 1)
        printf("cleanup\n");
}
