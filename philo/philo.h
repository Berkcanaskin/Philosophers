/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baskin <baskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:03:36 by baskin            #+#    #+#             */
/*   Updated: 2025/06/03 23:14:39 by baskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_data
{
	int				philo_count;
	int				ready;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				check_meals;
	int				finished;
	long int		start_time;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*forks;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				dead;
	int				eat_count;
	long int		birth_time;
	long int		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	eat_lock;
	t_data			*data;
}	t_philo;

long int	current_time(void);
void		precise_sleep(long int duration);
void		display_message(t_philo *philo, char *msg);
int			init_simulation(t_data *data);
int			set_philos(t_data *data, t_philo *philos);
int			start_simulation(t_data *data, t_philo *philos);
void		*philo_lifecycle(void *arg);
void		acquire_forks_and_consume(t_philo *philo);
int			monitor_death(t_philo *philo);
int			verify_meal_completion(t_philo *philos, t_data *data);
void		supervise_simulation(t_data *data, t_philo *philos);
void		handle_error_and_exit(char *msg, t_data *data, t_philo *philos);
int			allocate_mutexes(t_data *data);
int			validate_input(int argc, char **argv);
int			init_simulation(t_data *data);
void		handle_error_and_exit(char *msg, t_data *data, t_philo *philos);
int			ft_atoi(char *str);
int			check_simulation_end(t_philo *philo);
int			acquire_forks_ordered(t_philo *philo);
void		update_meal_time(t_philo *philo);
void		increment_meals(t_philo *philo);
void		release_forks(t_philo *philo);
int			declare_death(t_philo *philo);
void		*handle_single_philo(t_philo *philo);

#endif