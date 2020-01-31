#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *read_var(void *var)
{
	int toprint = (int)var;
	char *msg;
	sprintf(msg,"%d",toprint);
	printf("The father's thread reads var = %d\n", atoi(msg));
}

int main(void)
{
	int var = 10, thread;
	pid_t father = getpid(), son = fork();
	pthread_t threadf, threads;

/*	var = 10;
	void *test = (void *)var;
	int retest = (int)test;
	char *msg;// = itoa(var, );
	sprintf(msg,"%d",retest);
        printf("The son's threadv reads var = %d\n", atoi(msg));*/

	if(son == 0) // on est dans le fils
	{
		thread = pthread_create(&threads, NULL, read_var, (void *)var);
                printf("%d\n",thread);

		while(1)
		{
			printf("The son reads var = %d\n", var);
			sleep(5);
		}
	}
	else // on est dans le père
	{
		if(pthread_create(&threadf, NULL, read_var, (void *)var))
                        printf("Thread_f\n");
                while(1)
                {
			var = -10;
                        printf("The father reads var = %d\n", var);
                        sleep(5);
                }
        }
	
	return 0;
}
