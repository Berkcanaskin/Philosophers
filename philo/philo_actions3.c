/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 23:14:04 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 23:19:20 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sync_start(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&philo->eat_lock);
	philo->birth_time = current_time();
	philo->last_meal = philo->birth_time;
	pthread_mutex_unlock(&philo->eat_lock);
	while (1)
	{
		pthread_mutex_lock(data->death_lock);
		if (data->ready)
		{
			pthread_mutex_unlock(data->death_lock);
			return ;
		}
		pthread_mutex_unlock(data->death_lock);
		precise_sleep(1);
	}
}

void	*handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	display_message(philo, "has taken a fork");
	precise_sleep(philo->data->time_to_die);
	declare_death(philo);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*philo_lifecycle(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	philo_sync_start(philo, data);
	if (data->philo_count == 1)
		return (handle_single_philo(philo));
	if (philo->id % 2 == 0)
		precise_sleep(data->time_to_eat / 2);
	while (!check_simulation_end(philo))
	{
		acquire_forks_and_consume(philo);
		if (check_simulation_end(philo))
			break ;
		display_message(philo, "is sleeping");
		precise_sleep(data->time_to_sleep);
		if (check_simulation_end(philo))
			break ;
		display_message(philo, "is thinking");
		if (data->philo_count % 2 == 1)
			precise_sleep(1);
	}
	return (NULL);
}
