#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "time/ft_time.h"
#include "t_config/t_config.h"
#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table/t_table.h"
#include "track/track.h"

static t_error init(t_config cfg, t_table* table);
static t_error run(t_table* table);
static void cleanup(t_table* table);

int main(int ac, char* av[]) {
    t_config cfg;
    t_error err;

    err = load_config(ac, av, &cfg);
    if (err != NO_ERROR) {
        printf("Failed to load config, %s\n", error_repr(err));
        exit(1);
    }
    log_config(cfg);

    t_table table;
    err = init(cfg, &table);
    run(&table);
    track_progress(&table);
    cleanup(&table);
}

static t_error init(t_config cfg, t_table* table) {
    *table =
        (t_table){.philosophers =
                      malloc(cfg.n_philosophers * sizeof(*table->philosophers)),
                  .forks = malloc(cfg.n_philosophers * sizeof(*table->forks)),
                  .messages = malloc(sizeof(*table->messages)),
                  .simulation_start = instant_now(),
                  .cfg = cfg};
    if (!table->philosophers || !table->forks || !table->messages) {
        free(table->philosophers);
        free(table->forks);
        free(table->messages);
        return E_OOM;
    }
    t_error err = mq_new(table->messages);
    if (err != NO_ERROR)
        return err;

    for (u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(table->forks + i, NULL);
    }

    err = big_red_button_init(&table->abort_button);
    if (err != NO_ERROR)
        return err;

    for (u32 i = 0; i < cfg.n_philosophers; i++)
        table->philosophers[i] =
            philosopher_new(i, table->forks, table->messages, &table->abort_button, cfg);

    return NO_ERROR;
}

static t_error run(t_table* table) {
    for (u32 i = 0; i < table->cfg.n_philosophers; i++) {
        philosopher_start(table->philosophers + i);  // fallible
    }
    return NO_ERROR;
}

static void cleanup(t_table* table) {
    (void)table;
    for (u32 i = 0; i < table->cfg.n_philosophers; i++) {
        pthread_join(table->philosophers[i].thread, NULL);
    }
    printf("cleanup\n");
}
