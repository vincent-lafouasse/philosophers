/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_error.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:45:26 by poss              #+#    #+#             */
/*   Updated: 2025/01/09 21:45:44 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum e_error {
    NO_ERROR,
    E_OOM,
    E_PTHREAD_CREATE,
    E_PTHREAD_JOIN,
    E_NOT_A_NUMBER,
    E_OVERFLOW,
    E_BADUSAGE,
} t_error;

const char* error_repr(t_error err);
