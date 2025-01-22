/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:22:18 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:36:27 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_config/t_config.h"
#include "t_message_queue/t_message_queue.h"
#include "t_table/t_big_red_button.h"
#include "time/ft_time.h"
#include "track.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

typedef struct s_tracker
{
	t_message		**last_meals;
	t_u32			*n_meals;
}					t_tracker;

static bool			everyone_is_full(t_u32 *n_meals, t_config cfg);

void	log_message(const t_message *message, t_instant start)
{
	printf("%06u %u", timestamp_ms(message->timestamp, start), message->index
		+ 1);
	if (message->state == THINKING)
		printf(" is thinking\n");
	if (message->state == FORK_HANDED1 || message->state == FORK_HANDED2)
		printf(" has taken a fork\n");
	if (message->state == EATING)
		printf(" is eating\n");
	if (message->state == SLEEPING)
		printf(" is sleeping\n");
}

void	log_death(t_u32 philo, t_instant start, t_instant last_meal)
{
	if (VERBOSITY == 1)
		printf("%06u %u HAS NOT EATEN SINCE %06u AND FUCKING DIED\n",
			timestamp_ms(instant_now(), start), philo + 1,
			timestamp_ms(last_meal, start));
	else
		printf("%06u %u has died\n", timestamp_ms(instant_now(), start), philo
			+ 1);
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

typedef enum e_simulation_status
{
	CONTINUE,
	DONE
}					t_simulation_status;

static bool	must_stop(t_table *table, t_tracker *tracker)
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

static void	cleanup_tracker(t_tracker *t, const t_table *table)
{
	t_u32	i;

	i = 0;
	while (i < table->cfg.n_philosophers)
	{
		free(t->last_meals[i]);
		i++;
	}
	free(t->last_meals);
	free(t->n_meals);
}

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

t_error	track_progress(t_table *table)
{
	t_tracker	tracker;
	t_error		err;

	err = init_tracker(&tracker, table);
	if (err != NO_ERROR)
		return (err);
	while (track_progress_inner(table, &tracker) == CONTINUE)
		;
	cleanup_tracker(&tracker, table);
	return (NO_ERROR);
}

static bool	everyone_is_full(t_u32 *n_meals, t_config cfg)
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
