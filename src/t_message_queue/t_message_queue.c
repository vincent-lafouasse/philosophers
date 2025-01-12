/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_message_queue.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:34:28 by poss              #+#    #+#             */
/*   Updated: 2025/01/12 22:38:57 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error/t_error.h"
#include "t_message_queue.h"
#include <stdio.h>
#include <stdlib.h>

t_error	mq_new(t_message_queue *out)
{
	out->head = NULL;
	out->tail = NULL;
	if (pthread_mutex_init(&out->guard, NULL))
		return (E_MUTEX_INIT);
	return (NO_ERROR);
}

t_error	mq_push(t_message_queue *mq, t_state state, t_u32 index)
{
	t_message	*message;

	message = malloc(sizeof(*message));
	if (!message)
		return (E_OOM);
	*message = (t_message){.state = state, .next = NULL, .prev = NULL,
		.timestamp = instant_now(), .index = index};
	pthread_mutex_lock(&mq->guard);
	if (!mq->head)
	{
		mq->head = message;
		mq->tail = message;
	}
	else
	{
		message->prev = mq->tail;
		mq->tail->next = message;
		mq->tail = message;
	}
	pthread_mutex_unlock(&mq->guard);
	return (NO_ERROR);
}

t_message	*mq_pop(t_message_queue *mq)
{
	t_message	*out;

	pthread_mutex_lock(&mq->guard);
	if (!mq->head)
	{
		pthread_mutex_unlock(&mq->guard);
		return (NULL);
	}
	out = mq->head;
	if (out->next != NULL)
		out->next->prev = NULL;
	mq->head = out->next;
	pthread_mutex_unlock(&mq->guard);
	return (out);
}

void mq_clear(t_message_queue* mq) {
	while (mq->head)
		free(mq_pop(mq));
}
