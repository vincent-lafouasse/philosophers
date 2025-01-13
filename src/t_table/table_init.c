#include "t_error/t_error.h"
#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table.h"
#include "t_table/t_big_red_button.h"
#include "time/ft_time.h"
#include <stdlib.h>
#include <string.h>

static void	allocate_memory(t_table *table, t_config cfg)
{
	table->philosophers = malloc(cfg.n_philosophers
			* sizeof(*table->philosophers));
	table->forks = malloc(cfg.n_philosophers * sizeof(*table->forks));
	table->messages = malloc(sizeof(*table->messages));
	table->abort_button = malloc(sizeof(*table->abort_button));
	if (table->philosophers)
		memset(table->philosophers, 0, cfg.n_philosophers * sizeof(*table->philosophers));
	if (table->forks)
		memset(table->forks, 0, cfg.n_philosophers * sizeof(*table->forks));
	if (table->messages)
		memset(table->messages, 0, sizeof(*table->messages));
	if (table->abort_button)
		memset(table->abort_button, 0, sizeof(*table->abort_button));
}

static void cleanup(t_philosopher* philos, pthread_mutex_t* forks, t_message_queue* mq, t_big_red_button* abort_button, t_config cfg) {
	t_u32 i;

	if (philos) {
		free(philos);
	}
	i = 0;
	if (forks) {
		while (i < cfg.n_philosophers) {
			pthread_mutex_destroy(forks + i++);
		}
		free(forks);
	}
	if (mq) {
		mq_clear(mq);
		free(mq);
	}
	if (abort_button) {
		pthread_mutex_destroy(&abort_button->abort_guard);
		free(abort_button);
	}
}

static t_error init_forks(pthread_mutex_t* forks, t_config cfg) {
	t_u32 i = 0;
	t_u32 j;

	while (i < cfg.n_philosophers) {
		if (pthread_mutex_init(forks + i, NULL)) {
			j = 0;
			while (j < i) {
				pthread_mutex_destroy(forks + j++);
			}
			free(forks);
			return E_MUTEX_INIT;
		}
		i++;
	}
	return NO_ERROR;
}


t_error	table_init(t_config cfg, t_table *table)
{
	memset(table, 0, sizeof(*table));
	table->cfg = cfg;
	allocate_memory(table, cfg);
	if (!table->philosophers || !table->forks || !table->messages || !table->abort_button)
	{
		free(table->philosophers);
		free(table->forks);
		free(table->messages);
		free(table->abort_button);
		return (E_OOM);
	}
	if (init_forks(table->forks, cfg) != NO_ERROR) {
		free(table->philosophers);
		free(table->messages);
		free(table->abort_button);
		return (E_MUTEX_INIT);
	}
	if (mq_new(table->messages) != NO_ERROR) {

		cleanup(table->philosophers, table->forks, NULL, NULL, cfg);
		free(table->messages);
		free(table->abort_button);
		return E_MUTEX_INIT;
	}
	if (big_red_button_init(table->abort_button) != NO_ERROR) {
		cleanup(table->philosophers, table->forks, table->messages, NULL, cfg);
		free(table->abort_button);
		return E_MUTEX_INIT;
	}
	for (t_u32 i = 0; i < cfg.n_philosophers; i++)
	{
		table->philosophers[i] = philosopher_new(i, table->forks,
				table->messages, table->abort_button, cfg);
	}
	table->simulation_start = instant_now();
	return (NO_ERROR);
}
