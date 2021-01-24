/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/24 03:51:08 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int		get_timestamp(const struct timeval *time_start, const struct timeval *time_end)
{
	struct timeval	curr_time;
	struct timeval	timestamp;

	if (time_end == NULL)
	{
		gettimeofday(&curr_time, NULL);
		timestamp.tv_sec = curr_time.tv_sec - time_start->tv_sec;
		timestamp.tv_usec = curr_time.tv_usec - time_start->tv_usec;
	}
	else
	{
		timestamp.tv_sec = time_end->tv_sec - time_start->tv_sec;
		timestamp.tv_usec = time_end->tv_usec - time_start->tv_usec;
	}
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

t_philo_data	*take_philo_params(char **av, int ac)
{
	t_philo_data	*new;
	int				total_philos;

	total_philos = ft_atoi(av[1]);
	if (!(new = (t_philo_data *)malloc(sizeof(t_philo_data))))
		return (NULL);
	new->total_philos = ft_atoi(av[1]);
	new->time_to_die = ft_atoi(av[2]);
	new->time_to_eat = ft_atoi(av[3]);  
	new->time_to_sleep = ft_atoi(av[4]);
	new->nb_eat = ((ac == 6) ? ft_atoi(av[5]) : -1);
	new->fork_num = new->total_philos;
	if (!(new->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * new->fork_num))) // общее количество вилок
		return (NULL);
	gettimeofday(&new->time_start, NULL);
	return (new);
}

int					philo_actions(t_ph_params *ph)
{
	pthread_mutex_lock(&ph->data->fork[ph->hand[LEFT]]);
	pthread_mutex_lock(&ph->data->fork[ph->hand[RIGHT]]);
	printf("%ld %d has taken a fork\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	printf("%ld %d is eating\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	gettimeofday(&ph->last_meal, NULL);
	usleep(ph->data->time_to_eat * 1000);
	pthread_mutex_unlock(&ph->data->fork[ph->hand[LEFT]]);
	pthread_mutex_unlock(&ph->data->fork[ph->hand[RIGHT]]);
	printf("%ld %d is sleeping\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	usleep(ph->data->time_to_sleep * 1000);
	printf("%ld %d is thinking\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
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

t_ph_params		*init_philos(t_philo_data *ph)
{
	int			i;
	t_ph_params	*philo;
	
	if (!(philo = (t_ph_params *)malloc(sizeof(t_ph_params) * ph->total_philos)))
		return (NULL); // вывод ошибки
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
	while (i < ph->fork_num)
		if (pthread_mutex_init(&ph->fork[i++], NULL)) 
			return (NULL); // вывод ошибки
	return (philo);
}

int				run_philos(t_philo_data *ph, t_ph_params *philo)
{
	pthread_t		*thread;
	int				i;

	if (!(thread = (pthread_t *)malloc(sizeof(pthread_t) * ph->total_philos)))
		return (0); // TODO подумать как лучше реализовать return 
	i = 0;
	while (i < ph->total_philos)
	{
		if (i % 2 == 0)
		{
			usleep(10);
			if (pthread_create(&thread[i], NULL, start_philos, &philo[i]))
				return (0); // вывод ошибки
			pthread_detach(thread[i]);
		}
		i++;
	}
	i = 0;
	usleep(1000);
	while (i < ph->total_philos)
	{
		if (i % 2 == 1)
		{
			usleep(10);
			if (pthread_create(&thread[i], NULL, start_philos, &philo[i]))
				return (0); // вывод ошибки
			pthread_detach(thread[i]);
		}
		i++;
	}
	return (0);
}

int				start_proc(t_philo_data *ph)
{
	t_ph_params		*philo;
	int				i;
	int				last_meal;

	if (!(philo = init_philos(ph)))
		return (0); // вывод ошибок
	run_philos(ph, philo);
	while (1)
	{
		usleep(100);
		i = 0;
		last_meal = 0;
		while (i < ph->total_philos)
		{
			if (get_timestamp(&philo[i].last_meal, NULL) > ph->time_to_die)
			{
				printf("die i = %d die time = %ld\n", i + 1, get_timestamp(&philo[i].last_meal, NULL));
				return (0);
			}
			if (philo[i].life_status == S_LAST_MEAL)
				last_meal++;
			i++;
		}
		if (last_meal == ph->total_philos - 1)
			break ;
	}
}


int				main(int ac, char **av)
{

	t_philo_data *ph;

	if (ac != 5 && ac != 6)
		return (programm_failed(12, "Not a valid arguments count"));
	if (!(ph = take_philo_params(av, ac)))
		return (programm_failed(1, "Malloc can't allocate memory"));
	start_proc(ph);
	// TODO очистить всю память, уничтожить мьютексы
	return (0);


}