/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 10:50:15 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 11:39:23 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_usleep(int dure)
{
	int	start_time;
	int	end_time;

	start_time = get_current_time();
	end_time = start_time + dure;
    while(get_current_time() < end_time)
		usleep(500);
}

void	ft_putstr(char *s)
{
	if (!s)
		return ;
	while (*s)
	{
		write(1, &*s, 1);
		s++;
	}
}

int ft_atoi(const char *str)
{
    int result = 0;
    int sign = 1;

    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}

void	display_philo_status(t_philo *philo, char *status)
{
    int	time;

    time = get_current_time() - philo->sim->sim_start_time;
    pthread_mutex_lock(&philo->sim->death_mutex);
    pthread_mutex_lock(&philo->sim->mutex_print);
    if (philo->sim->philo_is_dead < 1)
    {
        printf("[%d] %d %s\n", time, philo->id, status);
        pthread_mutex_unlock(&philo->sim->mutex_print);
        pthread_mutex_unlock(&philo->sim->death_mutex);
    }
    pthread_mutex_unlock(&philo->sim->mutex_print);
    pthread_mutex_unlock(&philo->sim->death_mutex);
}

int	parse_num(char **argv)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (argv[j])
	{
		i = 0;
		while (argv[j][i])
		{
			if (!(argv[j][i] <= '9' && argv[j][i] >= '0'))
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}
