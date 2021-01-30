# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>
# include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
# include <sys/wait.h>


int		child(int *per)
{
	printf("in child per = %d\n", *per);
	return (0);
}

int		parent(int *per)
{
	*per = 10;
	printf("in parend = %d\n", *per);
}

int		main(void)
{
	int	m, i;
	pid_t	thread;

	m = 5;
	i = 0;
	while (i < 3)
	{
		if ((thread = fork()) < 0)
			return (0);//ошибка
		else if (thread == 0)
		{
			if (!((int)child(&m)))
				return (1);
			//дочерний
		}
		else
		{
			parent(&m);
			waitpid(-1, NULL, WUNTRACED);

			//родитель
		}
		i++;
	}
	return (0);
}