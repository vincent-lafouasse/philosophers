/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_philosopher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:02:40 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:06:00 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_message_queue/t_message_queue.h"
#include "t_philosopher.h"
#include "t_table/t_table.h"
#include "time/ft_time.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

t_philosopher	philosopher_new(t_u32 index, t_table *table)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (index % 2)
	{
		first_fork = table->forks + index;
		second_fork = table->forks + (index + 1) % table->cfg.n_philosophers;
	}
	else
	{
		first_fork = table->forks + (index + 1) % table->cfg.n_philosophers;
		second_fork = table->forks + index;
	}
	return (t_philosopher){
		.index = index,
		.cfg = table->cfg,
		.first_fork = first_fork,
		.second_fork = second_fork,
		.state = THINKING,
		.messages = table->messages,
		.abort_button = table->abort_button,
	};
}

void	philosopher_set_state(t_philosopher *self, t_state new_state)
{
	self->state = new_state;
	if (mq_push(self->messages, new_state, self->index) != NO_ERROR)
	{
		big_red_button_press(self->abort_button);
		printf("Out of memory\n");
	}
}

void	*lone_philosopher(void *arg)
{
	t_philosopher	*self;

	self = (t_philosopher *)arg;
	pthread_mutex_lock(self->first_fork);
	philosopher_set_state(self, FORK_HANDED1);
	checked_sleep(1100 * self->cfg.time_to_die_ms);
	pthread_mutex_unlock(self->first_fork);
	return (NULL);
}

void	*thread_routine(void *arg)
{
	t_philosopher	*self;
	int				delay_ms;

	self = (t_philosopher *)arg;
	while (1)
	{
		if (must_abort(self->abort_button))
		{
			if (VERBOSITY == 1)
				printf("philo %u shutdown\n", self->index + 1);
			if (self->state == FORK_HANDED1)
			{
				pthread_mutex_unlock(self->first_fork);
			}
			if (self->state == FORK_HANDED2 || self->state == EATING)
			{
				pthread_mutex_unlock(self->first_fork);
				pthread_mutex_unlock(self->second_fork);
			}
			return (NULL);
		}
		if (self->state == THINKING)
		{
			delay_ms = ((int)self->cfg.time_to_die_ms
					- (int)self->cfg.time_to_eat_ms
					- (int)self->cfg.time_to_sleep_ms) / 10;
			if (delay_ms > 0)
				checked_sleep(delay_ms * 1000);
			pthread_mutex_lock(self->first_fork);
			philosopher_set_state(self, FORK_HANDED1);
			continue ;
		}
		else if (self->state == FORK_HANDED1)
		{
			pthread_mutex_lock(self->second_fork);
			philosopher_set_state(self, FORK_HANDED2);
			continue ;
		}
		else if (self->state == FORK_HANDED2)
		{
			philosopher_set_state(self, EATING);
			continue ;
		}
		else if (self->state == EATING)
		{
			checked_sleep(self->cfg.time_to_eat_ms * 1000);
			pthread_mutex_unlock(self->first_fork);
			pthread_mutex_unlock(self->second_fork);
			philosopher_set_state(self, SLEEPING);
			continue ;
		}
		else if (self->state == SLEEPING)
		{
			checked_sleep(self->cfg.time_to_sleep_ms * 1000);
			philosopher_set_state(self, THINKING);
			continue ;
		}
		else
		{
			break ;
		}
	}
	return (NULL);
}

t_error	philosopher_start(t_philosopher *self)
{
	int	status;

	if (self->cfg.n_philosophers != 1)
		status = pthread_create(&self->thread, NULL, thread_routine, self);
	else
		status = pthread_create(&self->thread, NULL, lone_philosopher, self);
	if (status == 0)
		return (E_PTHREAD_CREATE);
	return (NO_ERROR);
}
