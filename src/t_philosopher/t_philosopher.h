/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_philosopher.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:02:46 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:05:06 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_PHILOSOPHER_H
# define T_PHILOSOPHER_H

# include "t_config/t_config.h"
# include "t_error/t_error.h"
# include "t_table/t_big_red_button.h"
# include <pthread.h>

struct s_message_queue;
typedef struct s_message_queue	t_message_queue;

typedef enum e_state			t_state;
enum							e_state
{
	THINKING,
	FORK_HANDED1,
	FORK_HANDED2,
	EATING,
	SLEEPING,
	DEAD,
};

typedef struct s_philosopher	t_philosopher;
struct							s_philosopher
{
	pthread_t					thread;
	t_u32						index;
	pthread_mutex_t				*first_fork;
	pthread_mutex_t				*second_fork;
	t_state						state;
	t_message_queue				*messages;
	t_big_red_button			*abort_button;
	t_config					cfg;
};

typedef struct s_table			t_table;

t_philosopher					philosopher_new(t_u32 index, t_table *table);
t_error							philosopher_start(t_philosopher *self);

#endif
