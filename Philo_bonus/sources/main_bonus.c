/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 21:34:32 by marvin            #+#    #+#             */
/*   Updated: 2023/03/23 21:34:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	ft_life_or_death(t_philo *philo)
{
	if (ft_time() - philo->diner->start > \
		philo->diner->ms_die + philo->last_supper_time)
	{
		sem_wait(philo->diner->deads_lock);
		philo->diner->dead_philos += 1;
		if (philo->diner->dead_philos == 1)
		{
			usleep(100);
			printf("Time = %ldms. Philosopher %d %s\n", \
				ft_time() - philo->diner->start, philo->id + 1, "has died");
		}
		sem_post(philo->diner->deads_lock);
	}
}

int	ft_usleep(time_t sleep_time, t_philo *philo)
{
	time_t	end_sleep;

	ft_life_or_death(philo);
	sem_wait(philo->diner->deads_lock);
	if (philo->diner->dead_philos != 0)
		return (sem_post(philo->diner->deads_lock), 0);
	sem_post(philo->diner->deads_lock);
	end_sleep = ft_time() + sleep_time;
	while (ft_time() < end_sleep)
	{
		ft_life_or_death(philo);
		sem_wait(philo->diner->deads_lock);
		if (philo->diner->dead_philos != 0)
			return (sem_post(philo->diner->deads_lock), 0);
		sem_post(philo->diner->deads_lock);
		ft_life_or_death(philo);
		usleep(1000);
	}
	ft_life_or_death(philo);
	return (0);
}

time_t	ft_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void	ft_free_philo(t_diner *diner)
{
	int	i;

	i = -1;
	sem_close(diner->deads_lock);
	sem_close(diner->write_lock);
	sem_close(diner->forks);
	if (diner->philos)
	{
		if (diner->philos[0])
		{
			while (diner->philos[++i])
			{
				if (diner->philos[i])
					free(diner->philos[i]);
			}
		}
	}
	if (diner->philos)
		free(diner->philos);
	free(diner);
	return ;
}

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
		return (ft_free_philo(diner), 1);
	i = -1;
	while (++i < diner->nb_of_philo)
		pthread_join(diner->philos[i]->thread, NULL);
	ft_free_philo(diner);
	return (0);
}
