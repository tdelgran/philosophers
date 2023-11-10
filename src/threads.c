/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 09:49:20 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 15:40:30 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	join_philo_threads(t_sim *sim, pthread_t *monitor)
{
	int	i;

	i = -1;
	while (++i < sim->philo_count)
	{
		pthread_join(sim->thread[i], NULL);
		pthread_join(monitor[i], NULL);
	}
}

void	create_philo_threads(t_sim *sim, pthread_t *monitor)
{
	int	i;

	i = -1;
	while (++i < sim->philo_count)
	{
		pthread_create(&sim->thread[i], NULL, philo_routine, &sim->philo[i]);
		pthread_create(&monitor[i], NULL, monitor_check, &sim->philo[i]);
	}
}

void clean_sim(t_sim *sim)
{
    pthread_mutex_destroy(&sim->output_lock);
    free(sim->philo);
}

int	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (int)1000 + tv.tv_usec / 1000);
}

void single_philo(t_sim *sim,pthread_t *monitor)
{
    pthread_mutex_lock(&sim->death_mutex);
    printf("[0] 1 is thinking\n");
    printf("[%ld] 1 has taken a fork\n", sim->time_to_die);
    usleep(sim->time_to_die * 1000);
    printf("[%ld] 1 is dead\n", sim->time_to_die);
    pthread_mutex_unlock(&sim->death_mutex);
    free(monitor);
}
