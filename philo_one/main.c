/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/23 02:43:00 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(unsigned int n)
{
	struct timeval	start;
	struct timeval	step;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(10);
		gettimeofday(&step, NULL);
		if ((size_t)(((size_t)(step.tv_sec - start.tv_sec)) * 1000000 +
((size_t)(step.tv_usec - start.tv_usec))) > n)
			break ;
	}
}

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
	// if (timestamp.tv_usec < 0) 
	// {
	// 	timestamp.tv_sec--;
	// 	timestamp.tv_usec += 1000000; 
	// }
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

void				*time_checker(void *philo)
{
	
}

int					print_philo_message(int action, t_ph_params *ph)
{
	if (action == A_EAT)
	{
		printf("%ld %d is eating\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
		ft_usleep(ph->data->time_to_eat * 1000);
		return (ph->data->time_to_eat);
	}
	else if (action == A_SLEEP)
	{
		gettimeofday(&ph->last_meal, NULL);
		printf("%ld %d is sleeping\n", get_timestamp(&ph->data->time_start, NULL), ph->ph_index);
		ft_usleep(ph->data->time_to_sleep * 1000);
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
	pthread_mutex_unlock(&ph->data->fork[hand[LEFT]]); // бросает вилки
	pthread_mutex_unlock(&ph->data->fork[hand[RIGHT]]);
}
// IDEA попробовать создать отдельный поток который будет отслеживать время, ибо это пздц



void				*start_philos(void *philos)
{
	t_ph_params		*ph;
	int				j;
	long int 		die_time; // подсчет времени до смерти
	t_timing		time;

	ph = (t_ph_params *)philos;
	die_time = 0;
	gettimeofday(&ph->last_meal, NULL);
	while (ph->data->nb_eat == -1 ? 1 : j++ < ph->data->nb_eat)
	{
		if (!pthread_mutex_lock(&ph->data->fork[ph->hand[LEFT]]))
			print_philo_message(A_TAKE_FORK, ph);
		// printf("left fort = %d right fork = %d\n", ph->hand[LEFT], ph->hand[RIGHT]);
		if (!pthread_mutex_lock(&ph->data->fork[ph->hand[RIGHT]]))
			print_philo_message(A_TAKE_FORK, ph);
		print_philo_message(A_EAT, ph);
		throw_forks(ph, ph->hand); // бросает вилки
		print_philo_message(A_SLEEP, ph);
		print_philo_message(A_THINK, ph);
	}
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
		printf("i = %d left = %d right = %d\n", i, philo[i].hand[LEFT], philo[i].hand[RIGHT]);
		philo[i].life_status = S_LIFE;
		philo[i].ph_index = i;
		philo[i++].data = ph;
	}
	i = 0;
	while (i < ph->fork_num)
		if (pthread_mutex_init(&ph->fork[i++], NULL)) 
			return (NULL); // вывод ошибки
	return (philo);
}

#ifdef TEST
void	*start_timer(void *time)
{
	int		sec;

	while (sec++ < TEST_TIME)
		ft_usleep(1000000);
	*(int *)time = 1;
}
#endif


int				start_proc(t_philo_data *ph)
{
	pthread_t		*thread;
	t_ph_params		*philo;
	int				i;
	int				last_meal;
	int				j;
	struct timeval	curr_time;
#ifdef TEST
	int				result;
	pthread_t		time_count;
#endif

	thread = NULL;
	if (!(philo = init_philos(ph)))
		return (0); // вывод ошибок
	if (!(thread = (pthread_t *)malloc(sizeof(pthread_t) * ph->total_philos)))
		return (0); // TODO подумать как лучше реализовать return 
	i = 0;
	while (i < ph->total_philos)
	{
		if (i % 2 == 0)
		{
			ft_usleep(10);
			if (pthread_create(&thread[i], NULL, start_philos, &philo[i]))
				return (0); // вывод ошибки
			pthread_detach(thread[i]);
		}
		i++;
	}
	i = 0;
	ft_usleep(1000);
	while (i < ph->total_philos)
	{
		if (i % 2 == 1)
		{
			if (pthread_create(&thread[i], NULL, start_philos, &philo[i]))
				return (0); // вывод ошибки
			pthread_detach(thread[i]);
		}
		i++;
	}
#ifdef TEST
	result = 0;
	if (pthread_create(&time_count, NULL, start_timer, &result))
				return (0);
	pthread_detach(time_count);
#endif
	while (1)
	{
		ft_usleep(100);
		i = 0;
		last_meal = 0;
		gettimeofday(&curr_time, NULL);
		while (i < ph->total_philos)
		{
			if (get_timestamp(&philo[i].last_meal, &curr_time) > ph->time_to_die)
			{
				printf("die i = %d \n", i);
				return (0);
			}
			if (philo[i].life_status == S_LAST_MEAL)
				last_meal++;
			if (last_meal == ph->total_philos - 1)
				break ;
#ifdef TEST
			if (result == 1)
				return (0);
#endif
			i++;
		}
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