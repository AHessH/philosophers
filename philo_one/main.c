/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/20 01:00:43 by froxanne         ###   ########.fr       */
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
	if (timestamp.tv_usec < 0) 
	{
		timestamp.tv_sec--;
		timestamp.tv_usec += 1000000; 
	}
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

t_philo_data	*take_philo_params(char **av, int ac)
{
	t_philo_data	*new;
	int				total_philos;
	int				i;

	total_philos = ft_atoi(av[1]);
	if (!(new = (t_philo_data *)malloc(sizeof(t_philo_data))))
		return (NULL);
	new->total_philos = ft_atoi(av[1]);
	new->time_to_die = ft_atoi(av[2]);
	new->time_to_eat = ft_atoi(av[3]);  
	new->time_to_sleep = ft_atoi(av[4]);
	new->nb_eat = ((ac == 6) ? ft_atoi(av[5]) : -1);
	new->fork_num = new->total_philos;
	if (!(new->fork = (t_fork *)malloc(sizeof(t_fork) * new->fork_num))) // общее количество вилок
		return (NULL);
	i = 0;
	while (i < new->fork_num)
		if (!(new->fork[i++].fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t))))
			return (NULL);
	gettimeofday(&new->time_start, NULL);
	return (new);
}


int					print_philo_message(int action, t_ph_params *ph)
{
	if (action == A_EAT)
	{
		printf("%ld %d is eating\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
		usleep(ph->data->time_to_eat * 1000);
	}
	else if (action == A_SLEEP)
	{
		printf("%ld %d is sleeping\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
		usleep(ph->data->time_to_sleep * 1000);
		return (ph->data->time_to_sleep);
	}
	else if (action == A_THINK)
	{
		printf("%ld %d is thinking\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	}
	else if (action == A_TAKE_FORK)
	{
		printf("%ld %d has taken a fork\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
	}
	else if (action == A_DIE)
	{
		ph->life_status = S_DIE;
		printf("%ld %i died\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
		return (0);
	}
	return (0);
}

void				throw_forks(t_ph_params *ph, int *hand)
{
	pthread_mutex_unlock(ph->data->fork[hand[LEFT]].fork); // бросает вилки
	pthread_mutex_unlock(ph->data->fork[hand[RIGHT]].fork);
}

void				*start_philos(void *philos)
{
	t_ph_params		*ph;
	int				j;
	int				i;
	int64_t 		die_time; // подсчет времени до смерти
	t_timing		time;

	ph = (t_ph_params *)philos;
	die_time = 0;
	while (ph->data->nb_eat == -1 ? 1 : j++ < ph->data->nb_eat)
	{
		i = 0;
		if (!pthread_mutex_lock(ph->data->fork[ph->hand[LEFT]].fork))
			print_philo_message(A_TAKE_FORK, ph);
		if (!pthread_mutex_lock(ph->data->fork[ph->hand[RIGHT]].fork))
			print_philo_message(A_TAKE_FORK, ph);
		// если время до смерти больше чем time_to_die то философ должен умереть. все должно быть в цикле
		if (get_timestamp(&time.start, &time.end) > ph->data->time_to_die)// TODO здесь проверять сколько времени философ не может взять вилки
			return ((void *)print_philo_message(A_DIE, ph));
		die_time = 0;
		if ((die_time += print_philo_message(A_EAT, ph)) > ph->data->time_to_die)
			return ((void *)print_philo_message(A_DIE, ph));
		throw_forks(ph, ph->hand); // бросает вилки
		if ((die_time += print_philo_message(A_SLEEP, ph)) > ph->data->time_to_die)
			return ((void *)print_philo_message(A_DIE, ph));
		print_philo_message(A_THINK, ph);
	}
	if (j >= ph->data->nb_eat)
		ph->life_status = S_LAST_MEAL;
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
		if (pthread_mutex_init(ph->fork[i++].fork, NULL)) 
			return (NULL); // вывод ошибки
	return (philo);
}

int				start_proc(t_philo_data *ph)
{
	pthread_t		*thread;
	t_ph_params		*philo;
	int				i;
	int				last_meal;

	thread = NULL;
	if (!(philo = init_philos(ph)))
		return (0); // вывод ошибок
	if (!(thread = (pthread_t *)malloc(sizeof(pthread_t) * ph->total_philos)))
		return (0); // TODO подумать как лучше реализовать return 
	i = 0;
	while (i < ph->total_philos)
	{
		usleep(2);
		if (pthread_create(&thread[i], NULL, start_philos, &philo[i++]))
			return (0); // вывод ошибки
	}
	i = 0;
	while (i < ph->total_philos)
		pthread_detach(thread[i++]);
	i = 0;
	last_meal = 0;
	while (i < ph->total_philos)
	{
		if (i == 0)
			last_meal = 0;
		if (philo[i].life_status == S_DIE)
			break ;
		if (philo[i].life_status == S_LAST_MEAL)
			last_meal++;
		if (last_meal == ph->total_philos - 1)
			break ;
		//TODO проверку статуса для S_LAST_MEAL
		i = ((i < ph->total_philos - 1) ? i + 1 : 0); 
	}
	return (0);
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