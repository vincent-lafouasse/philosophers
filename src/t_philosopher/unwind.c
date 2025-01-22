/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unwind.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:15:03 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:16:03 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_philosopher.h"
#include <stdio.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

void	*unwind(t_philosopher *self)
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
