/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table_fields.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:00:04 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:01:28 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error/t_error.h"
#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table.h"
#include "t_table/t_big_red_button.h"
#include <stdlib.h>
#include <string.h>

t_error	init_forks(pthread_mutex_t **forks, t_config cfg)
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

t_error	init_abort_button(t_big_red_button **button)
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

t_error	init_mq(t_message_queue **mq)
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

t_error	init_philos(t_table *table)
{
	t_u32	i;

	table->philosophers = malloc(table->cfg.n_philosophers
			* sizeof(*table->philosophers));
	if (table->philosophers == NULL)
		return (E_OOM);
	i = 0;
	while (i < table->cfg.n_philosophers)
	{
		table->philosophers[i] = philosopher_new(i, table->forks,
				table->messages, table->abort_button, table->cfg);
		i++;
	}
	return (NO_ERROR);
}
