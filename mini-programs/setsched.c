#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

#include <unistd.h>
#include <sys/resource.h>
#define SCHED_VTF 7
#define SCHED_NORMAL 0

void main(int argc, char **argv)
{
	int pid = atoi(argv[1]), priority;

	if(atoi(argv[2]) == 1)
	{
		priority = getpriority(PRIO_PROCESS, pid);
		
		if(priority != -1)
		{
			const struct sched_param sp = {
				.sched_priority = priority,
			};
	
			sched_setscheduler(pid, SCHED_VTF, &sp);
		}
		printf("New policy is : %d\n",sched_getscheduler(pid));
	}
	else
	{
		priority = getpriority(PRIO_PROCESS, pid);
		if(priority != -1)
		{
			const struct sched_param sp = {
        		        .sched_priority = priority,
	        	};

	        	sched_setscheduler(pid, SCHED_NORMAL, &sp);
		}
		printf("New policy is : %d\n",sched_getscheduler(pid));
	}
}
