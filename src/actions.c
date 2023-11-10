/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 09:45:01 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 11:49:32 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	take_forks(pthread_mutex_t *fork, t_philo *philo)
{
	pthread_mutex_lock(fork);
	display_philo_status(philo, "has taken a fork");
}

void	forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
        take_forks(philo->left_fork, philo);
		take_forks(philo->right_fork, philo);
	}
	else
	{
		take_forks(philo->right_fork, philo);
        take_forks(philo->left_fork, philo);
	}
}

int eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->sim->death_mutex);
    if(philo->sim->philo_is_dead)
    {
        pthread_mutex_unlock(&philo->sim->death_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->sim->death_mutex);
    forks(philo); 
    pthread_mutex_lock(&philo->sim->mutex_last_meal);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(&philo->sim->mutex_last_meal);
    display_philo_status(philo, "is eating");
    ft_usleep(philo->sim->time_to_eat);
    pthread_mutex_lock(&philo->sim->mutex_count_meal);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->sim->mutex_count_meal);
    drop_forks(philo->left_fork);
    drop_forks(philo->right_fork);
    return (0);
}

int	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->death_mutex);
	if (philo->sim->philo_is_dead >= 1)
	{
		pthread_mutex_unlock(&philo->sim->death_mutex);
        return (1);
	}
	pthread_mutex_unlock(&philo->sim->death_mutex);
	display_philo_status(philo, "is thinking");
    return (0);
}

int sleep_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->sim->death_mutex);
    if(philo->sim->philo_is_dead >= 1)
    {
        pthread_mutex_unlock(&philo->sim->death_mutex);
        return (1) ;
    }
    pthread_mutex_unlock(&philo->sim->death_mutex);
    display_philo_status(philo, "is sleeping");
    ft_usleep(philo->sim->time_to_sleep);
    return (0);
}
