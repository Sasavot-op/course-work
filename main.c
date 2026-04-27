#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char *brand;
    char *model;
    int year;
    int price;
    int seats;
    char *color;
    char *technical_inspection;
    char *body_type;
    char *mileage;
    char *gear_box;
    char *fuel_type;
    char *owner;
} Car;

extern Car *cars;
extern int car_count;
extern int next_id;

size_t my_strlen(const char *s);
char *my_strdup(const char *s);
int my_strcmp(const char *a, const char *b);

char *readLine(void);
void freeCar(Car *c);
void printCar(const Car *c);
void printCarShort(const Car *c);

void addCarInteractive(void);
void deleteCarById(void);
void showAllCars(void);
void saveToFile(const char *filename);
int loadFromFile(const char *filename);
void generateTestData(int n);

void sortCarsByPrice(void);
void sortCarsByYear(void);
void sortCarsById(void);
Car *filterByBrand(const char *brand, int *out_count);
Car *filterByModel(const char *model, int *out_count);
Car *filterByPriceRange(int minp, int maxp, int *out_count);
Car *filterByYearRange(int miny, int maxy, int *out_count);
void mapToFile(const Car *list, int list_count, const char *filename);
void freeCarArray(Car *arr, int n);

void printHelp(const char *progname) {
    printf("Использование: %s [ФАЙЛ] КОМАНДА [АРГУМЕНТЫ]\n", progname);
    printf("Файл данных (по умолчанию cars.db) загружается в начале и сохраняется в конце.\n\n");
    printf("Команды:\n");
    printf("  --help                Показать эту справку\n");
    printf("  --add                 Добавить машину (интерактивно)\n");
    printf("  --delete ID           Удалить машину по ID\n");
    printf("  --list                Показать все машины (кратко)\n");
    printf("  --show ID             Показать подробно одну машину\n");
    printf("  --filter-brand НАЗВАНИЕ      Фильтр по марке\n");
    printf("  --filter-model НАЗВАНИЕ      Фильтр по модели\n");
    printf("  --filter-price МИН МАКС       Фильтр по цене\n");
    printf("  --filter-year МИН МАКС        Фильтр по году\n");
    printf("  --gen-test N          Сгенерировать N тестовых машин\n");
    printf("  --save [ФАЙЛ]         Сохранить (если нет файла — используется текущий)\n");
    printf("\nПримеры:\n");
    printf("  %s cars.db --add\n", progname);
    printf("  %s --gen-test 10\n", progname);
    printf("  %s --filter-brand BMW\n", progname);
}

int main(int argc, char **argv) {
    const char *dbfile = "cars.db";
    const char *command = NULL;
    int arg_start = 1;

    if (argc >= 2 && (my_strcmp(argv[1], "--help") == 0 || my_strcmp(argv[1], "-h") == 0)) {
        printHelp(argv[0]);
        return 0;
    }

    if (argc >= 2 && argv[1][0] != '-') {
        dbfile = argv[1];
        arg_start = 2;
    }

    loadFromFile(dbfile);

    if (argc <= arg_start) {
        printHelp(argv[0]);
        saveToFile(dbfile);
        for (int i = 0; i < car_count; i++) freeCar(&cars[i]);
        free(cars);
        return 0;
    }

    command = argv[arg_start];

    if (my_strcmp(command, "--add") == 0) {
        addCarInteractive();
    }
    else if (my_strcmp(command, "--delete") == 0) {
        if (arg_start + 1 >= argc) {
            printf("Ошибка: нужен ID для удаления\n");
        } else {
            int id = atoi(argv[arg_start + 1]);
            int idx = -1;
            for (int i = 0; i < car_count; i++) {
                if (cars[i].id == id) {
                    idx = i;
                    break;
                }
            }
            if (idx == -1) {
                printf("ID %d не найден\n", id);
            } else {
                freeCar(&cars[idx]);
                for (int j = idx; j < car_count - 1; j++) cars[j] = cars[j + 1];
                car_count--;
                if (car_count == 0) {
                    free(cars);
                    cars = NULL;
                } else {
                    Car *tmp = realloc(cars, sizeof(Car) * car_count);
                    if (tmp) cars = tmp;
                }
                printf("Удалена машина с ID %d\n", id);
            }
        }
    }
    else if (my_strcmp(command, "--list") == 0) {
        if (car_count == 0) {
            printf("Нет машин\n");
        } else {
            for (int i = 0; i < car_count; i++) {
                printCarShort(&cars[i]);
            }
        }
    }
    else if (my_strcmp(command, "--show") == 0) {
        if (arg_start + 1 >= argc) {
            printf("Ошибка: нужен ID\n");
        } else {
            int id = atoi(argv[arg_start + 1]);
            int found = 0;
            for (int i = 0; i < car_count; i++) {
                if (cars[i].id == id) {
                    printCar(&cars[i]);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("ID %d не найден\n", id);
        }
    }
    else if (my_strcmp(command, "--filter-brand") == 0) {
        if (arg_start + 1 >= argc) {
            printf("Ошибка: нужна марка\n");
        } else {
            char *brand = my_strdup(argv[arg_start + 1]);
            int out_count = 0;
            Car *res = filterByBrand(brand, &out_count);
            if (out_count == 0) {
                printf("Машин с маркой '%s' не найдено\n", brand);
            } else {
                for (int i = 0; i < out_count; i++) {
                    printCarShort(&res[i]);
                }
            }
            freeCarArray(res, out_count);
            free(brand);
        }
    }
    else if (my_strcmp(command, "--filter-model") == 0) {
        if (arg_start + 1 >= argc) {
            printf("Ошибка: нужна модель\n");
        } else {
            char *model = my_strdup(argv[arg_start + 1]);
            int out_count = 0;
            Car *res = filterByModel(model, &out_count);
            if (out_count == 0) {
                printf("Машин с моделью '%s' не найдено\n", model);
            } else {
                for (int i = 0; i < out_count; i++) {
                    printCarShort(&res[i]);
                }
            }
            freeCarArray(res, out_count);
            free(model);
        }
    }
    else if (my_strcmp(command, "--filter-price") == 0) {
        if (arg_start + 2 >= argc) {
            printf("Ошибка: нужны MIN и MAX цена\n");
        } else {
            int minp = atoi(argv[arg_start + 1]);
            int maxp = atoi(argv[arg_start + 2]);
            int out_count = 0;
            Car *res = filterByPriceRange(minp, maxp, &out_count);
            if (out_count == 0) {
                printf("Машин в цене от %d до %d не найдено\n", minp, maxp);
            } else {
                for (int i = 0; i < out_count; i++) {
                    printCarShort(&res[i]);
                }
            }
            freeCarArray(res, out_count);
        }
    }
    else if (my_strcmp(command, "--filter-year") == 0) {
        if (arg_start + 2 >= argc) {
            printf("Ошибка: нужны MIN и MAX год\n");
        } else {
            int miny = atoi(argv[arg_start + 1]);
            int maxy = atoi(argv[arg_start + 2]);
            int out_count = 0;
            Car *res = filterByYearRange(miny, maxy, &out_count);
            if (out_count == 0) {
                printf("Машин в годах от %d до %d не найдено\n", miny, maxy);
            } else {
                for (int i = 0; i < out_count; i++) {
                    printCarShort(&res[i]);
                }
            }
            freeCarArray(res, out_count);
        }
    }
    else if (my_strcmp(command, "--gen-test") == 0) {
        if (arg_start + 1 >= argc) {
            printf("Ошибка: нужен N (количество тестовых машин)\n");
        } else {
            int n = atoi(argv[arg_start + 1]);
            if (n > 0) generateTestData(n);
            else printf("Ошибка: N должно быть больше 0\n");
        }
    }
    else if (my_strcmp(command, "--save") == 0) {
        if (arg_start + 1 >= argc) {
            saveToFile(dbfile);
        } else {
            saveToFile(argv[arg_start + 1]);
        }
    }
    else {
        printf("Неизвестная команда: %s\n", command);
        printHelp(argv[0]);
    }

    saveToFile(dbfile);

    for (int i = 0; i < car_count; i++) freeCar(&cars[i]);
    free(cars);
    cars = NULL;
    car_count = 0;

    return 0;
}