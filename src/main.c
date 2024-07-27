/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 05:31:45 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/07/27 04:34:09 by xazuaje-         ###   ########.fr       */
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

int	destroy_forks(t_fork **forks, const int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&(*forks)[i].mutex);
		i++;
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

	philo = (t_philo *)params;
	pthread_mutex_lock(&philo->mutex);
	philo->time_since_eat = get_time();
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->times.eat * 1000);
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld: %d is eating\n", get_time() - philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->left_fork->philo = NULL;
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->right_fork->philo = NULL;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_lock(&philo->mutex);
	philo->state = sleeping;
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void *sleep_philo(void *params)
{
	t_philo	*philo;

	philo = (t_philo *)params;
	usleep(philo->times.sleep * 1000);
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld: %d is sleeping\n", get_time() - philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	pthread_mutex_lock(&philo->mutex);
	philo->state = thinking;
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void grab_forks_if_can(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	if (philo->left_fork->philo == NULL)
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%ld: %d has taken left fork -> %p\n", get_time() - philo->started, philo->number, philo->left_fork);
		pthread_mutex_unlock(philo->print_mutex);
		philo->left_fork->philo = philo;
	}
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	if (philo->right_fork->philo == NULL)
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%ld: %d has taken right fork -> %p\n", get_time() - philo->started, philo->number, philo->right_fork);
		pthread_mutex_unlock(philo->print_mutex);
		philo->right_fork->philo = philo;
	}
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

int must_die(t_philo *philo)
{
	time_t i;

	pthread_mutex_lock(&philo->mutex);
	i = get_time() - philo->time_since_eat > philo->times.die;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

int check_state(t_philo *philo, t_states state)
{
	time_t i;

	pthread_mutex_lock(&philo->mutex);
	i = philo->state == state;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

int can_eat(t_philo *philo)
{
	time_t i;

	i = philo->left_fork->philo == philo && philo->right_fork->philo == philo && philo->state != waiting && philo->left_fork != philo->right_fork;
	return (i);
}

void	*philosopher(void *params)
{
	t_philo		*philo;
	int			*someone_died;
	pthread_t	action;

	philo = (t_philo *) params;
	someone_died = philo->someone_died;
	philo->time_since_eat = get_time();
	philo->started = get_time();
	philo->state = thinking;
	pthread_mutex_init(&philo->mutex, NULL);
	if (philo->number % 2 == 0)
		usleep(philo->times.eat * 1000);
	while (!*someone_died)
	{
		if (must_die(philo))
		{
			pthread_mutex_lock(philo->print_mutex);
			printf("%ld: %d died\n", get_time() - philo->started, philo->number);
			*someone_died = 1;
			pthread_mutex_unlock(philo->print_mutex);
			break;
		}
		if (check_state(philo, thinking))
		{
			pthread_mutex_lock(philo->print_mutex);
			printf("%ld: %d is thinking\n", get_time() - philo->started, philo->number);
			pthread_mutex_unlock(philo->print_mutex);
			philo->state = eating;
		}
		else if (check_state(philo, eating))
		{
			grab_forks_if_can(philo);
			if (can_eat(philo))
			{
				pthread_create(&action, NULL, eat, philo);
				philo->state = waiting;
			}
		}
		else if (check_state(philo, sleeping))
		{
			pthread_create(&action, NULL, sleep_philo, philo);
			philo->state = waiting;
		}
	}
	pthread_mutex_destroy(&philo->mutex);
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
	int				total_philos;
	pthread_t		*philos_thread;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t print_mutex;
	int			died;

	total_philos = parse_input(argc, argv, &philos, &died, &print_mutex);
	died = 0;
	if (total_philos == 0)
		return (1);
	if (!init_structs(&forks, &philos_thread, total_philos))
		return (1);
	assign_forks(total_philos, philos, forks);
	create_threads(total_philos, philos_thread, philos);
	destroy_forks(&forks, total_philos);
	pthread_mutex_destroy(&print_mutex);
	free(philos);
	free(forks);
	free(philos_thread);
	return (0);
}
