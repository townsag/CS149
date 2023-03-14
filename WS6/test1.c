#include "apue.h"

int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* parent */
		sleep(2);
		printf("inside parent pid: %d\n", getpid());
		exit(2);				/* terminate with exit status 2 */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* first child */
		sleep(4);
		printf("inside first child process pid: %d\n", getpid());
		abort();				/* terminate with core dump */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* second child */
		execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
		printf("shouldnt get here pid: %d\n", getpid());
		exit(7);				/* shouldn't get here */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* third child */
		sleep(8);
		printf("inside third child process pid: %d\n", getpid());
		exit(0);				/* normal exit */
	}

	sleep(6);					/* fourth child */
	printf("inside fourth child pid: %d\n", getpid());
	kill(getpid(), SIGKILL);	/* terminate w/signal, no core dump */
	exit(6);					/* shouldn't get here */
}
