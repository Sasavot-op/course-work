#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Car {
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
};

char* readLine() {
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), stdin)) return strdup("");
    buffer[strcspn(buffer, "\n")] = 0;
    return strdup(buffer);
}

void initCar(struct Car *c) {
    c->brand = NULL; c->model = NULL; c->color = NULL; c->technical_inspection = NULL;
    c->body_type = NULL; c->mileage = NULL; c->gear_box = NULL; c->fuel_type = NULL;
    c->owner = NULL; c->year = 0; c->price = 0; c->seats = 0;
}

void freeCar(struct Car *c) {
    if(c->brand) free(c->brand);
    if(c->model) free(c->model);
    if(c->color) free(c->color);
    if(c->technical_inspection) free(c->technical_inspection);
    if(c->body_type) free(c->body_type);
    if(c->mileage) free(c->mileage);
    if(c->gear_box) free(c->gear_box);
    if(c->fuel_type) free(c->fuel_type);
    if(c->owner) free(c->owner);
}

char* chooseBrand() {
    const char *brands[] = {"BMW", "Audi", "Toyota"};
    int n = 3;
    printf("Choose brand:\n");
    for(int i=0;i<n;i++) printf("%d - %s\n", i+1, brands[i]);
    int choice;
    do {
        printf("Your choice: ");
        scanf("%d",&choice); while(getchar()!='\n');
    } while(choice<1 || choice>n);
    return strdup(brands[choice-1]);
}

char* chooseModel(const char* brand) {
    const char *modelsBMW[] = {"X5","X6","M3"};
    const char *modelsAudi[] = {"A4","Q7","Q5"};
    const char *modelsToyota[] = {"Camry","RAV4","Corolla"};

    const char **models = NULL;
    int n = 0;

    if(strcmp(brand,"BMW")==0){models=modelsBMW;n=3;}
    else if(strcmp(brand,"Audi")==0){models=modelsAudi;n=3;}
    else if(strcmp(brand,"Toyota")==0){models=modelsToyota;n=3;}

    printf("Choose model:\n");
    for(int i=0;i<n;i++) printf("%d - %s\n", i+1, models[i]);
    int choice;
    do {
        printf("Your choice: ");
        scanf("%d",&choice); while(getchar()!='\n');
    } while(choice<1 || choice>n);

    return strdup(models[choice-1]);
}

void addCar(struct Car **cars, int *count) {
    struct Car *temp = realloc(*cars, (*count + 1) * sizeof(struct Car));
    if(!temp) { printf("Memory allocation failed!\n"); return; }
    *cars = temp;

    struct Car *c = &(*cars)[*count];
    initCar(c);

    c->brand = chooseBrand();
    c->model = chooseModel(c->brand);

    printf("Year: "); scanf("%d",&c->year); while(getchar()!='\n');
    printf("Price: "); scanf("%d",&c->price); while(getchar()!='\n');
    printf("Seats: "); scanf("%d",&c->seats); while(getchar()!='\n');
    printf("Color: "); c->color = readLine();
    printf("Technical Inspection: "); c->technical_inspection = readLine();
    printf("Body Type: "); c->body_type = readLine();
    printf("Mileage: "); c->mileage = readLine();
    printf("Gear Box: "); c->gear_box = readLine();
    printf("Fuel Type: "); c->fuel_type = readLine();
    printf("Owner: "); c->owner = readLine();

    (*count)++;
}

void printCar(struct Car c, int index) {
    printf("\n--- Car #%d ---\n", index + 1);
    printf("Brand: %s\nModel: %s\nYear: %d\nPrice: %d$\nSeats: %d\nColor: %s\nTechnical Inspection: %s\nBody Type: %s\nMileage: %s\nGear Box: %s\nFuel Type: %s\nOwner: %s\n",
        c.brand, c.model, c.year, c.price, c.seats,
        c.color, c.technical_inspection, c.body_type, c.mileage,
        c.gear_box, c.fuel_type, c.owner);
    printf("------------------------\n");
}

void editCar(struct Car *c) {
    free(c->brand); c->brand = chooseBrand();
    free(c->model); c->model = chooseModel(c->brand);
    printf("Year: "); scanf("%d",&c->year); while(getchar()!='\n');
    printf("Price: "); scanf("%d",&c->price); while(getchar()!='\n');
    printf("Seats: "); scanf("%d",&c->seats); while(getchar()!='\n');
    free(c->color); printf("Color: "); c->color = readLine();
    free(c->technical_inspection); printf("Technical Inspection: "); c->technical_inspection = readLine();
    free(c->body_type); printf("Body Type: "); c->body_type = readLine();
    free(c->mileage); printf("Mileage: "); c->mileage = readLine();
    free(c->gear_box); printf("Gear Box: "); c->gear_box = readLine();
    free(c->fuel_type); printf("Fuel Type: "); c->fuel_type = readLine();
    free(c->owner); printf("Owner: "); c->owner = readLine();
}

void deleteCar(struct Car **cars, int *count, int index) {
    if(index<0 || index>=*count) { printf("Invalid car number!\n"); return; }
    freeCar(&(*cars)[index]);
    for(int i=index;i<*count-1;i++) (*cars)[i] = (*cars)[i+1];
    (*count)--;
    if(*count>0) *cars = realloc(*cars, (*count)*sizeof(struct Car));
    else { free(*cars); *cars=NULL; }
}

void saveCarsToFile(struct Car *cars,int count,const char *filename) {
    FILE *f = fopen(filename,"w");
    if(!f) { printf("Cannot open file!\n"); return; }
    for(int i=0;i<count;i++) {
        fprintf(f,"--- Brand: %s ---\n", cars[i].brand);
        fprintf(f,"Model: %s\n", cars[i].model);
        fprintf(f,"Year: %d\n", cars[i].year);
        fprintf(f,"Price: %d$\n", cars[i].price);
        fprintf(f,"Seats: %d\n", cars[i].seats);
        fprintf(f,"Color: %s\n", cars[i].color);
        fprintf(f,"Technical Inspection: %s\n", cars[i].technical_inspection);
        fprintf(f,"Body Type: %s\n", cars[i].body_type);
        fprintf(f,"Mileage: %s\n", cars[i].mileage);
        fprintf(f,"Gear Box: %s\n", cars[i].gear_box);
        fprintf(f,"Fuel Type: %s\n", cars[i].fuel_type);
        fprintf(f,"Owner: %s\n", cars[i].owner);
        fprintf(f,"------------------------\n");
    }
    fclose(f);
    printf("Saved %d cars to %s\n", count, filename);
}

void loadCarsFromFile(struct Car **cars,int *count,const char *filename) {
    FILE *f = fopen(filename,"r");
    if(!f) return;
    char line[256];
    struct Car tempCar;
    initCar(&tempCar);
    int stage=0;
    while(fgets(line,sizeof(line),f)) {
        line[strcspn(line,"\n")]=0;
        if(strncmp(line,"--- Brand:",10)==0){
            if(stage==1){ // add previous car
                *cars = realloc(*cars, (*count+1)*sizeof(struct Car));
                (*cars)[*count] = tempCar;
                (*count)++;
                initCar(&tempCar);
            }
            tempCar.brand = strdup(line+11);
            stage=1;
        }
        else if(strncmp(line,"Model:",6)==0) tempCar.model=strdup(line+7);
        else if(strncmp(line,"Year:",5)==0) tempCar.year=atoi(line+6);
        else if(strncmp(line,"Price:",6)==0) tempCar.price=atoi(line+7);
        else if(strncmp(line,"Seats:",6)==0) tempCar.seats=atoi(line+7);
        else if(strncmp(line,"Color:",6)==0) tempCar.color=strdup(line+7);
        else if(strncmp(line,"Technical Inspection:",21)==0) tempCar.technical_inspection=strdup(line+22);
        else if(strncmp(line,"Body Type:",10)==0) tempCar.body_type=strdup(line+11);
        else if(strncmp(line,"Mileage:",8)==0) tempCar.mileage=strdup(line+9);
        else if(strncmp(line,"Gear Box:",9)==0) tempCar.gear_box=strdup(line+10);
        else if(strncmp(line,"Fuel Type:",10)==0) tempCar.fuel_type=strdup(line+11);
        else if(strncmp(line,"Owner:",6)==0) tempCar.owner=strdup(line+7);
    }
    if(stage==1){
        *cars = realloc(*cars, (*count+1)*sizeof(struct Car));
        (*cars)[*count] = tempCar;
        (*count)++;
    }
    fclose(f);
}

int main() {
    struct Car *cars=NULL; int count=0;
    loadCarsFromFile(&cars,&count,"cars.txt");
    int choice;

    do {
        printf("\nMenu:\n1-Add Car\n2-Edit Car\n3-Delete Car\n4-Show All Cars\n5-Save to File\n0-Exit\nChoice: ");
        scanf("%d",&choice); while(getchar()!='\n');

        if(choice==1) addCar(&cars,&count);
        else if(choice==2) {
            if(count==0){printf("No cars!\n"); continue;}
            int idx; printf("Enter car number (1-%d): ",count); scanf("%d",&idx); while(getchar()!='\n');
            if(idx>0 && idx<=count) editCar(&cars[idx-1]);
            else printf("Invalid car number!\n");
        }
        else if(choice==3) {
            if(count==0){printf("No cars!\n"); continue;}
            int idx; printf("Enter car number (1-%d): ",count); scanf("%d",&idx); while(getchar()!='\n');
            deleteCar(&cars,&count,idx-1);
        }
        else if(choice==4) {
            if(count==0){printf("No cars!\n"); continue;}
            for(int i=0;i<count;i++) printCar(cars[i],i);
        }
        else if(choice==5) saveCarsToFile(cars,count,"cars.txt");
        else if(choice!=0) printf("Invalid choice!\n");

    } while(choice!=0);

    for(int i=0;i<count;i++) freeCar(&cars[i]);
    free(cars);
    return 0;
}
