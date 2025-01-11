/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_config.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:36:24 by poss              #+#    #+#             */
/*   Updated: 2025/01/11 17:32:51 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_config.h"
#include <stdio.h>
#include <stdlib.h>

t_error			checked_atou(const char *s, t_u32 *out);

static t_error	load_timers(char *av[], t_config *cfg)
{
	t_error	err;

	err = checked_atou(av[1], &cfg->n_philosophers);
	if (err != NO_ERROR)
		return (err);
	if (cfg->n_philosophers == 1)
	{
		printf("todo\n");
		exit(1);
	}
	err = checked_atou(av[2], &cfg->time_to_die_ms);
	if (err != NO_ERROR)
		return (err);
	err = checked_atou(av[3], &cfg->time_to_eat_ms);
	if (err != NO_ERROR)
		return (err);
	err = checked_atou(av[4], &cfg->time_to_sleep_ms);
	return (err);
}

t_error	load_config(int ac, char *av[], t_config *cfg)
{
	t_error	err;

	if (ac != 5 && ac != 6)
		return (E_BADUSAGE);
	err = load_timers(av, cfg);
	if (err != NO_ERROR)
		return (err);
	if (ac == 6)
	{
		err = checked_atou(av[5], &cfg->n_meals);
		if (err != NO_ERROR)
			return (err);
		cfg->track_meals = true;
	}
	else
		cfg->track_meals = false;
	return (NO_ERROR);
}

void	log_config(t_config cfg)
{
	printf("config:\n");
	printf("%u diners\n", cfg.n_philosophers);
	printf("life expectancy %u ms\n", cfg.time_to_die_ms);
	printf("dinner length %u ms\n", cfg.time_to_eat_ms);
	printf("nap length %u ms\n", cfg.time_to_sleep_ms);
	if (cfg.track_meals)
		printf("n meals %u\n", cfg.n_meals);
	printf("\n");
}
