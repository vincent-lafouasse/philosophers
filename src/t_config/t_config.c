/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_config.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:36:24 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 14:47:10 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_config.h"
#include "t_error/t_error.h"
#include <stdio.h>

#ifdef DEBUG
# define VERBOSITY 1
#else
# define VERBOSITY 0
#endif

#define MAX_PHILOS 20000

t_error			checked_atou(const char *s, t_u32 *out);

static t_error	load_timers(char *av[], t_config *cfg)
{
	t_error	err;

	err = checked_atou(av[1], &cfg->n_philosophers);
	if (err != NO_ERROR)
		return (err);
	err = checked_atou(av[2], &cfg->time_to_die_ms);
	if (err != NO_ERROR)
		return (err);
	err = checked_atou(av[3], &cfg->time_to_eat_ms);
	if (err != NO_ERROR)
		return (err);
	err = checked_atou(av[4], &cfg->time_to_sleep_ms);
	return (err);
}

static t_error	validate_config(t_config cfg)
{
	if (cfg.time_to_sleep_ms < 30)
	{
		return (E_INSUFFICIENT_TIME);
	}
	if (cfg.time_to_die_ms < 30)
	{
		return (E_INSUFFICIENT_TIME);
	}
	if (cfg.time_to_eat_ms < 30)
	{
		return (E_INSUFFICIENT_TIME);
	}
	if (cfg.n_philosophers > MAX_PHILOS)
	{
		return (E_TOO_MANY_THREADS);
	}
	return (NO_ERROR);
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
	if (VERBOSITY == 1)
		log_config(*cfg);
	return (validate_config(*cfg));
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
