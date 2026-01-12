/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:02:46 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 22:29:56 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	verify_meal_completion(t_philo *philos, t_data *data)
{
	int	i;
	int	eaten;
	int	all_satisfied;

	if (!data->check_meals)
		return (0);
	all_satisfied = 1;
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&philos[i].eat_lock);
		eaten = philos[i].eat_count;
		pthread_mutex_unlock(&philos[i].eat_lock);
		if (eaten < data->must_eat)
		{
			all_satisfied = 0;
			break ;
		}
		i++;
	}
	return (all_satisfied);
}

static int	check_philosopher_death(t_philo *philo)
{
	long int	current;
	long int	last_meal;
	long int	time_since_meal;

	current = current_time();
	pthread_mutex_lock(&philo->eat_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->eat_lock);
	time_since_meal = current - last_meal;
	if (time_since_meal >= philo->data->time_to_die)
	{
		pthread_mutex_lock(philo->data->death_lock);
		if (!philo->data->finished)
		{
			philo->data->finished = 1;
			printf("%ld %d died\n", current - philo->birth_time, philo->id);
			pthread_mutex_unlock(philo->data->death_lock);
			return (1);
		}
		pthread_mutex_unlock(philo->data->death_lock);
	}
	return (0);
}

static int	monitor_all_philosophers(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (check_philosopher_death(&philos[i]))
			return (1);
		i++;
	}
	if (verify_meal_completion(philos, data))
	{
		pthread_mutex_lock(data->death_lock);
		data->finished = 1;
		pthread_mutex_unlock(data->death_lock);
		return (1);
	}
	return (0);
}

void	supervise_simulation(t_data *data, t_philo *philos)
{
	while (1)
	{
		pthread_mutex_lock(data->death_lock);
		if (data->ready)
		{
			pthread_mutex_unlock(data->death_lock);
			break ;
		}
		pthread_mutex_unlock(data->death_lock);
		usleep(1000);
	}
	while (1)
	{
		pthread_mutex_lock(data->death_lock);
		if (data->finished)
		{
			pthread_mutex_unlock(data->death_lock);
			break ;
		}
		pthread_mutex_unlock(data->death_lock);
		if (monitor_all_philosophers(data, philos))
			break ;
		usleep(1000);
	}
}
