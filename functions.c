#include "structure.h"
#include "util.h"
#include <ctype.h>

Car *cars = NULL;
int car_count = 0;
int next_id = 1;

char *readLine(void){
    size_t cap = 64;
    size_t len = 0;
    char *buf = malloc(cap);
    if(!buf) return NULL;
    int c;
    while((c = getchar()) != '\n' && c != EOF){
        if(len+1 >= cap){
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if(!tmp){ free(buf); return NULL; }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    char *res = my_strdup(buf);
    free(buf);
    return res;
}

void freeCar(Car *c){
    if(!c) return;
    free(c->brand); free(c->model); free(c->color); free(c->technical_inspection);
    free(c->body_type); free(c->mileage); free(c->gear_box); free(c->fuel_type); free(c->owner);
}

void printCar(const Car *c){
    if(!c) return;
    printf("ID: %d\nBrand: %s\nModel: %s\nYear: %d\nPrice: %d\nSeats: %d\nColor: %s\nTechnical Inspection: %s\nBody Type: %s\nMileage: %s\nGear Box: %s\nFuel Type: %s\nOwner: %s\n\n",
        c->id,
        c->brand?c->brand:"(none)",
        c->model?c->model:"(none)",
        c->year,
        c->price,
        c->seats,
        c->color?c->color:"(none)",
        c->technical_inspection?c->technical_inspection:"(none)",
        c->body_type?c->body_type:"(none)",
        c->mileage?c->mileage:"(none)",
        c->gear_box?c->gear_box:"(none)",
        c->fuel_type?c->fuel_type:"(none)",
        c->owner?c->owner:"(none)"
    );
}

void printCarShort(const Car *c){
    if(!c) return;
    printf("%d: %s %s (%d) - %d$\n", c->id, c->brand?c->brand:"", c->model?c->model:"", c->year, c->price);
}

void addCarInteractive(void){
    Car newc;
    newc.id = next_id++;
    printf("Add new car (press enter for empty fields)\n");
    printf("Brand: "); newc.brand = readLine();
    printf("Model: "); newc.model = readLine();

    printf("Year (int): ");
    if(scanf("%d",&newc.year)!=1) newc.year = 0;
    while(getchar()!='\n');

    printf("Price (int): ");
    if(scanf("%d",&newc.price)!=1) newc.price = 0;
    while(getchar()!='\n');

    printf("Seats (int): ");
    if(scanf("%d",&newc.seats)!=1) newc.seats = 0;
    while(getchar()!='\n');

    printf("Color: "); newc.color = readLine();
    printf("Technical Inspection: "); newc.technical_inspection = readLine();
    printf("Body Type: "); newc.body_type = readLine();
    printf("Mileage: "); newc.mileage = readLine();
    printf("Gear Box: "); newc.gear_box = readLine();
    printf("Fuel Type: "); newc.fuel_type = readLine();
    printf("Owner: "); newc.owner = readLine();

    Car *tmp = realloc(cars, sizeof(Car)*(car_count+1));
    if(!tmp){
        printf("Memory error\n");
        freeCar(&newc);
        return;
    }
    cars = tmp;
    cars[car_count] = newc;
    car_count++;
    printf("Added car with ID %d\n", newc.id);
}

void deleteCarById(void){
    if(car_count==0){ printf("No cars available\n"); return; }
    printf("Enter car ID to delete: ");
    int id;
    if(scanf("%d",&id)!=1){ while(getchar()!='\n'); printf("Invalid input\n"); return; }
    while(getchar()!='\n');
    int idx = -1;
    for(int i=0;i<car_count;i++) if(cars[i].id == id){ idx = i; break; }
    if(idx==-1){ printf("ID not found\n"); return; }
    freeCar(&cars[idx]);
    for(int j=idx;j<car_count-1;j++) cars[j]=cars[j+1];
    car_count--;
    if(car_count==0){ free(cars); cars = NULL; }
    else {
        Car *tmp = realloc(cars, sizeof(Car)*car_count);
        if(tmp) cars = tmp;
    }
    printf("Deleted car %d\n", id);
}

static int cmp_price(const void *a, const void *b){
    const Car *x = a; const Car *y = b;
    return x->price - y->price;
}
static int cmp_year(const void *a, const void *b){
    const Car *x = a; const Car *y = b;
    return x->year - y->year;
}
static int cmp_id(const void *a, const void *b){
    const Car *x = a; const Car *y = b;
    return x->id - y->id;
}

void sortCarsByPrice(void){ qsort(cars, car_count, sizeof(Car), cmp_price); printf("Sorted by price.\n"); }
void sortCarsByYear(void){ qsort(cars, car_count, sizeof(Car), cmp_year); printf("Sorted by year.\n"); }
void sortCarsById(void){ qsort(cars, car_count, sizeof(Car), cmp_id); printf("Sorted by id.\n"); }

void showAllCars(void){
    if(car_count==0){ printf("No cars!\n"); return; }
    printf("Choose sorting: 1-price 2-year 3-id (default 3): ");
    int s=3;
    if(scanf("%d",&s)!=1) s=3;
    while(getchar()!='\n');
    if(s==1) sortCarsByPrice();
    else if(s==2) sortCarsByYear();
    else sortCarsById();

    for(int i=0;i<car_count;i++){
        printCarShort(&cars[i]);
    }

    printf("Show details for ID (or 0 to skip): ");
    int id;
    if(scanf("%d",&id)==1 && id!=0){
        while(getchar()!='\n');
        for(int i=0;i<car_count;i++) if(cars[i].id==id){ printCar(&cars[i]); return; }
        printf("ID not found\n");
    } else { while(getchar()!='\n'); }
}


void saveToFile(const char *filename){
    FILE *f = fopen(filename, "w");
    if(!f){ printf("Cannot open file for writing: %s\n", filename); return; }
    fprintf(f, "%d\n", car_count);
    for(int i=0;i<car_count;i++){
        Car *c = &cars[i];
        fprintf(f, "%d\n", c->id);
        fprintf(f, "%s\n", c->brand?c->brand:"");
        fprintf(f, "%s\n", c->model?c->model:"");
        fprintf(f, "%d\n", c->year);
        fprintf(f, "%d\n", c->price);
        fprintf(f, "%d\n", c->seats);
        fprintf(f, "%s\n", c->color?c->color:"");
        fprintf(f, "%s\n", c->technical_inspection?c->technical_inspection:"");
        fprintf(f, "%s\n", c->body_type?c->body_type:"");
        fprintf(f, "%s\n", c->mileage?c->mileage:"");
        fprintf(f, "%s\n", c->gear_box?c->gear_box:"");
        fprintf(f, "%s\n", c->fuel_type?c->fuel_type:"");
        fprintf(f, "%s\n", c->owner?c->owner:"");
    }
    fclose(f);
    printf("Saved %d cars to %s\n", car_count, filename);
}

int loadFromFile(const char *filename){
    FILE *f = fopen(filename, "r");
    if(!f) return 0;
    int count = 0;
    if(fscanf(f, "%d\n", &count)!=1){ fclose(f); return 0; }
    for(int i=0;i<car_count;i++) freeCar(&cars[i]);
    free(cars); cars=NULL; car_count=0; next_id=1;

    for(int i=0;i<count;i++){
        Car tmp;
        char *line = NULL;
        size_t bufcap = 0;
        int res;
        /* id */
        if(fscanf(f, "%d\n", &tmp.id)!=1) { /* corrupt */ break; }
        if(tmp.id >= next_id) next_id = tmp.id + 1;
        char buf[1024];
        if(!fgets(buf, sizeof(buf), f)) break;
        size_t ln = my_strlen(buf);
        if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0;
        tmp.brand = my_strdup(buf);

        if(!fgets(buf, sizeof(buf), f)) break;
        ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0;
        tmp.model = my_strdup(buf);

        if(fscanf(f, "%d\n", &tmp.year)!=1) tmp.year=0;
        if(fscanf(f, "%d\n", &tmp.price)!=1) tmp.price=0;
        if(fscanf(f, "%d\n", &tmp.seats)!=1) tmp.seats=0;

        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.color = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.technical_inspection = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.body_type = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.mileage = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.gear_box = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.fuel_type = my_strdup(buf);
        if(!fgets(buf, sizeof(buf), f)) break; ln = my_strlen(buf); if(ln>0 && buf[ln-1]=='\n') buf[ln-1]=0; tmp.owner = my_strdup(buf);

        Car *t = realloc(cars, sizeof(Car)*(car_count+1));
        if(!t){ freeCar(&tmp); break; }
        cars = t;
        cars[car_count] = tmp;
        car_count++;
    }
    fclose(f);
    printf("Loaded %d cars from %s\n", car_count, filename);
    return 1;
}

void generateTestData(int n){
    const char *brands[] = {"BMW","Audi","Toyota","Ford","Tesla","Opel","Skoda"};
    const char *models[] = {"X1","X3","A4","Q7","Camry","Model S","Octavia","Focus","Corolla"};
    const char *cols[] = {"Red","Blue","White","Black","Silver"};
    const char *bodies[] = {"Sedan","SUV","Coupe","Hatchback"};
    const char *gear[] = {"Auto","Manual"};
    const char *fuel[] = {"Petrol","Diesel","Electric"};
    const char *owners[] = {"Alice","Bob","Carol","Dave","Eve"};

    for(int i=0;i<n;i++){
        Car tmp;
        tmp.id = next_id++;
        tmp.brand = my_strdup(brands[i % (sizeof(brands)/sizeof(brands[0]))]);
        tmp.model = my_strdup(models[i % (sizeof(models)/sizeof(models[0]))]);
        tmp.year = 2000 + (i % 25);
        tmp.price = 5000 + (i * 1500);
        tmp.seats = 4 + (i % 3);
        tmp.color = my_strdup(cols[i % (sizeof(cols)/sizeof(cols[0]))]);
        tmp.technical_inspection = my_strdup("OK");
        tmp.body_type = my_strdup(bodies[i % (sizeof(bodies)/sizeof(bodies[0]))]);
        tmp.mileage = my_strdup("100000");
        tmp.gear_box = my_strdup(gear[i % (sizeof(gear)/sizeof(gear[0]))]);
        tmp.fuel_type = my_strdup(fuel[i % (sizeof(fuel)/sizeof(fuel[0]))]);
        tmp.owner = my_strdup(owners[i % (sizeof(owners)/sizeof(owners[0]))]);

        Car *t = realloc(cars, sizeof(Car)*(car_count+1));
        if(!t){ freeCar(&tmp); break; }
        cars = t;
        cars[car_count++] = tmp;
    }
    printf("Generated %d test cars\n", n);
}


static Car cloneCar(const Car *src){
    Car c;
    c.id = src->id;
    c.brand = my_strdup(src->brand?src->brand:"");
    c.model = my_strdup(src->model?src->model:"");
    c.year = src->year;
    c.price = src->price;
    c.seats = src->seats;
    c.color = my_strdup(src->color?src->color:"");
    c.technical_inspection = my_strdup(src->technical_inspection?src->technical_inspection:"");
    c.body_type = my_strdup(src->body_type?src->body_type:"");
    c.mileage = my_strdup(src->mileage?src->mileage:"");
    c.gear_box = my_strdup(src->gear_box?src->gear_box:"");
    c.fuel_type = my_strdup(src->fuel_type?src->fuel_type:"");
    c.owner = my_strdup(src->owner?src->owner:"");
    return c;
}

Car *filterByBrand(const char *brand, int *out_count){
    Car *res = NULL; int rc = 0;
    for(int i=0;i<car_count;i++){
        if(brand==NULL || brand[0]=='\0' || my_strcmp(cars[i].brand, brand)==0){
            Car c = cloneCar(&cars[i]);
            Car *t = realloc(res, sizeof(Car)*(rc+1));
            if(!t){ freeCar(&c); break; }
            res = t; res[rc++] = c;
        }
    }
    *out_count = rc;
    return res;
}

Car *filterByModel(const char *model, int *out_count){
    Car *res = NULL; int rc = 0;
    for(int i=0;i<car_count;i++){
        if(model==NULL || model[0]=='\0' || my_strcmp(cars[i].model, model)==0){
            Car c = cloneCar(&cars[i]);
            Car *t = realloc(res, sizeof(Car)*(rc+1));
            if(!t){ freeCar(&c); break; }
            res = t; res[rc++] = c;
        }
    }
    *out_count = rc;
    return res;
}

Car *filterByPriceRange(int minp, int maxp, int *out_count){
    Car *res = NULL; int rc = 0;
    for(int i=0;i<car_count;i++){
        if((minp<=cars[i].price) && (cars[i].price<=maxp)){
            Car c = cloneCar(&cars[i]);
            Car *t = realloc(res, sizeof(Car)*(rc+1));
            if(!t){ freeCar(&c); break; }
            res = t; res[rc++] = c;
        }
    }
    *out_count = rc;
    return res;
}

Car *filterByYearRange(int miny, int maxy, int *out_count){
    Car *res = NULL; int rc = 0;
    for(int i=0;i<car_count;i++){
        if((miny<=cars[i].year) && (cars[i].year<=maxy)){
            Car c = cloneCar(&cars[i]);
            Car *t = realloc(res, sizeof(Car)*(rc+1));
            if(!t){ freeCar(&c); break; }
            res = t; res[rc++] = c;
        }
    }
    *out_count = rc;
    return res;
}

void mapToFile(const Car *list, int list_count, const char *filename){
    if(!filename){
        for(int i=0;i<list_count;i++) printCar(&list[i]);
        return;
    }
    FILE *f = fopen(filename, "w");
    if(!f){ printf("Cannot open %s for writing\n", filename); return; }
    for(int i=0;i<list_count;i++){
        const Car *c = &list[i];
        fprintf(f, "ID:%d\t%s\t%s\t%d\t%d\t%s\n", c->id, c->brand?c->brand:"", c->model?c->model:"", c->year, c->price, c->owner?c->owner:"");
    }
    fclose(f);
    printf("Mapped %d records to %s\n", list_count, filename);
}

void freeCarArray(Car *arr, int n){
    if(!arr) return;
    for(int i=0;i<n;i++) freeCar(&arr[i]);
    free(arr);
}
