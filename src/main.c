#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "t_config/t_config.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table/t_table.h"
#include "track/track.h"

static t_error run(t_table* table);

int main(int ac, char* av[]) {
    t_config cfg;
    t_error err;

    err = load_config(ac, av, &cfg);
    if (err != NO_ERROR) {
        printf("Failed to load config, %s\n", error_repr(err));
        exit(1);
    }

    t_table table;
    err = table_init(cfg, &table);
    run(&table);
    track_progress(&table);
    table_cleanup(&table);
}

static t_error run(t_table* table) {
    for (t_u32 i = 0; i < table->cfg.n_philosophers; i++) {
        philosopher_start(table->philosophers + i);  // fallible
    }
    return NO_ERROR;
}
