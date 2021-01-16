/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:33:09 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/16 15:05:25 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PHILO_H
# define _PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include <stdio.h> // DELETE

#define	LEFT	0
#define RIGHT	1



#define EAT		1
#define SLEEP	2
#define THINK	3

long long int				g_time_start;

typedef enum				e_fork_status
{
		FREE		=		1,
		LOCK		=		0
}							t_fork_status;

typedef enum				e_life_status
{
		DIE			=		0,
		LAST_MEAL	=		1,
		LIFE		=		2
}							t_life_status;

typedef struct	s_fork
{
		pthread_mutex_t		*fork;
		t_fork_status		status;
}				t_fork;


typedef struct				s_philo_data // общие данные
{
		int					total_philos;
		int					time_to_die;
		int					time_to_eat;
		int					time_to_sleep;
		int					nb_eat;
		int					fork_num;
		t_fork				*fork; // TODO удалить нижнюю строку и заменить на данную структуру, изменить инит
}							t_philo_data;

typedef struct				s_ph_params // у каждого совй
{
		t_philo_data		*data;
		t_life_status		life_status;
		int					ph_index;
}							t_ph_params;



int							ft_strlen(char const *str);
int							programm_failed(int code, const char *message);

int							ft_atoi(const char *str);
t_philo_data				**take_philo_data(char **av, int ac);


#endif