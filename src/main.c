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
int main_pipe(char const *, char const *, int);

int main(int argc, char *argv[]) {
        int opt;
        int mode = EXECUTE;
        char *source = NULL;
		char *key = NULL;
		int key_length = 0;
        /*
         * optarg – указатель на текущий аргумент, если таковой имеется.
         * optind – индекс на следующий указатель argv, который будет обработан
         * при следующем вызове getopt().
         * optopt – нераспознанная опция.
         * argv[optind] – и далее – собранные файлы
         */
        while ((opt = getopt(argc, argv, ":s:k:n:h")) != -1) {
                switch (opt) {
                        case 's':
                                printf("Источник: %s\n", optarg);
                                source = optarg;
                                break;
                        case 'k':
                                printf("Ключ: %s\n", optarg);
                                key = optarg;
                                break;
                        case 'n':
                                printf("Длинна ключа: %s\n", optarg);
                                key_length = atoi(optarg);
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
            return 0;
        }
		
        if (mode == EXECUTE) {
            if (!source){
				printf("Нет источника");
				return -1;
			}
            if (!key){
				printf("Нет ключа");
				return -1;
			}
            if (!key_length){
				printf("Нет нет длинны ключа");
				return -1;
			}
			
        }
        return 0;
}

int main_pipe(char const *source, char const *key, int key_length)
{
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