#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>

enum Mode {
	HELP = 1, EXECUTE
};

int pipe_test(int *, char *const []);
int main_pipe(char const *, char const *);

int main(int argc, char *argv[]) 
{
	int opt;
	int mode = EXECUTE;
	char *source = NULL;
	char *key = NULL;
	/*
	 * optarg – указатель на текущий аргумент, если таковой имеется.
	 * optind – индекс на следующий указатель argv, который будет обработан
	 * при следующем вызове getopt().
	 * optopt – нераспознанная опция.
	 * argv[optind] – и далее – собранные файлы
	 */
	while ((opt = getopt(argc, argv, ":s:k:h")) != -1) {
		switch (opt) {
			case 's':
				// printf("Источник: %s\n", optarg);
				source = optarg;
				break;
			case 'k':
				// printf("Ключ: %s\n", optarg);
				key = optarg;
				break;
			case 'h':
				mode = HELP;
				break;
			case ':':
				printf("Требуются дополнительные аргументы\n");
				break;
			case '?':
				printf("Неизвестная команда: %c\n", optopt);
				break;
			default:
				abort();
		}
	}
		if (mode == HELP) {
	    printf("Использование:\n");
	    printf("pipetest -s <cmd for source> -k <cmd for key>\n");
	    return 0;
	}

	if (mode == EXECUTE) {
	    if (!source) {
				printf("Нет источника");
				return -1;
			}
	    if (!key) {
				printf("Нет ключа");
				return -1;
			}
			return main_pipe(source, key);
	}
	return 0;
}

int main_pipe(char const *source, char const *key)
{
    char source_copy[_POSIX_ARG_MAX];
    char key_copy[_POSIX_ARG_MAX];
	char *source_parsed[20];
	char *key_parsed[20];
	int count = 0;

    strcpy(source_copy, source);
    strcpy(key_copy, key);

	char *pch = strtok(source_copy, " ");

	while (pch != NULL) //пока есть лексемы
	{
		source_parsed[count] = pch;
		pch = strtok(NULL, " ");
		count++;
	}

	count = 0;
	pch = strtok(key_copy, " ");
	while (pch != NULL) //пока есть лексемы
	{
		key_parsed[count] = pch;
		pch = strtok(NULL, " ");
		count++;
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
	   pipe_test(pipe_1, source_parsed);
    }

    pid_2 = fork();
    if (pid_2 == (pid_t) 0)
    {
	   pipe_test(pipe_2, key_parsed);
    }

    char buf_1[_POSIX_PIPE_BUF], buf_2[_POSIX_PIPE_BUF];

    close(pipe_1[1]);
    close(pipe_2[1]);
    ssize_t buf_1_read, buf_2_read;

    while (1) {
		buf_1_read = read(pipe_1[0], buf_1, _POSIX_PIPE_BUF);
		buf_2_read = read(pipe_2[0], buf_2, _POSIX_PIPE_BUF);

		if (!buf_1 || !buf_2) {
		    perror("read");
		    return EXIT_FAILURE;
		}
		if (buf_1_read != buf_2_read)
		{
		    printf("sizes unequal\n");
		    return EXIT_FAILURE;
		}
		if (buf_1_read == 0) break;
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

