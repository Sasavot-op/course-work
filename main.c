#include "structure.h"
#include "util.h"
#include <stdio.h>
#include <string.h> 

void printHelp(const char *progname){
    printf("Usage: %s [file]\n", progname);
    printf("Options:\n");
    printf("  -h, --help        Show this help\n");
    printf("  --gen-test N      Generate N test records\n");
    printf("If file is provided, program will try to load data from it on start and save on exit.\n");
}

int main(int argc, char **argv){
    const char *dbfile = "cars.db";
    int gen_test = 0;
    /* Простая обработка argv */
    for(int i=1;i<argc;i++){
        if(my_strcmp(argv[i], "-h")==0 || my_strcmp(argv[i], "--help")==0){ printHelp(argv[0]); return 0; }
        else if(my_strcmp(argv[i], "--gen-test")==0){
            if(i+1<argc) { gen_test = atoi(argv[++i]); if(gen_test<0) gen_test=0; }
        } else {
            dbfile = argv[i];
        }
    }

    loadFromFile(dbfile);

    if(gen_test>0) generateTestData(gen_test);

    int choice;
    do{
        printf("\nMenu:\n");
        printf("1 - Add Car\n");
        printf("2 - Delete Car\n");
        printf("3 - Show All Cars\n");
        printf("4 - Save to File\n");
        printf("5 - Filter & Map\n");
        printf("6 - Generate Test Data\n");
        printf("0 - Exit\n");
        printf("Choice: ");
        if(scanf("%d",&choice)!=1){ while(getchar()!='\n'); choice = -1; }
        while(getchar()!='\n');
        switch(choice){
            case 1: addCarInteractive(); break;
            case 2: deleteCarById(); break;
            case 3: showAllCars(); break;
            case 4: {
                printf("Filename to save (or empty for default %s): ", dbfile);
                char *f = readLine();
                if(f && my_strlen(f)>0) saveToFile(f);
                else saveToFile(dbfile);
                free(f);
                break;
            }
            case 5: {
                printf("Filter menu. Choose:\n1-brand\n2-model\n3-price range\n4-year range\nChoice: ");
                int fc=0;
                if(scanf("%d",&fc)!=1) fc=0;
                while(getchar()!='\n');
                Car *res=NULL; int rc=0;
                if(fc==1){
                    printf("Brand: "); char *b=readLine();
                    res = filterByBrand(b,&rc);
                    free(b);
                } else if(fc==2){
                    printf("Model: "); char *m=readLine();
                    res = filterByModel(m,&rc);
                    free(m);
                } else if(fc==3){
                    printf("Min price: "); int mn=0; if(scanf("%d",&mn)!=1) mn=0; while(getchar()!='\n');
                    printf("Max price: "); int mx=100000000; if(scanf("%d",&mx)!=1) mx=100000000; while(getchar()!='\n');
                    res = filterByPriceRange(mn,mx,&rc);
                } else if(fc==4){
                    printf("Min year: "); int mn=0; if(scanf("%d",&mn)!=1) mn=0; while(getchar()!='\n');
                    printf("Max year: "); int mx=9999; if(scanf("%d",&mx)!=1) mx=9999; while(getchar()!='\n');
                    res = filterByYearRange(mn,mx,&rc);
                } else {
                    printf("Unknown filter\n");
                }
                if(res){
                    printf("Found %d records. Map to file? (enter filename or empty for stdout): ", rc);
                    char *fname = readLine();
                    if(fname && my_strlen(fname)>0) mapToFile(res, rc, fname);
                    else mapToFile(res, rc, NULL);
                    free(fname);
                    /* free res */
                    for(int i=0;i<rc;i++) freeCar(&res[i]);
                    free(res);
                }
                break;
            }
            case 6: {
                printf("How many test cars to generate? ");
                int n=0; if(scanf("%d",&n)!=1) n=0; while(getchar()!='\n');
                if(n>0) generateTestData(n);
                break;
            }
            case 0: break;
            default: printf("Invalid choice\n");
        }
    } while(choice!=0);

    saveToFile(dbfile);

    for(int i=0;i<car_count;i++) freeCar(&cars[i]);
    free(cars); cars = NULL;
    car_count = 0;
    return 0;
}
