#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

int width = 500;
int height = 500;

bool ** readFile(char *fileName, bool **array) {

char filestr[50];
 sprintf(filestr, "lifedesigns\\%s.txt", fileName);
    FILE *input = fopen(filestr, "r");
    char *str = malloc(width * sizeof(char));
    fgets(str, width, input);
    width = strlen(str)+1;     
    for (int i=0; i<height; i++) {
        array[i] = realloc(array[i], width * sizeof(bool));
    }  
    fseek(input, 0, SEEK_SET);
    int y = 0;
    while (fgets(str, width, input) != 0) {
        for (int x=0; x<width-2; x++) {
            if (str[x] == '1') {
                array[y][x] = 1;
            } else {
                array[y][x] = 0;
            }
        }
        y++;
    }
    width -= 2;
    height = y-1;
    array = realloc(array, height * sizeof(bool*));
    free(str);
    fclose(input);
    return array;
}

void printScreen(bool **array) {
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (array[y][x] == 0) {
                printf("  ");
            } else {
                printf("\e[107m  \e[0m");
            }
        }
        if (y < height-1) {
            printf("\n");
        }
    }
}

int findAdjacent(bool **array, int y, int x) {
    int count = 0;
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            if (i == 0 && j == 0) {
                continue;
            } else {
                if (x+j < 0 || x+j > (width-1) || y+i < 0 || y+i > (height-1)) {
                    continue;
                } else {
                    if (array[y+i][x+j] == 1) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}
void live (bool **array) {
    bool **array2 = calloc(height, sizeof(bool*));
    for (int i=0; i<height; i++) {
        array2[i] = calloc(width, sizeof(bool));
    }
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (array[y][x] == 0 && findAdjacent(array, y, x) == 3) {
                array2[y][x] = 1;
            } else if (array[y][x] == 1 && (findAdjacent(array, y, x) == 2 || findAdjacent(array, y, x) == 3)) {
                array2[y][x] = 1;
            }
        }
    }

    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            array[y][x] = array2[y][x];
        }
    }
}

int main(int argc, char **argv) {
    system("");
    printf("\e[?25l");
    bool **array = malloc(500 * sizeof(bool*));
    for (int i=0; i<height; i++) {
        array[i] = malloc(500 * sizeof(bool));
    }

    if (argc != 2) {
        char filename[50];
        printf("filename: ");
        scanf("%s", filename);
        readFile(filename, array);
    } else {
        readFile(argv[1], array);
    }

    printf("height = %d, width = %d\n", height, width);
    while (true) {
        printScreen(array);
        usleep(100000); 
        system("cls");
        live(array);
    }
    
}
