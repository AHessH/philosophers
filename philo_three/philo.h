/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:33:09 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/26 01:10:32 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PHILO_H
# define _PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>
# include <semaphore.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>

# define LEFT				0
# define RIGHT				1

# define ERR_ARG_COUNT		-1
# define ERR_ARG_VALUE		-2
# define ERR_MALLOC			-3
# define ERR_INIT			-4
# define ERR_PTHREAD_CREATE	-5

typedef enum				e_action_list
{
	A_EAT = 1,
	A_SLEEP = 2,
	A_THINK = 3,
	A_TAKE_FORK = 4,
	A_THROW_FORK = 5
}							t_action_list;

typedef enum				e_life_status
{
	S_DIE = 0,
	S_LAST_MEAL = 1,
	S_LIFE = 2
}							t_life_status;

typedef struct				s_timing
{
	struct timeval			start;
	struct timeval			end;
}							t_timing;

// добавить дополнительный семафор, установить значение в 5 и каждый раз когда философ поел, уменшать
// В мейне цикл, пока семафор полностью не освободится не выходить
// или попробовать с ret_value

typedef struct				s_philo_data
{
	int						total_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						nb_eat;
	int						fork_num;
	struct timeval			time_start;
	char					*sem_name;
	sem_t					*fork;
	pthread_t				*thread;
	pid_t					*pid;
}							t_philo_data;

typedef struct				s_ph_params
{
	t_philo_data			*data;
	int						hand[2];
	t_life_status			life_status;
	struct timeval			last_meal;
	int						ph_index;
}							t_ph_params;

/*
**				HELP
*/
char						*ft_strdup(char const *str);
int							ft_strlen(char const *str);
int							programm_failed(int code);
int							ft_atoi(const char *str);
long int					get_timestamp(const struct timeval *time_start,
								const struct timeval *time_end);

/*
**				PHILO
*/
t_ph_params					*init_philos(t_philo_data *ph);
void						*start_philos(void *philos);
int							run_philos(t_philo_data *ph, t_ph_params *philo);

#endif
