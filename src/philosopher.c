/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:56:45 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/16 21:59:11 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	try_to_eat(t_philo *philo)
{
	int	can_eat_now;

	grab_forks_if_can(philo);
	can_eat_now = can_eat(philo);
	if (can_eat_now == 1)
		eat(philo);
	else if (can_eat_now == -1)
	{
		throw_forks(philo);
		return (0);
	}
	return (1);
}

void	init_philo(void *params, t_philo **philo)
{
	*philo = (t_philo *) params;
	(*philo)->time_since_eat = get_time();
	(*philo)->state = thinking;
	pthread_mutex_init(&(*philo)->mutex, NULL);
}

void	*philosopher(void *params)
{
	t_philo		*philo;

	init_philo(params, &philo);
	if (philo->number % 2 == 0)
		fake_sleep(philo->times.eat);
	while (!did_someone_died(philo))
	{
		if (must_die(philo))
		{
			kill(philo);
			break ;
		}
		if (check_state(philo, thinking))
			think(philo);
		else if (!did_someone_died(philo) && check_state(philo, eating))
		{
			if (!try_to_eat(philo))
				return (NULL);
		}
		else if (!did_someone_died(philo) && check_state(philo, sleeping))
			sleep_philo(philo);
	}
	return (pthread_mutex_destroy(&philo->mutex), NULL);
}
