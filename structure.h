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

char *readLine(void);
void freeCar(Car *c);
void printCar(const Car *c);
void printCarShort(const Car *c);

void addCarInteractive(void);
void deleteCarById(void);
void showAllCars(void);
void saveToFile();
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

void printHelp(const char *progname);
