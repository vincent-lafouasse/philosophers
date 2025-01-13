#include "t_message_queue/t_message_queue.h"
#include "t_table.h"
#include "time/ft_time.h"
#include <stdlib.h>
#include <string.h>

static void	allocate_memory(t_table *table, t_config cfg)
{
	table->philosophers = malloc(cfg.n_philosophers
			* sizeof(*table->philosophers));
	table->forks = malloc(cfg.n_philosophers * sizeof(*table->forks));
	table->messages = malloc(sizeof(*table->messages));
	if (table->philosophers)
		memset(table->philosophers, 0, cfg.n_philosophers * sizeof(*table->philosophers));
	if (table->forks)
		memset(table->forks, 0, cfg.n_philosophers * sizeof(*table->forks));
	if (table->messages)
		memset(table->messages, 0, cfg.n_philosophers * sizeof(*table->messages));
}

t_error	table_init(t_config cfg, t_table *table)
{
	memset(table, 0, sizeof(*table));
	table->cfg = cfg;
	allocate_memory(table, cfg);
	if (!table->philosophers || !table->forks || !table->messages)
	{
		free(table->philosophers);
		free(table->forks);
		free(table->messages);
		return (E_OOM);
	}
	table->simulation_start = instant_now();
	if (mq_new(table->messages) != NO_ERROR)
		return E_MUTEX_INIT;
	if (big_red_button_init(&table->abort_button) != NO_ERROR)
		return E_MUTEX_INIT;
	for (t_u32 i = 0; i < cfg.n_philosophers; i++)
	{
		pthread_mutex_init(table->forks + i, NULL);
		table->philosophers[i] = philosopher_new(i, table->forks,
				table->messages, &table->abort_button, cfg);
	}
	return (NO_ERROR);
}
