/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:53:38 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 18:57:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error/t_error.h"
#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table.h"
#include "t_table/t_big_red_button.h"
#include "time/ft_time.h"
#include <stdlib.h>
#include <string.h>

static void	cleanup(pthread_mutex_t *forks, t_big_red_button *abort_button,
		t_message_queue *mq, t_config cfg)
{
	t_u32	i;

	i = 0;
	if (forks)
	{
		while (i < cfg.n_philosophers)
		{
			pthread_mutex_destroy(forks + i++);
		}
		free(forks);
	}
	if (mq)
	{
		mq_clear(mq);
		free(mq);
	}
	if (abort_button)
	{
		pthread_mutex_destroy(&abort_button->abort_guard);
		free(abort_button);
	}
}

static t_error	init_forks(pthread_mutex_t **forks, t_config cfg)
{
	t_u32	i;
	t_u32	j;

	i = 0;
	*forks = malloc(cfg.n_philosophers * sizeof(**forks));
	if (*forks == NULL)
		return (E_OOM);
	memset(*forks, 0, cfg.n_philosophers * sizeof(**forks));
	while (i < cfg.n_philosophers)
	{
		if (pthread_mutex_init(*forks + i, NULL))
		{
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(*forks + j++);
			}
			free(*forks);
			return (E_MUTEX_INIT);
		}
		i++;
	}
	return (NO_ERROR);
}

static t_error	init_abort_button(t_big_red_button **button)
{
	t_error	err;

	*button = malloc(sizeof(**button));
	if (*button == NULL)
		return (E_OOM);
	err = big_red_button_init(*button);
	if (err != NO_ERROR)
	{
		free(*button);
		return (err);
	}
	return (NO_ERROR);
}

static t_error	init_mq(t_message_queue **mq)
{
	t_error	err;

	*mq = malloc(sizeof(**mq));
	if (*mq == NULL)
	{
		return (E_OOM);
	}
	err = mq_new(*mq);
	if (err != NO_ERROR)
	{
		free(*mq);
		return (err);
	}
	return (NO_ERROR);
}

static t_error	init_philos(t_table *table)
{
	table->philosophers = malloc(table->cfg.n_philosophers
			* sizeof(*table->philosophers));
	if (table->philosophers == NULL)
		return (E_OOM);
	for (t_u32 i = 0; i < table->cfg.n_philosophers; i++)
	{
		table->philosophers[i] = philosopher_new(i, table->forks,
				table->messages, table->abort_button, table->cfg);
	}
	return (NO_ERROR);
}

t_error	table_init(t_config cfg, t_table *table)
{
	t_error	err;

	table->cfg = cfg;
	err = init_forks(&table->forks, cfg);
	if (err != NO_ERROR)
		return (err);
	err = init_abort_button(&table->abort_button);
	if (err != NO_ERROR)
	{
		cleanup(table->forks, NULL, NULL, cfg);
		return (err);
	}
	err = init_mq(&table->messages);
	if (err != NO_ERROR)
	{
		cleanup(table->forks, table->abort_button, NULL, cfg);
		return (err);
	}
	err = init_philos(table);
	if (err != NO_ERROR)
	{
		cleanup(table->forks, table->abort_button, table->messages, cfg);
		return (err);
	}
	table->simulation_start = instant_now();
	return (NO_ERROR);
}
