#include "philo.h"


void *philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	printf("philosopher %d was born\n", philosopher->index);
	return (0);
}

void	get_args(char *argv[], t_args *args)
{
	(*args).number_of_philosophers = ft_atoi(argv[1]);
	(*args).time_to_die = ft_atoi(argv[2]);
	(*args).time_to_eat = ft_atoi(argv[3]);
	(*args).time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		(*args).number_of_meals = ft_atoi(argv[5]);
	else
		(*args).number_of_meals = -42;
	return ;
}

void	wait_philosophers(t_philosopher **philosophers, t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->number_of_philosophers)
		pthread_join(*((*philosophers)[i].thread), NULL);
}

void	init_philosophers(t_philosopher **philosophers, t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->number_of_philosophers)
	{
		(*philosophers)[i].args = args;
		(*philosophers)[i].index = i + 1;
		printf("firosofo %d\n", (*philosophers)[i].index);
		(*philosophers)[i].thread = (pthread_t *)malloc(sizeof(pthread_t));
	}
	i = -1;
	while (++i < args->number_of_philosophers)
	{
		pthread_create((*philosophers)[i].thread, NULL, &philosopher_routine, (void *)((*philosophers) + i));
	}
}

int main(int argc, char *argv[])
{
	t_args			args;
	t_philosopher	*philosophers;

	printf("hello world\n");
	if (argc < 5 || argc > 6)
		return (-1);
	get_args(argv, &args);
	philosophers = malloc(args.number_of_philosophers * sizeof(t_philosopher));
	init_philosophers(&philosophers, &args);
	wait_philosophers(&philosophers, &args);
	return (0);
}
