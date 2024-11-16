/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:22:42 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/14 11:25:31 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "utils.h"

void	fake_sleep(time_t time)
{
	time_t	started;

	started = get_time();
	while (get_time() - started < time)
	{
		usleep(1);
	}
}

