/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_tracker.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:38:06 by poss              #+#    #+#             */
/*   Updated: 2025/01/23 14:02:56 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_tracker.h"
#include "t_table/t_table.h"
#include <stdlib.h>
#include <string.h>

t_error	init_tracker(t_tracker *tracker, t_table *table)
{
	tracker->last_meals = malloc(table->cfg.n_philosophers
			* sizeof(t_message *));
	if (tracker->last_meals == NULL)
	{
		big_red_button_press(table->abort_button);
		return (E_OOM);
	}
	memset(tracker->last_meals, 0, table->cfg.n_philosophers
		* sizeof(t_message *));
	if (table->cfg.track_meals)
	{
		tracker->n_meals = malloc(table->cfg.n_philosophers * sizeof(t_u32));
		if (tracker->n_meals == NULL)
		{
			free(tracker->last_meals);
			big_red_button_press(table->abort_button);
			return (E_OOM);
		}
		memset(tracker->n_meals, 0, table->cfg.n_philosophers
			* sizeof(*tracker->n_meals));
	}
	return (NO_ERROR);
}

void	cleanup_tracker(t_tracker *t, const t_table *table)
{
	t_u32	i;

	i = 0;
	while (i < table->cfg.n_philosophers)
	{
		free(t->last_meals[i]);
		i++;
	}
	free(t->last_meals);
	if (table->cfg.track_meals)
		free(t->n_meals);
}
