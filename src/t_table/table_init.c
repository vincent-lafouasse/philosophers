#include "t_table.h"

#include <stdlib.h>
#include "time/ft_time.h"
#include "t_message_queue/t_message_queue.h"

t_error table_init(t_config cfg, t_table* table) {
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

    for (t_u32 i = 0; i < cfg.n_philosophers; i++) {
        pthread_mutex_init(table->forks + i, NULL);
    }

    err = big_red_button_init(&table->abort_button);
    if (err != NO_ERROR)
        return err;

    for (t_u32 i = 0; i < cfg.n_philosophers; i++)
        table->philosophers[i] =
            philosopher_new(i, table->forks, table->messages, &table->abort_button, cfg);

    return NO_ERROR;
}
