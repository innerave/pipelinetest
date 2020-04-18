#include "archiver.h"

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
        while ((opt = getopt(argc, argv, ":e:c:h")) != -1) {
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
                                key_length = optarg;
                                break;
                        case 'h':
                                mode = HELP;
                                break;
                        case ':':
                                printf("Требуется имя архива\n");
                                break;
                        case '?':
                                printf("Неизвестная команда: %c\n", optopt);
                                break;
                        default:
                                abort();
                }
                if (mode == HELP) {
                        printf("Использование:\n");
                        printf("./arch -e {имя_архива} "
                               "извлечение из архива\n");
                        printf("./arch -c {имя_архива} {файл1} {файл2} {файл3}"
                               "... создание архива\n");
                        return 0;
                }

                if (mode == EXECUTE) {
                        if (!source) {
							printf("Не указан источник");
							return -1;
						}
                }
        }
        return 0;
}