/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_stuff.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:10:06 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/14 11:18:08 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_forks(t_fork **forks, const int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&(*forks)[i].mutex);
		i++;
	}
	return (1);
}

void	throw_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->left_fork->philo = NULL;
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->right_fork->philo = NULL;
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

int	is_fork_used(t_fork *fork)
{
	int i;

	pthread_mutex_lock(&fork->mutex);
	i = fork->philo == NULL;
	pthread_mutex_unlock(&fork->mutex);

	return (i);
}

void	grab_forks_if_can(t_philo *philo)
{
	if (is_fork_used(philo->left_fork) && !did_someone_died(philo))
	{
		pthread_mutex_lock(philo->print_mutex);
		pthread_mutex_unlock(philo->print_mutex);
		pthread_mutex_lock(&philo->left_fork->mutex);
		philo->left_fork->philo = philo;
		pthread_mutex_unlock(&philo->left_fork->mutex);
	}
	if (is_fork_used(philo->right_fork) && !did_someone_died(philo))
	{
		pthread_mutex_lock(philo->print_mutex);
		pthread_mutex_unlock(philo->print_mutex);
		pthread_mutex_lock(&philo->right_fork->mutex);
		philo->right_fork->philo = philo;
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
}
