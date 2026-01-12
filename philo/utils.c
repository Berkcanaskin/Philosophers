/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:03:57 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 22:35:37 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long int duration)
{
	long int	start;
	long int	elapsed;

	start = current_time();
	while (1)
	{
		elapsed = current_time() - start;
		if (elapsed >= duration)
			break ;
		if (duration - elapsed > 10)
			usleep((duration - elapsed - 1) * 1000);
		else
			usleep(100);
	}
}

void	display_message(t_philo *philo, char *msg)
{
	long int	timestamp;
	long int	birth_time;

	pthread_mutex_lock(philo->data->death_lock);
	if (philo->data->finished)
	{
		pthread_mutex_unlock(philo->data->death_lock);
		return ;
	}
	timestamp = current_time();
	pthread_mutex_lock(&philo->eat_lock);
	birth_time = philo->birth_time;
	pthread_mutex_unlock(&philo->eat_lock);
	printf("%ld %d %s\n", timestamp - birth_time, philo->id, msg);
	pthread_mutex_unlock(philo->data->death_lock);
}

int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (str[i] == '-' || str[i] == '+')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
