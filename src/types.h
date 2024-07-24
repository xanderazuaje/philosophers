/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xazuaje- <xazuaje-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:25:16 by xazuaje-          #+#    #+#             */
/*   Updated: 2024/07/24 01:00:22 by xazuaje-         ###   ########.fr       */
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
}	t_fork;

typedef struct s_times
{
	int	eat;
	int	sleep;
	int	die;
}	t_times;

typedef struct s_philo
{
	int				number;
	time_t			time_since_eat;
	time_t			started;
	t_fork			*left_fork;
	t_fork			*right_fork;
	int				max_eat_count;
	struct t_philo	*left_philo;
	struct t_philo	*right_philo;
	t_times			times;
	int				*someone_died;
	t_states		state;
}	t_philo;

#endif