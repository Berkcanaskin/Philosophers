/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:24:47 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 23:15:11 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	acquire_forks_and_consume(t_philo *philo)
{
	if (check_simulation_end(philo))
		return ;
	if (!acquire_forks_ordered(philo))
		return ;
	if (check_simulation_end(philo))
		return (release_forks(philo));
	update_meal_time(philo);
	display_message(philo, "is eating");
	precise_sleep(philo->data->time_to_eat);
	increment_meals(philo);
	release_forks(philo);
}

int	declare_death(t_philo *philo)
{
	pthread_mutex_lock(philo->data->death_lock);
	if (philo->data->finished)
	{
		pthread_mutex_unlock(philo->data->death_lock);
		return (1);
	}
	philo->data->finished = 1;
	pthread_mutex_unlock(philo->data->death_lock);
	pthread_mutex_lock(&philo->eat_lock);
	philo->dead = 1;
	pthread_mutex_unlock(&philo->eat_lock);
	printf("%ld %d died\n", current_time() - philo->data->start_time, philo->id);
	return (1);
}

int	monitor_death(t_philo *philo)
{
	long int	last;
	long int	elapsed;

	pthread_mutex_lock(&philo->eat_lock);
	last = philo->last_meal;
	pthread_mutex_unlock(&philo->eat_lock);
	elapsed = current_time() - last;
	if (elapsed >= philo->data->time_to_die)
		return (declare_death(philo));
	return (0);
}
