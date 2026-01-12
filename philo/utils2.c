/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:44:33 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 23:59:31 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_input(int argc, char **argv)
{
	int	i;
	int	j;
	int	philo_count;

	if (argc != 5 && argc != 6)
		return (printf("Error\n"), 1);
	philo_count = atoi(argv[1]);
	if (philo_count <= 0 || philo_count > 200)
		return (printf("Error\n"), 1);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (printf("Error:\n"), 1);
			j++;
		}
		i++;
	}
	return (0);
}

void	handle_error_and_exit(char *msg, t_data *data, t_philo *philos)
{
	int	i;

	printf("%s", msg);
	if (philos)
	{
		i = -1;
		while (++i < data->philo_count)
			pthread_mutex_destroy(&philos[i].eat_lock);
		free(philos);
	}
	if (data->forks)
	{
		i = -1;
		while (++i < data->philo_count)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	if (data->death_lock)
	{
		pthread_mutex_destroy(data->death_lock);
		free(data->death_lock);
	}
	exit(EXIT_FAILURE);
}

int	allocate_mutexes(t_data *data)
{
	int	i;

	data->death_lock = malloc(sizeof(pthread_mutex_t));
	if (!data->death_lock)
		return (1);
	if (pthread_mutex_init(data->death_lock, NULL) != 0)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_simulation(t_data *data)
{
	data->finished = 0;
	data->ready = 0;
	data->start_time = current_time();
	if (allocate_mutexes(data))
		return (1);
	return (0);
}
