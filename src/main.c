/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 05:31:45 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/07/24 01:06:51 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_structs(t_fork **forks, pthread_t **threads, const int n)
{
	int i;

	i = 0;
	*forks = (t_fork *) malloc(n * sizeof(t_fork));
	if (*forks == NULL)
		return (0);
	while (i < n)
	{
		(*forks)[i].philo = NULL;
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

time_t get_time()
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void *eat(void *params)
{
	t_philo *philo;
	pthread_mutex_t mutex;

	philo = (t_philo *)params;
	pthread_mutex_init(&mutex, NULL);
	philo->time_since_eat = get_time();
	usleep(philo->times.eat * 1000);
	pthread_mutex_lock(&mutex);
	printf("%ld: %d is eating\n", get_time() - philo->started, philo->number);
	philo->left_fork->philo = NULL;
	philo->right_fork->philo = NULL;
	pthread_mutex_unlock(&mutex);
	philo->state = sleeping;
	return (NULL);
}

void *sleep_philo(void *params)
{
	t_philo	*philo;
	pthread_mutex_t mutex;

	philo = (t_philo *)params;
	pthread_mutex_init(&mutex, NULL);
	usleep(philo->times.sleep * 1000);
	pthread_mutex_lock(&mutex);
	printf("%ld: %d is sleeping\n", get_time() - philo->started, philo->number);
	pthread_mutex_unlock(&mutex);
	philo->state = thinking;
	return (NULL);
}

void grab_forks_if_can(t_philo *philo)
{
	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);
	if (philo->left_fork->philo == NULL)
	{
		printf("%ld: %d has taken a fork\n", get_time() - philo->started, philo->number);
		philo->left_fork->philo = philo;
	}
	if (philo->right_fork->philo == NULL)
	{
		printf("%ld: %d has taken a fork\n", get_time() - philo->started, philo->number);
		philo->right_fork->philo = philo;
	}
	pthread_mutex_unlock(&mutex);
}

void	*philosopher(void *params)
{
	t_philo		*philo;
	int			*someone_died;
	pthread_t	action;
	pthread_mutex_t mutex;

	philo = (t_philo *) params;
	someone_died = philo->someone_died;
	philo->time_since_eat = get_time();
	philo->started = get_time();
	philo->state = thinking;
	pthread_mutex_init(&mutex, NULL);
	while (!*someone_died)
	{
		if (get_time() - philo->time_since_eat > philo->times.die)
		{
			pthread_mutex_lock(&mutex);
			printf("%ld: %d died\n", get_time() - philo->started, philo->number);
			pthread_mutex_unlock(&mutex);
			*someone_died = 1;
			philo->state = dead;
		}
		grab_forks_if_can(philo);
		if (philo->left_fork->philo == philo && philo->right_fork->philo == philo && philo->state != waiting)
		{
			pthread_create(&action, NULL, eat, philo);
			philo->state = waiting;
		}
		if (philo->state == thinking)
		{
			pthread_mutex_lock(&mutex);
			printf("%ld: %d is thinking\n", get_time() - philo->started, philo->number);
			pthread_mutex_unlock(&mutex);
			philo->state = eating;
		}
		if (philo->state == sleeping)
		{
			pthread_create(&action, NULL, sleep_philo, philo);
			philo->state = waiting;
		}
	}
	return (NULL);
}

void	assign_forks(const int total_philos, t_philo *philos, t_fork *forks)
{
	int	i;

	i = 0;
	while (i < total_philos)
	{
		if (i == 0)
			philos[i].left_fork = &forks[total_philos - 1];
		else
			philos[i].left_fork = &forks[i - 1];
		philos[i].right_fork = &forks[i];
		i++;
	}
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
	int			total_philos;
	pthread_t	*philos_thread;
	t_philo		*philos;
	t_fork		*forks;
	int			died;

	total_philos = parse_input(argc, argv, &philos, &died);
	died = 0;
	if (total_philos == 0)
		return (1);
	if (!init_structs(&forks, &philos_thread, total_philos))
		return (1);
	assign_forks(total_philos, philos, forks);
	create_threads(total_philos, philos_thread, philos);
	free(philos);
	free(forks);
	free(philos_thread);
	return (0);
}
