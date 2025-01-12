/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:50:00 by poss              #+#    #+#             */
/*   Updated: 2025/01/12 15:55:08 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_time.h"
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>

t_instant	instant_now(void)
{
	t_instant	now;

	gettimeofday(&now, NULL);
	return (now);
}

t_duration	duration_since(const t_instant *instant)
{
	t_instant	now;
	int			delta_secs;
	int			delta_us;

	now = instant_now();
	delta_secs = now.tv_sec - instant->tv_sec;
	delta_us = now.tv_usec - instant->tv_usec;
	return ((t_duration){.micros = delta_secs * 1000000 + delta_us});
}

t_duration	duration_difference(const t_instant *to, const t_instant *from)
{
	int	delta_secs;
	int	delta_us;

	delta_secs = to->tv_sec - from->tv_sec;
	delta_us = to->tv_usec - from->tv_usec;
	return ((t_duration){.micros = delta_secs * 1000000 + delta_us});
}

t_u32	timestamp_ms(t_instant instant, t_instant start)
{
	t_duration	duration;

	duration = duration_difference(&instant, &start);
	return (duration.micros / 1000);
}

void	checked_sleep(t_u32 us)
{
	t_instant	start;

	start = instant_now();
	while (duration_since(&start).micros < us)
	{
		usleep(250);
	}
}
