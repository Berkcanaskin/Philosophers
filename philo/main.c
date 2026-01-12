/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:03:07 by baskin            #+#    #+#             */
/*   Updated: 2025/06/04 00:02:32 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].dead = 0;
		philos[i].eat_count = 0;
		philos[i].birth_time = data->start_time;
		philos[i].last_meal = data->start_time;
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		if (i + 1 < data->philo_count)
			philos[i].right_fork = &data->forks[i + 1];
		else
			philos[i].right_fork = &data->forks[0];
		if (pthread_mutex_init(&philos[i].eat_lock, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	start_simulation(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philo_lifecycle, &philos[i]) != 0)
			return (1);
		i++;
	}
	pthread_mutex_lock(data->death_lock);
	data->ready = 1;
	pthread_mutex_unlock(data->death_lock);
	return (0);
}

static void	terminate_simulation(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&philos[i].eat_lock);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	pthread_mutex_destroy(data->death_lock);
	free(data->death_lock);
	free(philos);
}

static int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (1);
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->check_meals = 0;
	data->must_eat = -1;
	if (argc == 6)
	{
		data->must_eat = ft_atoi(argv[5]);
		if (data->must_eat == 0)
			printf("%s", "Error\n");
		data->check_meals = 1;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (validate_input(argc, argv))
		return (1);
	if (parse_args(argc, argv, &data))
		return (printf("Error\n"));
	if (init_simulation(&data))
		handle_error_and_exit("Error\n", &data, NULL);
	philos = malloc(sizeof(t_philo) * data.philo_count);
	if (!philos)
		handle_error_and_exit("Error\n", &data, NULL);
	if (set_philos(&data, philos))
		handle_error_and_exit("Error\n", &data, philos);
	if (start_simulation(&data, philos))
		handle_error_and_exit("Error\n", &data, philos);
	supervise_simulation(&data, philos);
	terminate_simulation(&data, philos);
	return (0);
}
