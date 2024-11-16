/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 05:32:26 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/16 22:13:23 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include "utils/utils.h"
# include "assert_input/parse_input.h"
# include "types.h"

int		destroy_forks(t_fork **forks, int n);
void	throw_forks(t_philo *philo);
int		is_fork_used(t_fork *fork);
void	grab_forks_if_can(t_philo *philo);
void	assign_forks(int total_philos, t_philo *philos, t_fork *forks);
int		did_someone_died(t_philo *philo);
int		init_structs(t_fork **forks, pthread_t **threads, int n);
void	*eat(void *params);
void	*sleep_philo(void *params);
int		must_die(t_philo *philo);
void	kill(t_philo *philo);
int		check_state(t_philo *philo, t_states state);
int		can_eat(t_philo *philo);
void	*philosopher(void *params);
void	think(t_philo *philo);

#endif
