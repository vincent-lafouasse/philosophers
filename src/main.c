/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:19:40 by poss              #+#    #+#             */
/*   Updated: 2025/01/22 19:20:44 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "t_config/t_config.h"
#include "t_philosopher/t_philosopher.h"
#include "t_table/t_table.h"
#include "track/track.h"

static t_error run(t_table* table);

int main(int ac, char* av[]) {
    t_config cfg;
    t_error err;
    t_table table;

    err = load_config(ac, av, &cfg);
    if (err != NO_ERROR) {
        printf("Failed to load config, %s\n", error_repr(err));
        return (EXIT_FAILURE);
    }
    err = table_init(cfg, &table);
    if (err != NO_ERROR) {
        printf("Failed to init state, %s\n", error_repr(err));
        return (EXIT_FAILURE);
    }
    run(&table);
    track_progress(&table);
    table_cleanup(&table);
}

static t_error run(t_table* table) {
    t_u32 i;

    i = 0;
    while (i < table->cfg.n_philosophers) {
        philosopher_start(table->philosophers + i);
        i++;
    }
    return (NO_ERROR);
}
