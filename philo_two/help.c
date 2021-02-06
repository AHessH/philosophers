/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:37:48 by froxanne          #+#    #+#             */
/*   Updated: 2021/02/06 23:53:15 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				ft_atoi(const char *str)
{
	int				is_neg;
	long int		num;
	int				i;

	num = 0;
	is_neg = 1;
	i = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		is_neg = (*str == '-') ? -1 : 1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		num *= 10;
		num += *str++ - '0';
		i++;
	}
	if (i > 10)
		return (is_neg == 1 ? -1 : 0);
	return (num * is_neg);
}

int				ft_strlen(char const *str)
{
	int		len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

int				programm_failed(int code)
{
	printf("[FAIL]: ");
	if (code == ERR_ARG_COUNT)
		printf("Not a valid arguments count\n");
	else if (code == ERR_MALLOC)
		printf("Malloc can't allocate memory\n");
	else if (code == ERR_ARG_VALUE)
		printf("Not a valid argument value\n");
	else if (code == ERR_INIT)
		printf("Impossibly initialize philosophers\n");
	else if (code == ERR_PTHREAD_CREATE)
		printf("Pthread create error\n");
	return (code);
}

long int		get_timestamp(const struct timeval *time_start,
							const struct timeval *time_end)
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

char			*ft_strdup(char const *str)
{
	int		len;
	int		i;
	char	*copy;

	i = 0;
	len = ft_strlen(str);
	if (!(copy = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = 0;
	return (copy);
}
