/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_big_red_button.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:52:56 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 18:54:14 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_BIG_RED_BUTTON_H
# define T_BIG_RED_BUTTON_H

# include "t_error/t_error.h"
# include <pthread.h>
# include <stdbool.h>

typedef struct s_big_red_button
{
	bool			abort;
	pthread_mutex_t	abort_guard;
}					t_big_red_button;

t_error				big_red_button_init(t_big_red_button *out);
bool				must_abort(t_big_red_button *button);
void				big_red_button_press(t_big_red_button *button);

#endif
