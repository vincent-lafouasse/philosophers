/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_message_queue.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:34:02 by poss              #+#    #+#             */
/*   Updated: 2025/01/12 15:53:46 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "time/ft_time.h"
#include "t_philosopher/t_philosopher.h"
#include "cool_ints.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct s_message		t_message;
struct							s_message
{
	t_state						state;
	t_instant					timestamp;
	t_u32							index;
	t_message					*next;
	t_message					*prev;
};

typedef struct s_message_queue	t_message_queue;
struct							s_message_queue
{
	t_message					*head;
	t_message					*tail;
	pthread_mutex_t				guard;
};

t_error							mq_new(t_message_queue *out);
t_error							mq_push(t_message_queue *mq, t_state state,
									t_u32 index);
t_message						*mq_pop(t_message_queue *mq);
