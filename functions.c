#include "structure.h"

Car* cars = NULL;
int car_count = 0;
int next_id = 1;

char* readLine() {
    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return strdup(buffer);
}

int chooseOption(const char* prompt, char* options[], int n) {
    int choice = 0;
    printf("%s\n", prompt);
    for(int i=0;i<n;i++)
        printf("%d - %s\n", i+1, options[i]);
    printf("Your choice: ");
    scanf("%d",&choice);
    while(getchar()!='\n');
    if(choice<1 || choice>n) return chooseOption(prompt, options, n);
    return choice;
}

void addCar() {
    cars = realloc(cars, sizeof(Car)*(car_count+1));
    Car* c = &cars[car_count];

    char* brands[] = {"BMW","Audi","Toyota"};
    int brand_choice = chooseOption("Choose brand:", brands, 3);
    strcpy(c->brand, brands[brand_choice-1]);

    char* bmw_models[] = {"X5","X6","M3"};
    char* audi_models[] = {"A4","Q7","A6"};
    char* toyota_models[] = {"Camry","RAV4","Corolla"};

    if(strcmp(c->brand,"BMW")==0){
        int model_choice = chooseOption("Choose model:", bmw_models,3);
        strcpy(c->model,bmw_models[model_choice-1]);
    } else if(strcmp(c->brand,"Audi")==0){
        int model_choice = chooseOption("Choose model:", audi_models,3);
        strcpy(c->model,audi_models[model_choice-1]);
    } else {
        int model_choice = chooseOption("Choose model:", toyota_models,3);
        strcpy(c->model,toyota_models[model_choice-1]);
    }

    printf("Year: "); scanf("%d",&c->year); while(getchar()!='\n');
    printf("Price: "); scanf("%d",&c->price); while(getchar()!='\n');
    printf("Seats: "); scanf("%d",&c->seats); while(getchar()!='\n');
    printf("Color: "); strcpy(c->color, readLine());
    printf("Technical Inspection: "); strcpy(c->technical_inspection, readLine());

    char* body_types[] = {"Sedan","SUV","Coupe"};
    int bt = chooseOption("Choose body type:", body_types,3);
    strcpy(c->body_type, body_types[bt-1]);

    printf("Mileage: "); strcpy(c->mileage, readLine());

    char* gear_boxes[] = {"Auto","Manual"};
    int gb = chooseOption("Choose gear box:", gear_boxes,2);
    strcpy(c->gear_box, gear_boxes[gb-1]);

    char* fuels[] = {"Petrol","Diesel"};
    int f = chooseOption("Choose fuel type:", fuels,2);
    strcpy(c->fuel_type, fuels[f-1]);

    printf("Owner: "); strcpy(c->owner, readLine());

    c->id = next_id++;
    car_count++;
}

void deleteCar() {
    if(car_count==0){ printf("No cars!\n"); return; }
    printf("Enter car ID to delete: ");
    int id; scanf("%d",&id); while(getchar()!='\n');
    int found=0;
    for(int i=0;i<car_count;i++){
        if(cars[i].id==id){
            for(int j=i;j<car_count-1;j++)
                cars[j]=cars[j+1];
            car_count--;
            cars = realloc(cars,sizeof(Car)*car_count);
            found=1;
            printf("Car deleted!\n");
            break;
        }
    }
    if(!found) printf("ID not found!\n");
}

int comparePrice(const void* a,const void* b){
    Car* c1=(Car*)a;
    Car* c2=(Car*)b;
    return c1->price-c2->price;
}

void showCars(){
    if(car_count==0){ printf("No cars!\n"); return; }
    qsort(cars, car_count, sizeof(Car), comparePrice);

    char printed_brands[MAX_BRANDS][50]; int brand_count=0;

    for(int i=0;i<car_count;i++){
        int brand_index=-1;
        for(int j=0;j<brand_count;j++)
            if(strcmp(printed_brands[j],cars[i].brand)==0){ brand_index=j; break; }
        if(brand_index==-1){
            strcpy(printed_brands[brand_count],cars[i].brand);
            brand_index=brand_count++;
            printf("=== %s ===\n\n", cars[i].brand);
        }

        int model_already=0;
        for(int k=0;k<i;k++)
            if(strcmp(cars[k].brand,cars[i].brand)==0 && strcmp(cars[k].model,cars[i].model)==0) model_already=1;

        if(!model_already){
            printf("--- Model: %s ---\n", cars[i].model);
            for(int m=0;m<car_count;m++){
                if(strcmp(cars[m].brand,cars[i].brand)==0 && strcmp(cars[m].model,cars[i].model)==0){
                    printf("%d#car\nYear: %d\nPrice: %d$\nSeats: %d\nColor: %s\nTechnical Inspection: %s\nBody Type: %s\nMileage: %s\nGear Box: %s\nFuel Type: %s\nOwner: %s\n\n",
                    cars[m].id,cars[m].year,cars[m].price,cars[m].seats,cars[m].color,cars[m].technical_inspection,
                    cars[m].body_type,cars[m].mileage,cars[m].gear_box,cars[m].fuel_type,cars[m].owner);
                }
            }
        }
    }
}

void saveToFile(){
    FILE* f=fopen("cars.txt","w");
    if(!f){ printf("Cannot open file!\n"); return; }
    qsort(cars, car_count, sizeof(Car), comparePrice);

    char printed_brands[MAX_BRANDS][50]; int brand_count=0;

    for(int i=0;i<car_count;i++){
        int brand_index=-1;
        for(int j=0;j<brand_count;j++)
            if(strcmp(printed_brands[j],cars[i].brand)==0){ brand_index=j; break; }
        if(brand_index==-1){
            strcpy(printed_brands[brand_count],cars[i].brand);
            brand_index=brand_count++;
            fprintf(f,"=== %s ===\n\n", cars[i].brand);
        }

        int model_already=0;
        for(int k=0;k<i;k++)
            if(strcmp(cars[k].brand,cars[i].brand)==0 && strcmp(cars[k].model,cars[i].model)==0) model_already=1;

        if(!model_already){
            fprintf(f,"--- Model: %s ---\n", cars[i].model);
            for(int m=0;m<car_count;m++){
                if(strcmp(cars[m].brand,cars[i].brand)==0 && strcmp(cars[m].model,cars[i].model)==0){
                    fprintf(f,"%d#car\nYear: %d\nPrice: %d$\nSeats: %d\nColor: %s\nTechnical Inspection: %s\nBody Type: %s\nMileage: %s\nGear Box: %s\nFuel Type: %s\nOwner: %s\n\n",
                    cars[m].id,cars[m].year,cars[m].price,cars[m].seats,cars[m].color,cars[m].technical_inspection,
                    cars[m].body_type,cars[m].mileage,cars[m].gear_box,cars[m].fuel_type,cars[m].owner);
                }
            }
        }
    }

    fclose(f);
    printf("Saved %d cars to cars.txt\n", car_count);
}