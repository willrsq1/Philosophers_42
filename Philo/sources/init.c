/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:02:47 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/10 20:40:29 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/* -------------------------------------------------------------------------- */
/* ---- FT_PHILO_INIT ------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Initiates threads, mutexes, and all needed datas. ------------------------ */
/* Launches the Diner with one thread for each Philosopher. ----------------- */
/* They will all start at the "same" time thanks to diner->start_us. -------- */
/* -------------------------------------------------------------------------- */

int	ft_philo_init(t_diner *diner, char **argv, int argc)
{
	diner->ms_die = ft_atoi_philo(argv[2]);
	if (diner->nb_of_philo == 1)
		return (printf("Time = 0ms. Philosopher 1 has taken a fork\n"), \
			printf("Time = %ldms. Philosopher 1 is dead\n", diner->ms_die), 3);
	diner->ms_eat = ft_atoi_philo(argv[3]);
	diner->ms_sleep = ft_atoi_philo(argv[4]);
	diner->ms_think = (diner->ms_die - diner->ms_sleep - diner->ms_eat) * 0.7;
	if (diner->nb_of_philo % 2 == 0)
		diner->ms_think = (diner->ms_eat - diner->ms_sleep) / 2;
	diner->meals_needed = -1;
	if (argc == 6)
		diner->meals_needed = ft_atoi_philo(argv[5]);
	if (ft_init_forks(diner) != 0 || ft_init_threads_philosophers(diner) != 0)
		return (1);
	diner->i = -1;
	diner->start = (ft_time_microsec() + 1000000) / 1000;
	diner->start_us = ft_time_microsec() + 1000000;
	while (++diner->i < diner->nb_of_philo)
	{
		if (pthread_create(&diner->philos[diner->i]->thread, \
			NULL, &philo_life, diner->philos[diner->i]) != 0)
			return (1);
		usleep(100);
	}
	return (0);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_INIT_THREADS_PHILOSOPHERS ---------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Initiates each philo with it's data. ------------------------------------- */
/* Assigns forks. ----------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

int	ft_init_threads_philosophers(t_diner *diner)
{
	int		i;
	t_philo	**philos;

	i = -1;
	philos = diner->philos;
	while (++i < diner->nb_of_philo)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (1);
		memset(philos[i], 0, sizeof(t_philo));
		philos[i]->diner = diner;
		philos[i]->has_eaten = 0;
		philos[i]->meals_needed = philos[i]->diner->meals_needed;
		philos[i]->id = i;
		philos[i]->last_supper_time = 0;
		philos[i]->fork1 = philos[i]->id;
		philos[i]->fork2 = (philos[i]->id + 1) % philos[i]->diner->nb_of_philo;
	}
	printf("\n	Loading...\n\n");
	return (0);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_INIT_FORKS ------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Mallocs and pthread_mutex_init. ------------------------------------------ */
/* -------------------------------------------------------------------------- */

int	ft_init_forks(t_diner *diner)
{
	int				i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * diner->nb_of_philo);
	if (!forks)
		return (1);
	diner->dead_philos = 0;
	memset(forks, 0, sizeof(pthread_mutex_t) * diner->nb_of_philo);
	i = -1;
	while (++i < diner->nb_of_philo)
		pthread_mutex_init(&forks[i], NULL);
	diner->forks = forks;
	pthread_mutex_init(&diner->write_lock, NULL);
	pthread_mutex_init(&diner->deads_lock, NULL);
	diner->philos = malloc(sizeof(t_philo) * diner->nb_of_philo);
	if (!diner->philos)
		return (1);
	memset(diner->philos, 0, sizeof(t_philo) * diner->nb_of_philo);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_PARSING ---------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Chekcs the validity of the inputed args. --------------------------------- */
/* -------------------------------------------------------------------------- */

int	ft_parsing(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (printf("Bad input\n"), 1);
	if (ft_atoi_philo(argv[1]) == -1 || ft_atoi_philo(argv[2]) == -1 || \
		ft_atoi_philo(argv[1]) == 0)
	{
		if (ft_atoi_philo(argv[1]) == 0)
			printf("How can ZERO philosophers have diner ?\n");
		else
			printf("Bad input\n");
		return (1);
	}
	if (ft_atoi_philo(argv[3]) == -1 || ft_atoi_philo(argv[4]) == -1)
		return (printf("Bad input\n"), 1);
	if (argc == 6)
	{
		if (ft_atoi_philo(argv[5]) == -1)
			return (printf("Bad input\n"), 1);
		if (ft_atoi_philo(argv[5]) == 0)
			return (printf("No one was hungry enough to eat\n"), 1);
	}
	return (0);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_ATOI_PHILO ------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Regular atoi, arranged for this projet. ---------------------------------- */
/* -------------------------------------------------------------------------- */

time_t	ft_atoi_philo(const char *str)
{
	time_t	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	if (i == 0 || str[i] != '\0' || result > 2147483647)
		return (-1);
	return (result);
}
