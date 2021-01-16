/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:26 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/16 15:09:03 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	new->time_to_die = ft_atoi(av[3]);  
	new->time_to_sleep = ft_atoi(av[4]);
	new->nb_eat = ((ac == 6) ? ft_atoi(av[5]) : -1);
	new->fork_num = new->total_philos;
	if (!(new->fork = (t_fork *)malloc(sizeof(t_fork) * new->fork_num))) // общее количество вилок
		return (NULL);
	i = 0;
	while (i < new->fork_num)
		if (!(new->fork[i++].fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t))))
			return (NULL);
	return (new);
}


int					philo_action(int action, int time)
{
	if (action == EAT)
	{
		printf("is eating\n");
		usleep(time);
	}
	else if (action == SLEEP)
	{
		printf("is sleeping\n");
		usleep(time);
		return (time);
	}
	else if (action == THINK)
	{
		printf("is thinking");
	}
	return (0);
}

void				throw_forks(t_philo_data *ph, int *hand)
{
	pthread_mutex_unlock(ph->fork[hand[LEFT]].fork); // бросает вилки
	pthread_mutex_unlock(ph->fork[hand[RIGHT]].fork);
	ph->fork[hand[LEFT]].status = FREE;
	ph->fork[hand[RIGHT]].status = FREE;
}

void				*start_philos(void *philos)
{
	t_philo_data	*ph;
	int				i;
	int				j;
	int				hand[2];
	int64_t 		die_time; // подсчет времени до смерти
/*
** Подумать где разместить смену статуса на dead и last_meal
*/
	ph = (t_philo_data *)philos;
	while (ph->nb_eat == -1 ? 1 : j++ < ph->nb_eat) // IDEA так же добавить в условие, если не получилось залочить статусный мьютекс, если не получатся статусы
	{
		hand[LEFT] = -1;
		hand[RIGHT] = -1;
		i = 0;
		while (hand[LEFT] == -1 || hand[RIGHT] == -1) // берет свободные вилки в обе руки
		{
			printf("in while hands\n");
			if (ph->fork->status == FREE)
			{
				printf("in if hands\n");
				pthread_mutex_lock(ph->fork[i].fork);
				if (hand[LEFT] == -1)
					hand[LEFT] = i;
				else if (hand[RIGHT] == -1)
					hand[RIGHT] = i;
			}
			// если время до смерти больше чем time_to_die то философ должен умереть. все должно быть в цикле
			i = ((i < ph->fork_num) ? i + 1 : 0); 
		} //
		philo_action(EAT, ph->time_to_eat);
		throw_forks(ph, hand); // бросает вилки
		printf("after unlock\n");
		if ((die_time += philo_action(SLEEP, ph->time_to_sleep)) > ph->time_to_die)
		{
			//philo is dead
			printf("philo_is_dead\n");
		}
		printf("die time = %ld\n", die_time);
		philo_action(THINK, 0);
	}
	return (NULL);
}

int				init_philos(t_philo_data *ph, pthread_t *thread)
{
	int			i;
	t_ph_params	*philos;
	
	if (!(philos = (t_ph_params *)malloc(sizeof(t_ph_params) * ph->total_philos)))
		return (1);
	i = 0;
	while (i < ph->total_philos)
	{
		philos[i].life_status = LIFE;
		philos[i].ph_index = i + 1;
		i++;
	}
	if (!(thread = (pthread_t *)malloc(sizeof(pthread_t) * ph->total_philos)))
		return (1); // TODO подумать как лучше реализовать return 
	i = 0;
	while (i < ph->fork_num)
	{
		if (pthread_mutex_init(ph->fork[i].fork, NULL)) 
			return (2);
		ph->fork[i++].status = FREE;
	}
	i = 0; // IDEA после создания мьютексов, каждый экземляр философа должен указывать на те же мьютексы
	while (i < ph->total_philos)
		pthread_create(&thread[i++], NULL, start_philos, ph);
		// if (pthread_create(&thread[i++], NULL, start_philos, ph))
		// 	return (1);
	return (0);
}

/* IDEA
**  Использую отдельную структуру с номером и статусом философа, когда филосов умирает, или поел последний раз
**	статус меняется на соответствующий и в мейне программа завершается
**	Если не получится с вилками так, то поступить по аналогии с философами, завести статус вилки и проверять данный статус 
*/

int				start_proc(t_ph_params *philos)
{
	pthread_t		*thread;
	int				err_code;
	int				i;

	thread = NULL;// TODO добавить инициализацию номеров философов и статус жизни
	
	if (!(err_code = init_philos(philos, thread)))
		return (err_code);
	i = 0;
	i = 0;
	while (i < philos->data->total_philos)
	{
		printf("in detach\n");
		pthread_detach(thread[i]); // IDEA поэксперементировать со статусами
		i++;
	}
	i = 0;
	while (i < philos->data->total_philos)
	{
		if (philos[i].life_status == LIFE)
			i = ((i < philos->data->total_philos) ? i + 1 : 0); 
		else // TODO реализовать вывод die
		{
			pritnf("dead\n");
			break ;
		}
	}
	return (0);
}

int				main(int ac, char **av)
{
	t_ph_params *ph;

	printf("ac = %d\n", ac);
	if (ac != 5 && ac != 6)
		return (programm_failed(12, "Not a valid arguments count"));
	if (!(ph = take_philo_params(av, ac)))
		return (programm_failed(1, "Malloc can't allocate memory"));
	start_proc(ph);
	// TODO очистить всю память, уничтожить мьютексы
	return (0);
}