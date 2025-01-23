/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_config.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:36:33 by poss              #+#    #+#             */
/*   Updated: 2025/01/23 14:21:13 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_CONFIG_H
# define T_CONFIG_H

# include "cool_ints.h"
# include "t_error/t_error.h"
# include <stdbool.h>

typedef struct s_config	t_config;
struct					s_config
{
	t_u32				n_philosophers;
	t_u32				time_to_die_ms;
	t_u32				time_to_eat_ms;
	t_u32				time_to_sleep_ms;
	t_u32				n_meals;
	bool				track_meals;
};

t_error					load_config(int ac, char *av[], t_config *cfg);
void					log_config(t_config cfg);

#endif
