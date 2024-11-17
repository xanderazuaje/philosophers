/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:22:42 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/17 13:31:38 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "../philo.h"

void	fake_sleep(t_philo *philo, time_t time)
{
	time_t	started;

	started = get_time();
	while (get_time() - started < time)
	{
		if (must_die(philo))
		{
			kill(philo);
			return;
		}
		usleep(1);
	}
}
