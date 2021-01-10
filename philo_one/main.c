/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/10 16:32:29 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo_params	*take_philo_params(char **av, int ac)
{
	t_philo_params	*new;

	if (!(new = (t_philo_params *)malloc(sizeof(t_philo_params))))
		return (NULL);
	new->philo_num = ft_atoi(av[1]);
	new->time_to_die = ft_atoi(av[2]);
	new->time_to_die = ft_atoi(av[3]);  
	new->time_to_sleep = ft_atoi(av[4]);
	new->number_of_times_each_ph_must_eat = ((ac == 6) ? ft_atoi(av[5]) : -1);
	new->fork_num = new->philo_num;
	return (new);
}


int					philo_action(int action, int time)
{
	if (action == EAT)
	{
		//вывод действия
		sleep(time);
	}
	else if (action == SLEEP)
	{
		//вывод действия
		sleep(time);
		return (time);
	}
	else if (action == THINK)
	{
		
	}
	return (0);
}

void				*start_philos(void *philos)
{
	t_philo_params	*ph;
	int				i;
	int				j;
	int				hand[2];
	int64_t 		die_time; // подсчет времени до смерти

	while (ph->number_of_times_each_ph_must_eat == -1 ? 1 : j++ < ph->number_of_times_each_ph_must_eat)
	{
		hand[LEFT] = -1;
		hand[RIGHT] = -1;
		ph = (t_philo_params *)philos;
		i = 0;
		while (hand[LEFT] == -1 || hand[RIGHT] == -1) // берет свободные вилки в обе руки
		{
			if (!pthread_mutex_lock(&ph->fork[i]))
			{
				if (hand[LEFT] == -1)
					hand[LEFT] = i;
				else if (hand[RIGHT] == -1)
					hand[RIGHT] = i;
			}
			// если время до смерти больше чем time_to_die то философ должен умереть. все должно быть в цикле
			i = ((i < ph->fork_num) ? i + 1 : 0);
		}
		philo_action(EAT, ph->time_to_eat);
		pthread_mutex_unlock(&ph->fork[hand[LEFT]]); // бросает вилки
		pthread_mutex_unlock(&ph->fork[hand[RIGHT]]);
		if ((die_time += philo_action(SLEEP, ph->time_to_sleep)) > ph->time_to_die)
		{
			//philo is dead
				
		}				
		philo_action(THINK, 0);
	}
	return (NULL);
}

int				init_philos(t_philo_params *ph, pthread_t *thread)
{
	int		i;

	if (!(thread = (pthread_t *)malloc(sizeof(pthread_t) * ph->philo_num)))
		return (1); // TODO подумать как лучше реализовать return 
	i = 0;
	while (i < ph->philo_num)
		if (!pthread_create(&thread[i++], NULL, start_philos, ph))
			return (1);
	i = 0;
	while (i < ph->fork_num)
		if (!pthread_mutex_init(&ph->fork[i++], NULL))
			return (2);
	return (0);
}

int				start_proc(t_philo_params *philos)
{
	pthread_t		*thread;
	int				err_code;

	thread = NULL;
	if (!(err_code = init_philos(philos, thread)))
		return (err_code);
	return (0);
}

int				main(int ac, char **av)
{
	t_philo_params *ph;

	(void)av;
	if (ac != 5 || ac != 6)
		return (programm_failed(12, "Not a valid arguments count"));
	if (!(ph = take_philo_params(av, ac)))
		return (programm_failed(1, "Malloc can't allocate memory"));
	start_proc(ph);
	return (0);
}