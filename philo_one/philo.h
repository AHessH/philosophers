/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:33:09 by froxanne          #+#    #+#             */
/*   Updated: 2020/12/04 16:09:18 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PHILO_H
# define _PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_philo_data
{
				int num;
}				t_philo_data;


int				ft_strlen(char const *str);
int				programm_failed(int code, const char *message);

#endif