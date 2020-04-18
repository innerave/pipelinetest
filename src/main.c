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
                if (mode == HELP) {
                        printf("Использование:\n");
                        return 0;
                }

                if (mode == EXECUTE) {
                                                      //  printf("%s \n %s \n %d", source, key, key_length);
                }
        }
        return 0;
}