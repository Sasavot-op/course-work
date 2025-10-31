#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BRANDS 10
#define MAX_MODELS 10

typedef struct {
    char brand[50];
    char model[50];
    int year;
    int price;
    int seats;
    char color[100];
    char technical_inspection[20];
    char body_type[20];
    char mileage[50];
    char gear_box[20];
    char fuel_type[20];
    char owner[50];
    int id;
} Car;

extern Car* cars;
extern int car_count;
extern int next_id;

char* readLine();
int chooseOption(const char* prompt, char* options[], int n);
void addCar();
void deleteCar();
void showCars();
void saveToFile();