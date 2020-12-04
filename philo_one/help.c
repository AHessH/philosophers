/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 22:37:48 by froxanne          #+#    #+#             */
/*   Updated: 2020/12/03 22:47:20 by froxanne         ###   ########.fr       */
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

int			programm_failed(int code, const char *message)
{
	write(1, message, ft_strlen(message));
	write(1, "\n", 1);
	return (code);	
}
