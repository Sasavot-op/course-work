#include "structure.h"


int main()
{
    int choice;
    do{
        printf("\nMenu:\n1-Add Car\n2-Delete Car\n3-Show All Cars\n4-Save to File\n0-Exit\nChoice: ");
        scanf("%d",&choice);
        while(getchar()!='\n');
        switch(choice){
            case 1: addCar(); break;
            case 2: deleteCar(); break;
            case 3: showCars(); break;
            case 4: saveToFile(); break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    }while(choice!=0);

    free(cars);
    return 0;
}
