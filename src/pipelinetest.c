
int pipe_test(int *, char *const []);

int main_pipe(int argc, char const **argv)
{

    if (argc < 5)
    {
	printf("not enough arguments\n");
	return -1;
    }


    pid_t pid_1, pid_2;
    int pipe_1[2], pipe_2[2];

    if (pipe(pipe_1))
    {
	perror("pipe");
	return -2;
    }

    if (pipe(pipe_2))
    {
	perror("pipe");
	return -2;
    }

    pid_1 = fork();
    if (pid_1 == (pid_t) 0)
    {
	char *str[] = {(char *)argv[1], (char *)argv[2], NULL};

	pipe_test(pipe_1, str);
    }

    pid_2 = fork();
    if (pid_2 == (pid_t) 0)
    {
	char *str[] = {(char *)argv[3], (char *)argv[4], NULL};

	pipe_test(pipe_2, str);
    }

    char buf_1[_POSIX_PIPE_BUF], buf_2[_POSIX_PIPE_BUF];

    close(pipe_1[1]);
    close(pipe_2[1]);
    ssize_t buf_1_read, buf_2_read;

    while (buf_1_read = read(pipe_1[0], buf_1, _POSIX_PIPE_BUF)) {
	buf_2_read = read(pipe_2[0], buf_2, _POSIX_PIPE_BUF);

	if (buf_1_read != buf_2_read)
	{
	    printf("sizes unequal\n");
	    return EXIT_FAILURE;
	}
	for (int i = 0 ; i < (int) buf_1_read; i++)
	{
	    printf("%c", buf_1[i] ^ buf_2[i]);
	}
    }
    return EXIT_SUCCESS;
}


int pipe_test(int *pipe, char *const args[])
{
    close(pipe[0]);
    dup2(pipe[1], STDOUT_FILENO);
    execvp(args[0], args);
    perror("exec");
    _exit(EXIT_FAILURE);
}
