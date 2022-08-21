/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roaraujo <roaraujo@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 17:10:40 by roaraujo          #+#    #+#             */
/*   Updated: 2022/08/21 20:05:33 by roaraujo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	init_common_data(t_common_data *common)
{
	common->halt_execution = FALSE;
	common->first_death = TRUE;
	get_curr_time_abs_usec(&(common->pgm_started_abs_usec));
	pthread_mutex_init(&common->printf_mutex, NULL);
	pthread_mutex_init(&common->halt_mutex, NULL);
	return ;
}

static void	assign_forks(t_philo_data **philos, int philos_number)
{
	int	i;

	i = -1;
	while (++i < philos_number)
	{
		if (i % 2)
		{
			(*philos)[i].lefthand_fork = (*philos)[i].this_fork;
			(*philos)[i].righthand_fork = (*philos)[(i + 1) % philos_number]
				.this_fork;
		}
		else
		{
			(*philos)[i].lefthand_fork = (*philos)[(i + 1) % philos_number]
				.this_fork;
			(*philos)[i].righthand_fork = (*philos)[i].this_fork;
		}
	}
	return ;
}

static void	init_philos(t_philo_data **philos, t_common_data *common)
{
	int	i;

	*philos = malloc(common->nb_of_philos * sizeof(t_philo_data));
	i = -1;
	while (++i < common->nb_of_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_had = 0;
		(*philos)[i].time_to_eat_ms = common->time_to_eat_ms;
		(*philos)[i].time_to_sleep_ms = common->time_to_sleep_ms;
		(*philos)[i].time_to_die_ms = common->time_to_die_ms;
		(*philos)[i].nb_of_meals = common->nb_of_meals;
		(*philos)[i].halt = &common->halt_execution;
		(*philos)[i].last_meal_abs_usec = common->pgm_started_abs_usec;
		(*philos)[i].this_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((*philos)[i].this_fork, NULL);
		(*philos)[i].last_meal_mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((*philos)[i].last_meal_mutex, NULL);
		(*philos)[i].meals_had_mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((*philos)[i].meals_had_mutex, NULL);
		(*philos)[i].thread = malloc(sizeof(pthread_t));
		(*philos)[i].common = common;
	}
	return ;
}

void	initialise_data(t_philo_data **philos, t_common_data *common)
{
	init_common_data(common);
	init_philos(philos, common);
	assign_forks(philos, common->nb_of_philos);
	return ;
}

void	create_threads(t_philo_data **philos)
{
	int	i;
	int	nb_of_philos;

	nb_of_philos = (*philos)[0].common->nb_of_philos;
	i = -1;
	while (++i < nb_of_philos)
		pthread_create((*philos)[i].thread,
			NULL,
			&dine,
			(void *)((*philos) + i));
	return ;
}
