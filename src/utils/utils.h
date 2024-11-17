/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 06:42:10 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/17 13:45:10 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <sys/time.h>
# include <unistd.h>
# include <ctype.h>
# include <limits.h>
# include "../types.h"

int		ft_atoi(const char *str);
time_t	get_time(void);
void	fake_sleep(t_philo *philo, time_t time);

#endif
