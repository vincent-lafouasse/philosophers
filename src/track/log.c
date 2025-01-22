/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:40:02 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:41:18 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_tracker.h"
#include <stdio.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

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
