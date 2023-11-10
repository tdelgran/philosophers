/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 10:31:04 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 11:45:43 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_arg(int argc, char **argv, t_sim *sim)
{
	if (parse_arg(argc, argv, sim) != 0)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (init_data(sim) != 0)
	{
		printf("Error: initialization failure\n");
		return (1);
	}
	return (0);
}

int parse_arg(int argc, char **argv, t_sim *sim)
{
    if (argc < 5 || argc > 6)
        return (-1);
	if (parse_num(argv))
		return (-1);
    sim->philo_count = atoi(argv[1]);
    sim->time_to_die = atoi(argv[2]);
    sim->time_to_eat = atoi(argv[3]);
    sim->time_to_sleep = atoi(argv[4]);
    if (argc == 6)
        sim->required_meals_count = atoi(argv[5]);
    else
        sim->required_meals_count = -1;
    if (sim->philo_count <= 0 || sim->time_to_die <= 0 || 
        sim->time_to_eat <= 0 || sim->time_to_sleep <= 0 || 
        (argc == 6 && sim->required_meals_count <= 0))
        return (-1);
    return (0);
}

void *philo_routine(void *arg)
{
    t_philo   *philo;

    philo = (t_philo *)arg;
    while (1)
    {
         if (think(philo))
            break ;
        else if (eat(philo))
            break ;
        else if (sleep_philo(philo))
            break ;
        pthread_mutex_lock(&philo->sim->mutex_count_meal);
        if (philo->sim->required_meals_count != -1 
            && philo->meals_eaten >= philo->sim->required_meals_count)
        {
            pthread_mutex_lock(&philo->sim->mutex_finish_meal);
            philo->sim->all_philo_have_eaten = 1;
            pthread_mutex_unlock(&philo->sim->mutex_finish_meal);
            pthread_mutex_unlock(&philo->sim->mutex_count_meal);
            break ;
        }
        pthread_mutex_unlock(&philo->sim->mutex_count_meal);
    }
    return (NULL);
}
