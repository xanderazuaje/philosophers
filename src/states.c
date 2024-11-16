/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:21:25 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/16 22:14:36 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*eat(void *params)
{
	t_philo	*philo;

	philo = (t_philo *)params;
	if (!did_someone_died(philo))
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%ld: %d is eating\n",
			get_time() - *philo->started, philo->number);
		pthread_mutex_unlock(philo->print_mutex);
	}
	else
		return (NULL);
	pthread_mutex_lock(&philo->mutex);
	philo->time_since_eat = get_time();
	pthread_mutex_unlock(&philo->mutex);
	fake_sleep(philo->times.eat);
	throw_forks(philo);
	pthread_mutex_lock(&philo->mutex);
	philo->state = sleeping;
	if (philo->max_eat_count != -1)
	{
		philo->total_eat_count++;
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void	*sleep_philo(void *params)
{
	t_philo	*philo;

	philo = (t_philo *)params;
	pthread_mutex_lock(philo->print_mutex);
	if (!did_someone_died(philo))
		printf("%ld: %d is sleeping\n",
			get_time() - *philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	fake_sleep(philo->times.sleep);
	pthread_mutex_lock(&philo->mutex);
	philo->state = thinking;
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void	think(t_philo *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	if (!did_someone_died(philo))
		printf("%ld: %d is thinking\n",
			get_time() - *philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	philo->state = eating;
}

int	must_die(t_philo *philo)
{
	time_t	i;

	pthread_mutex_lock(&philo->mutex);
	i = get_time() - philo->time_since_eat > philo->times.die;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

void	kill(t_philo *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	if (!did_someone_died(philo))
		printf("%ld: %d died\n", get_time() - *philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	pthread_mutex_lock(&philo->mutex);
	*philo->someone_died = 1;
	pthread_mutex_unlock(&philo->mutex);
}
