/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:04:41 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/16 22:05:47 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	did_someone_died(t_philo *philo)
{
	int	i;

	pthread_mutex_lock(&philo->mutex);
	i = *(philo->someone_died) == 1;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

int	check_state(t_philo *philo, t_states state)
{
	time_t	i;

	pthread_mutex_lock(&philo->mutex);
	i = philo->state == state;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

int	can_eat(t_philo *philo)
{
	time_t	i;
	int		l_is_taken;
	int		r_is_taken;

	pthread_mutex_lock(&philo->left_fork->mutex);
	l_is_taken = philo->left_fork->philo == philo;
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	r_is_taken = philo->right_fork->philo == philo;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	i = l_is_taken && r_is_taken
		&& philo->state != waiting && philo->left_fork != philo->right_fork;
	if (philo->max_eat_count > -1
		&& philo->total_eat_count >= philo->max_eat_count)
		return (-1);
	return (i);
}
