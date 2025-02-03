/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_table.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:53:17 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 18:54:01 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_TABLE_H
#define T_TABLE_H

#include <stdbool.h>
#include "t_big_red_button.h"
#include "t_message_queue/t_message_queue.h"
#include "t_philosopher/t_philosopher.h"

typedef struct s_table {
    t_philosopher* philosophers;
    pthread_mutex_t* forks;
    t_message_queue* messages;
    t_instant simulation_start;
    t_big_red_button* abort_button;
    t_config cfg;
} t_table;

t_error table_init(t_config cfg, t_table* table);
void table_cleanup(t_table* table);

#endif
