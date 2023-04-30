/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 13:39:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/08 13:39:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/* -------------------------------------------------------------------------- */
/* ---- MAIN ---------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Launches everything. ----------------------------------------------------- */
/* The call to ft_philo_init initiates the Philosophers' diner. ------------- */
/* Destroy the mutexes and frees everyting. --------------------------------- */
/* -------------------------------------------------------------------------- */

int	main(int argc, char **argv)
{
	int		i;
	t_diner	*diner;

	if (ft_parsing(argc, argv) != 0)
		return (1);
	diner = malloc(sizeof(t_diner));
	if (!diner)
		return (1);
	diner->nb_of_philo = ft_atoi_philo(argv[1]);
	i = ft_philo_init(diner, argv, argc);
	if (i == 3)
		return (free(diner), 0);
	if (i != 0)
		return (write(1, "Error\n", 6), 1);
	i = -1;
	while (++i < diner->nb_of_philo)
		pthread_join(diner->philos[i]->thread, NULL);
	pthread_mutex_destroy(&diner->write_lock);
	pthread_mutex_destroy(&diner->deads_lock);
	i = -1;
	while (++i < diner->nb_of_philo)
		pthread_mutex_destroy(&diner->forks[i]);
	ft_free_philo(diner);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_USLEEP ----------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Waits for X milliseconds. ------------------------------------------------ */
/* Before, after, and while waiting (using a loop of small usleep calls), --- */
/* Checks if the Philosophers is still alive with life_or_death function. --- */
/* Returns without completing the waiting time if a Philosopher is dead. ---- */
/* -------------------------------------------------------------------------- */

int	ft_usleep(time_t sleep_time, t_philo *philo)
{
	time_t	end_sleep;

	ft_life_or_death(philo);
	pthread_mutex_lock(&philo->diner->deads_lock);
	if (philo->diner->dead_philos != 0)
		return (pthread_mutex_unlock(&philo->diner->deads_lock), 0);
	pthread_mutex_unlock(&philo->diner->deads_lock);
	end_sleep = ft_time() + sleep_time;
	while (ft_time() < end_sleep)
	{
		ft_life_or_death(philo);
		pthread_mutex_lock(&philo->diner->deads_lock);
		if (philo->diner->dead_philos != 0)
			return (pthread_mutex_unlock(&philo->diner->deads_lock), 0);
		pthread_mutex_unlock(&philo->diner->deads_lock);
		usleep(100);
		ft_life_or_death(philo);
	}
	ft_life_or_death(philo);
	return (0);
}
/* -------------------------------------------------------------------------- */
/* ---- FT_TIME_MICROSEC ---------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Calls gettimeofday to get the time. -------------------------------------- */
/* Returns a more precise timestamp than FT_TIME. --------------------------- */
/* Returns a measure in milliseconds. --------------------------------------- */
/* -------------------------------------------------------------------------- */

time_t	ft_time_microsec(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000) + (tv.tv_usec));
}

/* -------------------------------------------------------------------------- */
/* ---- FT_TIME ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Calls gettimeofday to get the time. -------------------------------------- */
/* Returns a measure in milliseconds. --------------------------------------- */
/* -------------------------------------------------------------------------- */

time_t	ft_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* -------------------------------------------------------------------------- */
/* ---- FT_FREE_PHILO ------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Frees everything allocated. ---------------------------------------------- */
/* -------------------------------------------------------------------------- */

void	ft_free_philo(t_diner *diner)
{
	int	i;

	i = -1;
	while (diner->philos[++i])
		free(diner->philos[i]);
	if (diner->philos)
		free(diner->philos);
	if (diner->forks)
		free(diner->forks);
	free(diner);
}
