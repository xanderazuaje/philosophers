/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 05:31:45 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/16 22:13:13 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_structs(t_fork **forks, pthread_t **threads, const int n)
{
	int	i;

	i = 0;
	*forks = (t_fork *) malloc(n * sizeof(t_fork));
	if (*forks == NULL)
		return (0);
	while (i < n)
	{
		(*forks)[i].philo = NULL;
		pthread_mutex_init(&(*forks)[i].mutex, NULL);
		i++;
	}
	*threads = (pthread_t *) malloc(n * sizeof(pthread_t));
	if (*threads == NULL)
	{
		free(*forks);
		return (0);
	}
	return (1);
}

void	create_threads(int total, pthread_t *philos_thread, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < total)
	{
		pthread_create(&philos_thread[i], NULL, philosopher, &philos[i]);
		i++;
	}
	i = 0;
	while (i < total)
	{
		pthread_join(philos_thread[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int				total_philos;
	t_program		program;

	total_philos = parse_input(argc, argv, &program);
	program.died = 0;
	if (total_philos == 0)
		return (1);
	if (!init_structs(&program.forks, &program.philos_thread, total_philos))
		return (1);
	pthread_mutex_init(&program.print_mutex, NULL);
	assign_forks(total_philos, program.philos, program.forks);
	program.started = get_time();
	create_threads(total_philos, program.philos_thread, program.philos);
	destroy_forks(&program.forks, total_philos);
	pthread_mutex_destroy(&program.print_mutex);
	free(program.philos);
	free(program.forks);
	free(program.philos_thread);
	return (0);
}
