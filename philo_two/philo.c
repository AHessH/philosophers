/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 14:50:38 by froxanne          #+#    #+#             */
/*   Updated: 2021/02/09 23:03:10 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int			philo_actions(t_ph_params *ph)
{
	sem_wait(ph->data->fork);
	sem_wait(ph->data->fork);
	printf("%ld %d has taken a fork\n",
			get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	printf("%ld %d is eating\n",
			get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	gettimeofday(&ph->last_meal, NULL);
	usleep(ph->data->time_to_eat * 1000);
	sem_post(ph->data->fork);
	sem_post(ph->data->fork);
	printf("%ld %d is sleeping\n",
			get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	usleep(ph->data->time_to_sleep * 1000);
	printf("%ld %d is thinking\n",
			get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	return (0);
}

void				*start_philos(void *philos)
{
	t_ph_params		*ph;
	int				j;

	ph = (t_ph_params *)philos;
	j = 0;
	gettimeofday(&ph->last_meal, NULL);
	while (1)
	{
		philo_actions(ph);
		if (ph->data->nb_eat != -1 && ++j >= ph->data->nb_eat)
			ph->life_status = S_LAST_MEAL;
	}
	return (NULL);
}

t_ph_params			*init_philos(t_philo_data *ph)
{
	int			i;
	t_ph_params	*philo;

	if (!(philo = (t_ph_params *)
			malloc(sizeof(t_ph_params) * ph->total_philos)))
		return (NULL);
	i = 0;
	while (i < ph->total_philos)
	{
		philo[i].hand[LEFT] = i;
		philo[i].hand[RIGHT] = (i <= 0) ? ph->total_philos - 1 : i - 1;
		philo[i].life_status = S_LIFE;
		philo[i].ph_index = i + 1;
		philo[i++].data = ph;
	}
	i = 0;
	if ((ph->fork = sem_open(ph->sem_name,
				O_CREAT | O_EXCL, O_RDWR, ph->fork_num)) == SEM_FAILED)
	{
		sem_unlink(ph->sem_name);
		ph->fork = sem_open(ph->sem_name, O_CREAT | O_EXCL, O_RDWR, ph->sem_name);
	}
	return (philo);
}

int					run_philos(t_philo_data *ph, t_ph_params *philo)
{
	int				i;

	i = -1;
	while (++i < ph->total_philos)
		if (i % 2 == 0)
		{
			usleep(10);
			if (pthread_create(&ph->thread[i], NULL, start_philos, &philo[i]))
				return (0);
			pthread_detach(ph->thread[i]);
		}
	i = -1;
	usleep(1000);
	while (++i < ph->total_philos)
		if (i % 2 == 1)
		{
			usleep(10);
			if (pthread_create(&ph->thread[i], NULL, start_philos, &philo[i]))
				return (0);
			pthread_detach(ph->thread[i]);
		}
	return (1);
}
