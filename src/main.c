#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pipe_test(int *);

int main(int argc, char const *argv[])
{
	
	if (argc < 3) return -1; //not enough arguments


	pid_t pid_1, pid_2;
  	int pipe_1[2], pipe_2[2];

  	/* Create the pipe. */
  	if (pipe(pipe_1))
    {
      perror("pipe");
      return -2;
    }

    /* Create the pipe. */
  	if (pipe(pipe_2))
    {
      perror("pipe");
      return -2;
    }

    pid_1 = fork();
    if (pid_1 == (pid_t) 0)
    {
    	pipe_test(pipe_1);
    /*should not exit if state*/
    }

    pid_2 = fork();
    if (pid_2 == (pid_t) 0)
    {
    	pipe_test(pipe_2);
    /*should not exit if state*/
    }

    /* main process (probably... but who knows...) */


	return 0;
}


int pipe_test(int *pipe)
{
	/* here will be exec cat */
	/* and smth with pipes */
	_exit(EXIT_FAILURE);
}