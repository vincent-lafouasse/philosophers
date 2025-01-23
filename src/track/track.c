/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:22:18 by poss              #+#    #+#             */
/*   Updated: 2025/01/23 14:04:30 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_config/t_config.h"
#include "t_table/t_big_red_button.h"
#include "t_tracker.h"
#include "time/ft_time.h"
#include "track.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

typedef enum e_simulation_status
{
	CONTINUE,
	DONE
}					t_simulation_status;

t_simulation_status	process_meal(t_message *message, t_tracker *tracker,
		t_table *table)
{
	free(tracker->last_meals[message->index]);
	tracker->last_meals[message->index] = message;
	if (table->cfg.track_meals)
	{
		tracker->n_meals[message->index] += 1;
		if (everyone_is_full(tracker->n_meals, table->cfg))
		{
			big_red_button_press(table->abort_button);
			printf("Everybody is full\n");
			return (DONE);
		}
	}
	return (CONTINUE);
}

t_simulation_status	track_progress_inner(t_table *table, t_tracker *tracker)
{
	t_message	*message;

	if (must_stop(table, tracker))
		return (DONE);
	if (!mq_isempty(table->messages))
	{
		message = mq_pop(table->messages);
		log_message(message, table->simulation_start);
		if (message->state == EATING)
		{
			if (process_meal(message, tracker, table) == DONE)
				return (DONE);
		}
		else
			free(message);
	}
	return (CONTINUE);
}

t_error	track_progress(t_table *table)
{
	t_tracker	tracker;
	t_error		err;

	if (table->cfg.n_meals == 0)
	{
		big_red_button_press(table->abort_button);
		printf("Everybody is full\n");
		return NO_ERROR;
	}
	err = init_tracker(&tracker, table);
	if (err != NO_ERROR)
		return (err);
	while (track_progress_inner(table, &tracker) == CONTINUE)
		;
	cleanup_tracker(&tracker, table);
	return (NO_ERROR);
}
