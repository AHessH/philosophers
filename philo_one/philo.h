/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:33:09 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/20 00:12:35 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PHILO_H
# define _PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <stdio.h> // DELETE

#define	LEFT				0
#define RIGHT				1


typedef enum				e_action_list
{
		A_DIE		=		0,
		A_EAT		=		1,
		A_SLEEP		=		2,
		A_THINK		=		3,
		A_TAKE_FORK	=		4
}							t_action_list;

typedef enum				e_life_status
{
		S_DIE		=		0,
		S_LAST_MEAL	=		1,
		S_LIFE		=		2
}							t_life_status;

typedef struct				s_fork
{
		pthread_mutex_t		*fork;
}							t_fork;


typedef struct				s_timing
{
		struct timeval		start;
		struct timeval		end;
}							t_timing;


typedef struct				s_philo_data // общие данные
{
		int					total_philos;
		int					time_to_die;
		int					time_to_eat;
		int					time_to_sleep;
		int					nb_eat;
		int					fork_num;
		struct timeval		time_start;
		t_fork				*fork; // TODO удалить стрктуру изменить на мьютекс
}							t_philo_data;

typedef struct				s_ph_params // у каждого совй
{
		t_philo_data		*data;
		int					hand[2];
		t_life_status		life_status;
		int					ph_index;
}							t_ph_params;



int							ft_strlen(char const *str);
int							programm_failed(int code, const char *message);

int							ft_atoi(const char *str);
t_philo_data				**take_philo_data(char **av, int ac);


#endif