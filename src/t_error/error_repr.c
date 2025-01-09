/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_repr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:45:24 by poss              #+#    #+#             */
/*   Updated: 2025/01/09 22:25:35 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_error.h"

#define NO_ERROR_REPR "No error"
#define E_OOM_REPR "Out of memory"
#define E_PTHREAD_CREATE_REPR "pthread_create failed"
#define E_PTHREAD_JOIN_REPR "pthread_join failed"
#define E_NOT_A_NUMBER_REPR "Not a number"
#define E_OVERFLOW_REPR "Integer overflow"
#define E_BADUSAGE_REPR "Bad usage"
#define E_MUTEX_INIT_REPR "Failed to init mutex"

const char* error_repr(t_error err) {
    if (err == NO_ERROR)
        return NO_ERROR_REPR;
    if (err == E_OOM)
        return E_OOM_REPR;
    if (err == E_PTHREAD_CREATE)
        return E_PTHREAD_CREATE_REPR;
    if (err == E_PTHREAD_JOIN)
        return E_PTHREAD_JOIN_REPR;
    if (err == E_NOT_A_NUMBER)
        return E_NOT_A_NUMBER_REPR;
    if (err == E_OVERFLOW)
        return E_OVERFLOW_REPR;
    if (err == E_BADUSAGE)
        return E_BADUSAGE_REPR;
    if (err == E_MUTEX_INIT)
        return E_MUTEX_INIT_REPR;
    return "Unreachable";
}
