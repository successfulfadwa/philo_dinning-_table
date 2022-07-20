/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fadwa <fadwa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 05:32:17 by fadwa             #+#    #+#             */
/*   Updated: 2022/07/19 11:49:19 by fadwa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void *only_one(t_philo *philo)
{
	//only_one_fork_situation;
	pthread_mutex_lock(&philo->setup_philo->forks[philo->left_fork]);
	print_event(philo,"has taken a fork");
	pthread_mutex_unlock(&philo->setup_philo->forks[philo->left_fork]);
	print_event(philo, "died");
	//once
	philo->setup_philo->is_died=1;
	return(NULL);
}
void	eating(t_philo *philo)
{
		pthread_mutex_lock(&philo->setup_philo->forks[philo->left_fork]);
		pthread_mutex_lock(&philo->setup_philo->forks[philo->right_fork]);
		print_action(philo, "has taken a fork");
		print_action(philo, "has taken a fork");
		print_action(philo, "is eating");
		//last meal klaa
		philo->last_meal = get_time();
		// multiply number *1000 to convert to micro sec time to eat
		usleep(philo->setup_philo->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->setup_philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->setup_philo->forks[philo->right_fork]);

}

void *routine(void *ptr)
{
	t_philo *philo;
	philo = (t_philo*)ptr;
	if(philo->setup_philo->n_philos==1)
	return(only_one(philo));
	// so instead kol thread ytsna akhor ankhliw joj threads ykhdmo f waqt qrib for tak forks ..
	if (philo->philo_id % 2 == 0)
		usleep(1600);
	while (philo->setup_philo->is_died != 1)
	{
		eating(philo);
		print_event(philo, "is sleeping");
		// multiply number *1000 to convert to micro sec
		usleep(philo->setup_philo->time_to_sleep * 1000);
		print_event(philo, "is thinking");
		philo->n_meals++;


	}
	return(NULL);
}
//cqnt declqre t_direct bczof syn
void *check_rdie(void *ptr)
{
	int i ;
	t_philo *p;

	p=(t_philo*)ptr;
	while(!check_meals(p))
	{
		i=0;
		while (i < p->setup_philo->n_philos)
		{
			if (get_time() - p[i].last_meal > p->setup_philo->time_to_die)
			{
				print_event(&p[i], "died");
				p->setup_philo->is_died = 1;
				return (NULL);
			}
			i++;
		}
		//?
		usleep(1000);

	}
		return (NULL);



}


void	start_dinner(t_philo *philo)
{
	int i;
	pthread_t checker;

	i=0;
	philo->setup_philo->start_time=get_time();
	//start timefor every philo 1by1
	while(i<philo->setup_philo->n_philos)
	{
		pthread_create(&philo[i].thread,NULL,&routine,&philo[i]);
		i++;
	}
		pthread_create(&checker, NULL, &check_rdie, philo);
		i = 0;
		while (i < philo->setup_philo->n_philos)
		{
			pthread_join(philo[i].thread, NULL);
			i++;
		}
		pthread_join(checker, NULL);

}