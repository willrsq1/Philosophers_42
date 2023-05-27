/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 21:02:47 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/27 14:35:34 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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

static int	ft_init_threads_philosophers(t_diner *diner)
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
	}
	return (0);
}

static int	ft_init_forks(t_diner *diner)
{
	diner->philos = NULL;
	sem_unlink("/forks");
	sem_unlink("/write_lock");
	sem_unlink("/deads_lock");
	diner->forks = sem_open("/forks", O_CREAT, 0600, diner->nb_of_philo);
	diner->deads_lock = sem_open("/deads_lock", O_CREAT, 0600, 1);
	diner->write_lock = sem_open("/write_lock", O_CREAT, 0600, 1);
	diner->philos = malloc(sizeof(t_philo) * diner->nb_of_philo);
	if (!diner->philos)
		return (1);
	memset(diner->philos, 0, sizeof(t_philo) * diner->nb_of_philo);
	return (0);
}

int	ft_philo_init(t_diner *diner, char **argv, int argc)
{
	diner->ms_die = ft_atoi_philo(argv[2]);
	if (diner->nb_of_philo == 1)
		return (printf("Time = 0ms. Philosopher 1 has taken a fork\n"), \
			printf("Time = %ldms. Philosopher 1 is dead\n", diner->ms_die), 3);
	diner->ms_eat = ft_atoi_philo(argv[3]);
	diner->ms_sleep = ft_atoi_philo(argv[4]);
	diner->ms_think = diner->ms_eat * (diner->nb_of_philo % 2);
	if (diner->nb_of_philo % 2 == 1 && diner->ms_sleep > diner->ms_think)
		diner->ms_think -= diner->ms_sleep - diner->ms_think; 
	diner->dead_philos = 0;
	diner->start = ft_time() + 1000;
	diner->meals_needed = -1;
	if (argc == 6)
		diner->meals_needed = ft_atoi_philo(argv[5]);
	if (ft_init_forks(diner) != 0 || ft_init_threads_philosophers(diner) != 0)
		return (1);
	diner->i = -1;
	printf("\n	Loading...\n\n");
	while (++diner->i < diner->nb_of_philo)
	{
		if (pthread_create(&diner->philos[diner->i]->thread, \
			NULL, &philo_life, diner->philos[diner->i]) != 0)
			return (1);
	}
	return (0);
}
