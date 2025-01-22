/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:08:22 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:17:37 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_philosopher.h"
#include "time/ft_time.h"

void	*unwind(t_philosopher *self);

static void	think(t_philosopher *self)
{
	int	delay_ms;

	delay_ms = ((int)self->cfg.time_to_die_ms - (int)self->cfg.time_to_eat_ms
			- (int)self->cfg.time_to_sleep_ms) / 10;
	if (delay_ms > 0)
		checked_sleep(delay_ms * 1000);
	pthread_mutex_lock(self->first_fork);
	philosopher_set_state(self, FORK_HANDED1);
}

static void	eat(t_philosopher *self)
{
	checked_sleep(self->cfg.time_to_eat_ms * 1000);
	pthread_mutex_unlock(self->first_fork);
	pthread_mutex_unlock(self->second_fork);
	philosopher_set_state(self, SLEEPING);
}

static void	fork_handed_1(t_philosopher *self)
{
	pthread_mutex_lock(self->second_fork);
	philosopher_set_state(self, FORK_HANDED2);
}

static void	sleep__(t_philosopher *self)
{
	checked_sleep(self->cfg.time_to_sleep_ms * 1000);
	philosopher_set_state(self, THINKING);
}

void	*thread_routine(void *arg)
{
	t_philosopher	*self;

	self = (t_philosopher *)arg;
	while (1)
	{
		if (must_abort(self->abort_button))
			return (unwind(self));
		if (self->state == THINKING)
			think(self);
		else if (self->state == FORK_HANDED1)
			fork_handed_1(self);
		else if (self->state == FORK_HANDED2)
			philosopher_set_state(self, EATING);
		else if (self->state == EATING)
			eat(self);
		else if (self->state == SLEEPING)
			sleep__(self);
		else
			break ;
	}
	return (NULL);
}
