/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdelgran <tdelgran@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 12:14:06 by tdelgran          #+#    #+#             */
/*   Updated: 2023/11/10 11:51:09 by tdelgran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#ifndef ERROR
# define ERROR (-1)
#endif

#ifndef SUCCESS
# define SUCCESS 0
#endif

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>


typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	struct  s_sim	*sim;
}					t_philo;

typedef struct s_sim
{
	int				miam;
	int				is_sim_running;
	int				all_philo_have_eaten;
	int				number_of_times_each_philo_must_eat;
	pthread_mutex_t output_lock;
	pthread_mutex_t *forks_mutex;
	pthread_mutex_t death_mutex;
	pthread_mutex_t mutex_last_meal; //meal
	pthread_mutex_t mutex_count_meal; //meals_finish
	pthread_mutex_t mutex_finish_meal; //timestamp
	pthread_mutex_t mutex_print; //miam
	pthread_t		*thread;
	int				philo_is_dead;
	int				philo_count;
	long			time_until_death;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_die;
	int				required_meals_count;
	int				sim_start_time;
	t_philo			*philo;
}                   t_sim;


/*  actions */
void	forks(t_philo *philo);
void	drop_forks(pthread_mutex_t *philo);
void	take_forks(pthread_mutex_t *fork, t_philo *philo);
int		eat(t_philo *philo);
int		think(t_philo *philo);
int		sleep_philo(t_philo *philo);
void	single_philo(t_sim *sim,pthread_t *monitor);

/*  parsing */
int		parse_arg(int argc, char **argv, t_sim *sim);
int		parse_num(char **argv);
int		init_data(t_sim *data);
int		check_arg(int argc, char **argv, t_sim *sim);

/*  thread */
void	clean_sim(t_sim *sim);
void	join_philo_threads(t_sim *sim, pthread_t *monitor);
void	create_philo_threads(t_sim *sim, pthread_t *monitor);

/* display */
void	*philo_routine(void *arg);
void	display_philo_status(t_philo *philo, char *status);
void	*monitor_check(void *arg);

/*  utils */
void	ft_usleep(int duration);
int		ft_atoi(const char *str);
void	ft_putstr(char *s);
int		get_current_time(void);
void	free_resources(t_sim *data);

#endif