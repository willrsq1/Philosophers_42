/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 13:46:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/03/30 14:35:09 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	ft_lock_unlock(sem_t *forks, int option, t_philo *philo)
{
	if (option == 2)
	{
		if (sem_post(forks) == -1)
			printf("Error Sem Post\n");
	}
	ft_life_or_death(philo);
	sem_wait(philo->diner->deads_lock);
	if (philo->diner->dead_philos != 0)
	{
		sem_post(philo->diner->deads_lock);
		return ;
	}
	sem_post(philo->diner->deads_lock);
	if (option == 1)
	{
		if (sem_wait(forks) == -1)
			printf("Error Sem Wait\n");
	}
}

static void	ft_write_status(char *str, t_philo *philo)
{	
	ft_life_or_death(philo);
	sem_wait(philo->diner->write_lock);
	sem_wait(philo->diner->deads_lock);
	if (philo->diner->dead_philos == 0)
	{
		sem_post(philo->diner->deads_lock);
		printf("Time = %ldms. Philosopher %d %s\n", \
			ft_time() - philo->diner->start, philo->id + 1, str);
	}
	else
		sem_post(philo->diner->deads_lock);
	sem_post(philo->diner->write_lock);
}

static void	ft_eating(t_philo *philo)
{
	ft_lock_unlock(philo->diner->forks, 1, philo);
	ft_write_status("has taken a fork", philo);
	ft_lock_unlock(philo->diner->forks, 1, philo);
	ft_write_status("has taken a fork", philo);
	ft_write_status("is eating", philo);
	philo->last_supper_time = ft_time() - philo->diner->start;
	philo->has_eaten += 1;
	ft_usleep(philo->diner->ms_eat, philo);
	ft_lock_unlock(philo->diner->forks, 2, philo);
	ft_lock_unlock(philo->diner->forks, 2, philo);
	ft_write_status("is sleeping", philo);
	ft_usleep(philo->diner->ms_sleep, philo);
}

void	*philo_life(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	while (ft_time() < philo->diner->start)
		usleep(100);
	if (philo->id % 2 == 0)
	{
		ft_write_status("is thinking", philo);
		ft_usleep(philo->diner->ms_eat / 2, philo);
	}
	while (1)
	{
		ft_eating(philo);
		ft_write_status("is thinking", philo);
		ft_usleep(philo->diner->ms_think, philo);
		ft_life_or_death(philo);
		sem_wait(philo->diner->deads_lock);
		if (philo->diner->dead_philos != 0)
			return (sem_post(philo->diner->deads_lock), NULL);
		sem_post(philo->diner->deads_lock);
		if (philo->has_eaten == philo->meals_needed)
			break ;
	}
	return (ft_write_status("is done eating", philo), NULL);
}
