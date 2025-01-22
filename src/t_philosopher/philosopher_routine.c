/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:08:22 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:09:06 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_philosopher.h"
#include <stdio.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

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
