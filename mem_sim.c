#include<stdio.h>

#include<math.h>

#include <stdlib.h>


int main(int argc, char ** argv) {

    char * file = argv[2];
    int memorySize = atoi(argv[1]);
    int tip = atoi(argv[3]);
    char type[50];
    int idSize[50][2];
    int ram[memorySize / 4][2];
    int i = 0;
    int size = memorySize / 4;
    int frame = memorySize / 4;
    for (i = 0; i < frame; i++) {
        ram[i][0] = i;
        ram[i][1] = -1;
    }

    FILE * ptr = fopen(file, "r");
    if (ptr == NULL) {
        printf("Error!");
        exit(1);
    }
    printf("Program Launched\n");
    int k = 0;
    int status = fscanf(ptr, " %c %d %d", & type[0], & idSize[0][0], & idSize[0][1]);

    int external = 0, insufficient = 0, internal = 0;

    int kullanilan = (int) ceil(idSize[0][1] / 4.0);

    if (type[0] == 'B')
        printf("%c\t %d\t %d\t\t\t", type[0], idSize[0][0], idSize[0][1]);
    else
        printf("%c\t %d\t\t\t", type[0], idSize[0][0]);
    i = 0;
    int min = 9999, max = -1;
    int count = 0, index = 0;
    while (status != EOF) {
        if (type[i] == 'B') {
            count = 0;
            if (frame >= kullanilan) {
                int starting = 0;

                if (tip == 1) {
                    for (index = 0; index < size; index++) {
                        while (ram[index][1] == -1 && count < kullanilan) {

                            count++;
                            index++;
                        }
                        if (kullanilan == count) {

                            starting = index - count;
                            break;
                        } else
                            count = 0;
                    }

                }
                //best
                else if (tip == 2) {

                    min = 9999;
                    for (index = 0; index < size; index++) {
                        while (ram[index][1] == -1) {
                            count++;
                            index++;
                        }

                        if (min > count && kullanilan <= count) {
                            starting = index - count;
                            min = count;

                        } else
                            count = 0;

                    }

                }
                //worst
                else if (tip == 3) {

                    max = -1;
                    for (index = 0; index < size; index++) {
                        while (ram[index][1] == -1) {
                            count++;
                            index++;
                        }

                        if (max < count && kullanilan <= count) {
                            starting = index - count;
                            max = count;

                        } else
                            count = 0;
                    }

                }

                if (min != 9999) {
                    count = min;
                }
                if (max != -1) {
                    count = max;
                }

                if (count < kullanilan) {
                    external++;
                    printf("--> %d frames will be used, ERROR! External fragmentation\n", kullanilan);
                } else if (kullanilan <= count) {

                    for (k = starting; k < starting + kullanilan; k++) {
                        ram[k][1] = idSize[i][0];
                    }
                    frame = frame - kullanilan;
                    printf("--> %d frames will be used, remaining #frames: %d\n", kullanilan, frame);
                    if ((idSize[i][1] % 4) != 0)
                        internal += 4 - (idSize[i][1] % 4);
                }

            } else {
                printf("--> Error! Insufficient memory\n");
                insufficient++;
            }

        } else {
            k = 1;
            while (idSize[i][0] != idSize[k][0]) {
                k++;
            }
            int deallocated = (int) ceil(idSize[k][1] / 4.0);
            frame += deallocated;
            printf("\t--> %d frames are deallocated, available #frames: %d\n", deallocated, frame);
            int t;
            for (t = 0; t < size; t++) {
                if (ram[t][1] == idSize[i][0])
                    ram[t][1] = -1;
            }

        }

        i++;
        status = fscanf(ptr, " %c %d %d", & type[i], & idSize[i][0], & idSize[i][1]);
        kullanilan = (int) ceil(idSize[i][1] / 4.0);
        if (type[i] == 'B')
            printf("%c\t %d\t %d\t\t\t", type[i], idSize[i][0], idSize[i][1]);
        else if (type[i] == 'E')
            printf("%c\t %d\t\t\t", type[i], idSize[i][0]);
    }

    int holes[25][2];
    int holeCountSum = 0;
    int holeCount = 0;
    int flag = 0;
    int holeIndex = 0;
    for (index = 0; index < size; index++) {
        holeCount = 0;
        while (ram[index][1] == -1) {
            holeCount++;
            index++;
            flag = 1;
        }
        if (flag) {
            holes[holeIndex][0] = index - holeCount;
            holes[holeIndex][1] = holeCount;
            holeCountSum += holeCount;
            holeIndex++;
            flag = 0;
        }
    }
    printf("\nTotal free memory in holes: %d frames, %d KB\n", holeCountSum, holeCountSum * 4);
    printf("Total memory wasted as an internal fragmentation: %d KB\n", internal);
    printf("Total number of rejected processes due to external fragmentation: %d\n", external);
    printf("Total number of rejected processes due to insufficient memory: %d\n\n", insufficient);

    printf("Holes:\n");
    int z = 0;
    for (z = 0; z < holeIndex; z++)
        printf("%d %d\n", holes[z][0], holes[z][1]);

    
        

    return 0;
}
