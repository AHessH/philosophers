/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:33:09 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/10 15:50:50 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PHILO_H
# define _PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define	LEFT	0
#define RIGHT	1

#define EAT		1
#define SLEEP	2
#define THINK	3

long long int				g_time_start;

typedef struct				s_philo_params
{
		int					philo_num;
		int					time_to_die;
		int					time_to_eat;
		int					time_to_sleep;
		int					number_of_times_each_ph_must_eat;
		int					fork_num;
		pthread_mutex_t		*fork;
}							t_philo_params;


int							ft_strlen(char const *str);
int							programm_failed(int code, const char *message);

int							ft_atoi(const char *str);
t_philo_params				*take_philo_params(char **av, int ac);


#endif