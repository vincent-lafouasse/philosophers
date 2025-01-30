/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_error.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poss <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:45:26 by poss              #+#    #+#             */
/*   Updated: 2025/01/30 14:45:42 by poss             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_ERROR_H
# define T_ERROR_H

typedef enum e_error
{
	NO_ERROR,
	E_OOM,
	E_PTHREAD_CREATE,
	E_PTHREAD_JOIN,
	E_MUTEX_INIT,
	E_NOT_A_NUMBER,
	E_OVERFLOW,
	E_BADUSAGE,
	E_INSUFFICIENT_TIME,
	E_TOO_MANY_THREADS,
}			t_error;

const char	*error_repr(t_error err);

#endif
