/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:46:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/10 20:38:49 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/* -------------------------------------------------------------------------- */
/* ---- FT_PHILO_LIFE ------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Start the Diner at the same time for all Philosophers. ------------------- */
/* While the Philosopher is alive and hasn't eaten enough, keeps going. ----- */
/* -------------------------------------------------------------------------- */

void	*philo_life(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	usleep((philo->diner->start_us - ft_time_microsec()) * 0.8);
	while (philo->diner->start_us > ft_time_microsec())
		usleep(10);
	while (1)
	{
		ft_eating(philo);
		ft_write_status("is thinking", philo);
		ft_usleep(philo->diner->ms_think, philo);
		ft_life_or_death(philo);
		pthread_mutex_lock(&philo->diner->deads_lock);
		if (philo->diner->dead_philos != 0)
			return (pthread_mutex_unlock(&philo->diner->deads_lock), NULL);
		pthread_mutex_unlock(&philo->diner->deads_lock);
		if (philo->has_eaten == philo->meals_needed)
			break ;
	}
	return (ft_write_status("is done eating", philo), NULL);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_EATING ----------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Picks up forks (locks its mutexes), write status and waits. -------------- */
/* -------------------------------------------------------------------------- */

void	ft_eating(t_philo *philo)
{
	if (philo->has_eaten == 0 && philo->id % 2 == 0)
	{
		ft_write_status("is thinking", philo);
		ft_usleep(philo->diner->ms_eat, philo);
	}
	ft_lock_unlock(&philo->diner->forks[philo->fork1], 1, philo);
	ft_write_status("has taken a fork", philo);
	ft_lock_unlock(&philo->diner->forks[philo->fork2], 1, philo);
	ft_write_status("has taken a fork", philo);
	ft_write_status("is eating", philo);
	philo->last_supper_time = ft_time() - philo->diner->start;
	philo->has_eaten += 1;
	ft_usleep(philo->diner->ms_eat, philo);
	ft_lock_unlock(&philo->diner->forks[philo->fork1], 2, philo);
	ft_lock_unlock(&philo->diner->forks[philo->fork2], 2, philo);
	ft_write_status("is sleeping", philo);
	ft_usleep(philo->diner->ms_sleep, philo);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_LOCK_UNLOCK ------------------------------------------------------ */
/* -------------------------------------------------------------------------- */
/* Locks or unlocks. -------------------------------------------------------- */
/* Checks if the Philo is still alive if he gets stuck waiting for a fork. -- */
/* -------------------------------------------------------------------------- */

void	ft_lock_unlock(pthread_mutex_t *mutex, int option, t_philo *philo)
{
	if (option == 2)
		pthread_mutex_unlock(mutex);
	ft_life_or_death(philo);
	pthread_mutex_lock(&philo->diner->deads_lock);
	if (philo->diner->dead_philos != 0)
	{
		pthread_mutex_unlock(&philo->diner->deads_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->diner->deads_lock);
	if (option == 1)
		pthread_mutex_lock(mutex);
}

/* -------------------------------------------------------------------------- */
/* ---- FT_LIFE_OR_DEATH ---------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Checks that the Philosopher hasn't died since last eating. --------------- */
/* -------------------------------------------------------------------------- */

void	ft_life_or_death(t_philo *philo)
{
	if (ft_time() - philo->diner->start > \
		philo->diner->ms_die + philo->last_supper_time)
	{
		pthread_mutex_lock(&philo->diner->deads_lock);
		philo->diner->dead_philos += 1;
		if (philo->diner->dead_philos == 1)
		{
			usleep(10);
			printf("Time = %ldms. Philosopher %d %s\n", \
				ft_time() - philo->diner->start, philo->id + 1, "has died");
		}
		pthread_mutex_unlock(&philo->diner->deads_lock);
	}
}

/* -------------------------------------------------------------------------- */
/* ---- FT_WRITE_STATUS ----------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* Writes in the output the status change of a Philosopher. ----------------- */
/* Doesn't write if a Philosopher is dead. ---------------------------------- */
/* -------------------------------------------------------------------------- */

void	ft_write_status(char *str, t_philo *philo)
{
	ft_life_or_death(philo);
	pthread_mutex_lock(&philo->diner->write_lock);
	pthread_mutex_lock(&philo->diner->deads_lock);
	if (philo->diner->dead_philos == 0)
	{
		pthread_mutex_unlock(&philo->diner->deads_lock);
		printf("Time = %ldms. Philosopher %d %s\n", ft_time() \
			- philo->diner->start, philo->id + 1, str);
	}
	else
		pthread_mutex_unlock(&philo->diner->deads_lock);
	pthread_mutex_unlock(&philo->diner->write_lock);
	ft_life_or_death(philo);
}
