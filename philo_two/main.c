/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:32:36 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/20 00:10:52 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

while ((hand[LEFT] == -1 || hand[RIGHT] == -1)) // TODO не забыть про &&
		{
			printf("ph = %d, i = %d\n", i);
			if (!pthread_mutex_lock(ph->data->fork[i].fork))
			{
				philo_action(A_TAKE_FORK, ph);
				if (hand[LEFT] == -1)
					hand[LEFT] = i;
				else if (hand[RIGHT] == -1)
					hand[RIGHT] = i;
			}
			// если время до смерти больше чем time_to_die то философ должен умереть. все должно быть в цикле
			i = ((i < ph->data->fork_num) ? i + 1 : 0);
			gettimeofday(&time.end, NULL);// TODO тут установить время конца цикла и проверять
			if (get_timestamp(&time.start, &time.end) > ph->data->time_to_die)// TODO здесь проверять сколько времени философ не может взять вилки
			{
				printf("tyt1\n"); 
				return ((void *)philo_action(A_DIE, ph));
			}
		}
/* для того случая когда вилки лежат в центре