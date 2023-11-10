/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 09:14:10 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 15:48:01 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	display_died(t_philo *philo)
{
	int			timestamp;
	const char	*color_code;

	color_code = "\033[1;31m";
	timestamp = get_current_time() - philo->sim->sim_start_time;
	pthread_mutex_lock(&philo->sim->death_mutex);
	pthread_mutex_lock(&philo->sim->mutex_print);
	if (philo->sim->philo_is_dead == 1)
	{
		philo->sim->philo_is_dead++;
		printf("%s[%d] %d %s\033[0m\n",
			color_code, timestamp, philo->id, "died");
		pthread_mutex_unlock(&philo->sim->death_mutex);
		pthread_mutex_unlock(&philo->sim->mutex_print);
		return ;
	}
	pthread_mutex_unlock(&philo->sim->mutex_print);
	pthread_mutex_unlock(&philo->sim->death_mutex);
}

int	check_death(t_philo *philo)
{
	int	current_time;

	current_time = get_current_time();
	pthread_mutex_lock(&philo->sim->death_mutex);
	if (philo->sim->philo_is_dead >= 1)
	{
		pthread_mutex_unlock(&philo->sim->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->death_mutex);
	pthread_mutex_lock(&philo->sim->mutex_last_meal);
	if ((current_time - philo->last_meal_time) > philo->sim->time_to_die)
	{
		pthread_mutex_lock(&philo->sim->death_mutex);
		philo->sim->philo_is_dead++;
		pthread_mutex_unlock(&philo->sim->death_mutex);
		pthread_mutex_unlock(&philo->sim->mutex_last_meal);
		return (1);
	}
	pthread_mutex_unlock(&philo->sim->mutex_last_meal);
	return (0);
}

void	free_resources(t_sim *data)
{
	int	i;

	i = 0;
	free(data->forks_mutex);
	free(data->philo);
	free(data->thread);
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->forks_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->mutex_last_meal);
	pthread_mutex_destroy(&data->mutex_count_meal);
	pthread_mutex_destroy(&data->mutex_print);
}

void    *monitor_check(void *arg)
{
    t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->sim->death_mutex);
		if (philo->sim->philo_is_dead >= 1)
		{
			pthread_mutex_unlock(&philo->sim->death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->sim->death_mutex);
		if (check_death(philo))
		{
			pthread_mutex_lock(&philo->sim->mutex_last_meal);
			if (philo->sim->all_philo_have_eaten == 0)
				display_died(philo);
			pthread_mutex_unlock(&philo->sim->mutex_last_meal);
			return (NULL);
		}
		ft_usleep(100);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
    t_sim		sim;
	pthread_t	*monitor;
    
    if (check_arg(argc, argv, &sim))
        return (1);
    sim.sim_start_time = get_current_time();
	monitor = malloc(sizeof(t_philo) * sim.philo_count);
		if (sim.philo_count == 1)
	{
		single_philo(&sim, monitor);
		return (1);
	}
	create_philo_threads(&sim, monitor);
	join_philo_threads(&sim, monitor);
	free(monitor);
	free_resources(&sim);
    return (0);
}
