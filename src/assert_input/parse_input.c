/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:22:33 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/07/24 17:48:17 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_input.h"

void	parse_mandatory(char **argv, t_philo *philo, int *total, const int argc)
{
	philo->times.die = ft_atoi(argv[2]);
	if (philo->times.die == -1)
		*total = 0;
	philo->times.eat = ft_atoi(argv[3]);
	if (philo->times.eat == -1)
		*total = 0;
	philo->times.sleep = ft_atoi(argv[4]);
	if (philo->times.sleep == -1)
		*total = 0;
	if (argc > 5)
	{
		philo->max_eat_count = ft_atoi(argv[5]);
		if (philo->max_eat_count == -1)
			*total = 0;
	}
	else
		philo->max_eat_count = -1;
}

int	parse_input(int argc, char **argv, t_philo **philos, int *died, pthread_mutex_t *print_mutex)
{
	int	total_philo;
	int	i;

	i = 0;
	if (argc < 5)
	{
		write(2, "invalid input.\n", 15);
		return (0);
	}
	total_philo = ft_atoi(argv[1]);
	if (total_philo == -1)
		total_philo = 0;
	*philos = (t_philo *)malloc(sizeof(t_philo) * total_philo);
	if (*philos == NULL)
		return (0);
	while (i < total_philo && total_philo != 0)
	{
		(*philos)[i].number = i + 1;
		(*philos)[i].someone_died = died;
		parse_mandatory(argv, &(*philos)[i], &total_philo, argc);
		(*philos)[i].print_mutex = print_mutex;
		i++;
	}
	if (total_philo == 0)
		free(*philos);
	return (total_philo);
}
