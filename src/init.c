/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltestard <ltestard@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 19:17:52 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 19:10:33 by ltestard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_threads(t_sim *data)
{
	data->thread = malloc(sizeof(pthread_t) * data->philo_count);
	if (!data->thread)
		return (-1);
	return (0);
}

int	init_forks(t_sim *sim)
{
	int	i;

	sim->forks_mutex = malloc(sizeof(pthread_mutex_t) * sim->philo_count);
	if (!sim->forks_mutex)
		return (-1);
	i = 0;
	while (i < sim->philo_count)
	{
		pthread_mutex_init(&sim->forks_mutex[i], 0);
		i++;
	}
	return (0);
}

int	init_philo(t_sim *sim)
{
	int	i;

	sim->philo = malloc(sizeof(t_sim) * sim->philo_count);
	if (!sim->philo_count)
		return (-1);
	i = 0;
	while (i < sim->philo_count)
	{
		sim->philo[i].id = i + 1;
		sim->philo[i].last_meal_time = get_current_time();
		sim->philo[i].meals_eaten = 0;
		sim->philo[i].sim = sim;
		sim->philo[i].left_fork = &sim->forks_mutex[i];
		sim->philo[i].right_fork = &sim->forks_mutex[(i + 1)
			% sim->philo_count];
		i++;
	}
	return (0);
}

int	init_data(t_sim *data)
{
	if (init_forks(data) == -1)
		return (-1);
	if (init_philo(data) == -1)
	{
		free(data->forks_mutex);
		return (-1);
	}
	if (init_threads(data) == -1)
	{
		free(data->forks_mutex);
		free(data->philo);
		return (-1);
	}
	pthread_mutex_init(&data->death_mutex, 0);
	pthread_mutex_init(&data->mutex_last_meal, 0);
	pthread_mutex_init(&data->mutex_count_meal, 0);
	pthread_mutex_init(&data->mutex_finish_meal, 0);
	pthread_mutex_init(&data->mutex_print, 0);
	data->philo_is_dead = 0;
	data->all_philo_have_eaten = 0;
	return (0);
}

void	drop_forks(pthread_mutex_t *philo)
{
	pthread_mutex_unlock(philo);
}
