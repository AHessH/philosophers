/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:37:48 by froxanne          #+#    #+#             */
/*   Updated: 2021/01/10 16:24:12 by froxanne         ###   ########.fr       */
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

int		ft_strlen(char const *str)
{
	int len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}


int			programm_failed(int code, const char *message)
{
	write(1, message, ft_strlen(message));
	write(1, "\n", 1);
	return (code);	
}
