/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/02/07 00:11:05 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo_data		*take_philo_params(char **av, int ac)
{
	t_philo_data	*new;

	if (!(new = (t_philo_data *)malloc(sizeof(t_philo_data))))
		return (NULL);
	new->total_philos = ft_atoi(av[1]);
	new->time_to_die = ft_atoi(av[2]);
	new->time_to_eat = ft_atoi(av[3]);
	new->time_to_sleep = ft_atoi(av[4]);
	new->nb_eat = ((ac == 6) ? ft_atoi(av[5]) : -1);
	new->fork_num = new->total_philos;
	gettimeofday(&new->time_start, NULL);
	if (!(new->thread = (pthread_t *)
				malloc(sizeof(pthread_t) * new->total_philos)))
		return (NULL);
	if (!(new->sem_name = ft_strdup("forks")))
		return (NULL);
	return (new);
}

static int				start_proc(t_philo_data *ph, t_ph_params *philo)
{
	int				i;
	int				last_meal;

	if (!(run_philos(ph, philo)))
		return (ERR_PTHREAD_CREATE);
	while (1)
	{
		usleep(100);
		i = -1;
		last_meal = 0;
		while (++i < ph->total_philos)
		{
			if (get_timestamp(&philo[i].last_meal, NULL) > ph->time_to_die)
			{
				printf("die i = %d die time = %ld\n", i + 1,
							get_timestamp(&philo[i].last_meal, NULL));
				return (0);
			}
			if (philo[i].life_status == S_LAST_MEAL)
				last_meal++;
		}
		if (last_meal == ph->total_philos - 1)
			return (0);
	}
	return (0);
}

static void				clean_resourses(t_philo_data *data, t_ph_params *philo)
{
	if (data)
	{
		if (data->sem_name)
			free(data->sem_name);
		if (data->thread)
			free(data->thread);
		if (data->fork)
		{
			sem_close(data->fork);
			sem_unlink(data->sem_name);
		}
		free(data);
	}
	if (philo)
		free(philo);
}

int						check_args_value(char **av, int ac)
{
	int				i;
	char			*str;

	i = 1;
	while (i < ac)
	{
		str = av[i];
		while (*str)
		{
			if (*str < 48 || *str > 57)
				return (0);
			str++;
		}
		i++;
	}
	if (ft_atoi(av[1]) <= 1 || ft_atoi(av[2]) <= 0
			|| ft_atoi(av[3]) <= 0 || ft_atoi(av[4]) <= 0)
		return (0);
	if (ac == 6 && ft_atoi(av[5]) <= 0)
		return (0);
	return (1);
}

int						main(int ac, char **av)
{
	t_philo_data	*ph;
	t_ph_params		*philo;
	int				err;

	if (ac != 5 && ac != 6)
		return (programm_failed(ERR_ARG_COUNT));
	if (!(check_args_value(av, ac)))
		return (programm_failed(ERR_ARG_VALUE));
	if (!(ph = take_philo_params(av, ac)))
		return (programm_failed(ERR_MALLOC));
	if (!(philo = init_philos(ph)))
		return (ERR_INIT);
	if ((err = start_proc(ph, philo)))
		return (programm_failed(err));
	clean_resourses(ph, philo);
	return (0);
}
