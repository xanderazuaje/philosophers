/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:25:16 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/11/04 18:05:50 by xazuaje-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <pthread.h>

typedef struct s_philo	t_philo;

typedef enum e_states {
	eating,
	thinking,
	sleeping,
	dead,
	waiting
} t_states;

typedef struct s_fork
{
	t_philo *philo;
	pthread_mutex_t mutex;
}	t_fork;

typedef struct s_times
{
	time_t	eat;
	time_t	sleep;
	time_t	die;
}	t_times;

typedef struct s_philo
{
	int				number;
	time_t			time_since_eat;
	time_t			*started;
	t_fork			*left_fork;
	t_fork			*right_fork;
	int				max_eat_count;
	int				total_eat_count;
	struct t_philo	*left_philo;
	struct t_philo	*right_philo;
	t_times			times;
	int				*someone_died;
	t_states		state;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*print_mutex;
}	t_philo;


typedef struct s_program {
	pthread_t		*philos_thread;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t print_mutex;
	time_t			started;
	int				died;
} t_program;

#endif