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

typedef struct s_diner			t_diner;
typedef struct s_philosophers	t_philo;

/* -------------------------------------------------------------------------- */
/* --------------------------- Files and functions -------------------------- */
/* -------------------------------------------------------------------------- */

/* --------------------------- Main.c --------------------------------------- */
/* The program's basic functions. ------------------------------------------- */
/* -------------------------------------------------------------------------- */

int		main(int argc, char **argv);
int		ft_usleep(time_t sleep_time, t_philo *philo);
time_t	ft_time_microsec(void);
time_t	ft_time(void);
void	ft_free_philo(t_diner *diner);

/* --------------------------- Init.c --------------------------------------- */
/* Necessary initializations to start the Diner. ---------------------------- */
/* -------------------------------------------------------------------------- */

int		ft_philo_init(t_diner *diner, char **argv, int argc);
int		ft_init_threads_philosophers(t_diner *diner);
int		ft_init_forks(t_diner *diner);
int		ft_parsing(int argc, char **argv);
time_t	ft_atoi_philo(const char *str);

/* --------------------------- Philosophers.c ------------------------------- */
/* Where the Philosophers eat, sleep, think and maybe die. ------------------ */
/* -------------------------------------------------------------------------- */

void	*philo_life(void *philo_void);
void	ft_eating(t_philo *philo);
void	ft_lock_unlock(pthread_mutex_t *mutex, int option, t_philo *philo);
void	ft_life_or_death(t_philo *philo);
void	ft_write_status(char *str, t_philo *philo);

/* -------------------------------------------------------------------------- */
/* --------------------------- Structures ----------------------------------- */
/* -------------------------------------------------------------------------- */

/* Philosophers' datas. ----------------------------------------------------- */

typedef struct s_philosophers
{
	t_diner					*diner;
	int						id;
	pthread_t				thread;
	int						meals_needed;
	int						has_eaten;
	int						last_supper_time;
	int						fork1;
	int						fork2;
	pthread_mutex_t			left_fork;
	pthread_mutex_t			right_fork;
}		t_philo;

/* Diner's data. ------------------------------------------------------------ */

typedef struct s_diner
{
	int				nb_of_philo;
	time_t			start;
	time_t			ms_die;
	time_t			ms_eat;
	time_t			ms_sleep;
	time_t			ms_think;
	int				meals_needed;
	t_philo			**philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	deads_lock;
	int				dead_philos;
	int				i;
	time_t			start_us;

}		t_diner;

#endif