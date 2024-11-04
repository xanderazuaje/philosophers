/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 05:31:45 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/04 17:06:13 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int did_someone_died(t_philo *philo)
{
	int i;

	pthread_mutex_lock(&philo->mutex);
	i = *(philo->someone_died) == 1;
	pthread_mutex_unlock(&philo->mutex);
	return (i);
}

time_t get_time()
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void fake_sleep(time_t time)
{
	time_t started;

	started = get_time();
	while (get_time() - started < time)
	{
		usleep(1);
	}
}

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

void *eat(void *params)
{
	t_philo *philo;

	philo = (t_philo *)params;
	if (!did_someone_died(philo))
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%ld: %d is eating\n", get_time() - *philo->started, philo->number);
		pthread_mutex_unlock(philo->print_mutex);
	}
	else
		return (NULL);
	pthread_mutex_lock(&philo->mutex);
	philo->time_since_eat = get_time();
	pthread_mutex_unlock(&philo->mutex);
	fake_sleep(philo->times.eat);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->left_fork->philo = NULL;
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->right_fork->philo = NULL;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_lock(&philo->mutex);
	philo->state = sleeping;
	if (philo->max_eat_count != -1)
	{
		philo->total_eat_count++;
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

void *sleep_philo(void *params)
{
	t_philo	*philo;

	philo = (t_philo *)params;
	pthread_mutex_lock(philo->print_mutex);
	if (!did_someone_died(philo))
		printf("%ld: %d is sleeping\n", get_time() - *philo->started, philo->number);
	pthread_mutex_unlock(philo->print_mutex);
	fake_sleep(philo->times.sleep);
	pthread_mutex_lock(&philo->mutex);
	philo->state = thinking;
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

int is_fork_used(t_fork *fork)
{
	int i;

	pthread_mutex_lock(&fork->mutex);
	i = fork->philo == NULL;
	pthread_mutex_unlock(&fork->mutex);

	return (i);
}

void grab_forks_if_can(t_philo *philo)
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

int must_die(t_philo *philo)
{
	time_t i;

	pthread_mutex_lock(&philo->mutex);
	i = get_time() - philo->time_since_eat > philo->times.die;
	if (i != 1)
		i = philo->total_eat_count >= philo->max_eat_count && philo->max_eat_count != -1;
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

	philo = (t_philo *) params;
	philo->time_since_eat = get_time();
	philo->state = thinking;
	pthread_mutex_init(&philo->mutex, NULL);
	if (philo->number % 2 == 0)
		fake_sleep(philo->times.eat);
	while (!did_someone_died(philo))
	{
		if (must_die(philo))
		{
			pthread_mutex_lock(philo->print_mutex);
			if (!did_someone_died(philo))
				printf("%ld: %d died\n", get_time() - *philo->started, philo->number);
			pthread_mutex_unlock(philo->print_mutex);
			pthread_mutex_lock(&philo->mutex);
			*philo->someone_died = 1;
			pthread_mutex_unlock(&philo->mutex);
			break;
		}
		if (check_state(philo, thinking))
		{
			pthread_mutex_lock(philo->print_mutex);
			if (!did_someone_died(philo))
				printf("%ld: %d is thinking\n", get_time() - *philo->started, philo->number);
			pthread_mutex_unlock(philo->print_mutex);
			philo->state = eating;
		}
		else if (!did_someone_died(philo) && check_state(philo, eating))
		{
			grab_forks_if_can(philo);
			if (can_eat(philo))
				eat(philo);
		}
		else if (!did_someone_died(philo) && check_state(philo, sleeping))
			sleep_philo(philo);
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
	t_program		program;

	total_philos = parse_input(argc, argv, &program);
	program.died = 0;
	if (total_philos == 0)
		return (1);
	if (!init_structs(&program.forks, &program.philos_thread, total_philos))
		return (1);
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
