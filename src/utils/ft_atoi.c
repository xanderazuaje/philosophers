/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 06:39:30 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/07/18 07:17:19 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

int	check_valid_integer(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!isdigit(str[i]))
		{
			write(2, "values must be positive integers without sign.\n",
				47);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long	res;
	size_t	i;

	res = 0;
	i = 0;
	if (!check_valid_integer(str))
		return (-1);
	while (str[i])
	{
		res = (res * 10) + (str[i] - '0');
		if (res > INT_MAX)
		{
			write(2, "parameter(s) exceeds integer\n", 29);
			return (-1);
		}
		i++;
	}
	return (res);
}
