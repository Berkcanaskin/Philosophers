/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:04:45 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 22:32:37 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_simulation_end(t_philo *philo)
{
	int	finished;

	pthread_mutex_lock(philo->data->death_lock);
	finished = philo->data->finished;
	pthread_mutex_unlock(philo->data->death_lock);
	return (finished);
}

int	acquire_forks_ordered(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pthread_mutex_lock(first);
	if (check_simulation_end(philo))
		return (pthread_mutex_unlock(first), 0);
	display_message(philo, "has taken a fork");
	pthread_mutex_lock(second);
	if (check_simulation_end(philo))
	{
		pthread_mutex_unlock(second);
		return (pthread_mutex_unlock(first), 0);
	}
	display_message(philo, "has taken a fork");
	return (1);
}

void	update_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_lock);
	philo->last_meal = current_time();
	pthread_mutex_unlock(&philo->eat_lock);
}

void	increment_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_lock);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_lock);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
