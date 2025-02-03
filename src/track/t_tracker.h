/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_tracker.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:37:16 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:42:59 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TRACKER_H
#define T_TRACKER_H

#include "t_message_queue/t_message_queue.h"

typedef struct s_tracker {
    t_message** last_meals;
    t_u32* n_meals;
} t_tracker;

t_error init_tracker(t_tracker* tracker, t_table* table);
void cleanup_tracker(t_tracker* t, const t_table* table);
void log_death(t_u32 philo, t_instant start, t_instant last_meal);
void log_message(const t_message* message, t_instant start);
bool everyone_is_full(t_u32* n_meals, t_config cfg);
bool everybodys_alive(t_table* table, t_tracker* tracker);
bool must_stop(t_table* table, t_tracker* tracker);

#endif
