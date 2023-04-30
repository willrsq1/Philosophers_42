/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 20:16:19 by wruet-su          #+#    #+#             */
/*   Updated: 2023/03/26 20:16:19 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdint.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

typedef struct s_diner	t_diner;

typedef struct s_philosophers
{
	t_diner			*diner;
	int				id;
	pthread_t		thread;
	int				meals_needed;
	int				has_eaten;
	int				last_supper_time;
}		t_philo;

typedef struct s_diner
{
	int				nb_of_philo;
	time_t			start;
	time_t			ms_die;
	time_t			ms_eat;
	time_t			ms_sleep;
	int				ms_think;
	int				meals_needed;
	t_philo			**philos;
	sem_t			*forks;
	sem_t			*write_lock;
	sem_t			*deads_lock;
	int				dead_philos;
	int				i;
}		t_diner;

time_t	ft_time(void);
int		ft_parsing(int argc, char **argv);
time_t	ft_atoi_philo(const char *str);
int		ft_philo_init(t_diner *diner, char **argv, int argc);
void	*philo_life(void *philo_void);
void	ft_life_or_death(t_philo *philo);
int		ft_usleep(time_t time, t_philo *philo);

#endif