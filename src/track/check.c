/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:42:31 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:43:21 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_tracker.h"
#include "t_table/t_table.h"

bool	must_stop(t_table *table, t_tracker *tracker)
{
	if (must_abort(table->abort_button))
	{
		return (true);
	}
	if (!everybodys_alive(table, tracker))
	{
		return (true);
	}
	if (must_abort(table->abort_button))
	{
		return (true);
	}
	return (false);
}

bool	everybodys_alive(t_table *table, t_tracker *tracker)
{
	t_instant	last_meal;
	t_u32		i;

	i = 0;
	while (i < table->cfg.n_philosophers)
	{
		if (tracker->last_meals[i] != NULL)
			last_meal = tracker->last_meals[i]->timestamp;
		else
			last_meal = table->simulation_start;
		if (duration_since(&last_meal).micros > table->cfg.time_to_die_ms
			* 1000)
		{
			big_red_button_press(table->abort_button);
			log_death(i, table->simulation_start, last_meal);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	everyone_is_full(t_u32 *n_meals, t_config cfg)
{
	t_u32	i;

	i = 0;
	while (i < cfg.n_philosophers)
	{
		if (n_meals[i] < cfg.n_meals)
			return (false);
		i++;
	}
	return (true);
}
