/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:50:11 by poss              #+#    #+#             */
/*   Updated: 2025/01/12 15:52:59 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/time.h>
#include "cool_ints.h"

typedef struct s_duration	t_duration;
struct						s_duration
{
	t_u32					micros;
};

typedef struct timeval		t_instant;

t_instant					instant_now(void);
t_duration					duration_since(const t_instant *instant);
t_duration					duration_difference(const t_instant *to,
								const t_instant *from);
t_u32						timestamp_ms(t_instant instant, t_instant start);
void						checked_sleep(t_u32 us);
