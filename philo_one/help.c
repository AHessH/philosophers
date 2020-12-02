/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:37:48 by froxanne          #+#    #+#             */
/*   Updated: 2020/12/02 22:45:58 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		ft_strlen(char const *str)
{
	int len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

int			prog_fail(int code, const char const *message)
{
	write(1, "\e[1:1H\e[2JError: ", ft_strlen("\e[1:1H\e[2JError: "));
	write(1, message, ft_strlen(message));
	write(1, "\n", 1);
	return (code);	
}
