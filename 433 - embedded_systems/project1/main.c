#include <stdio.h>
#include <stdlib.h>

#define INP_IMG_NAME "./convs/test_64.pgm"
#define IMG_EDGED_NAME "./outs/test_edged.pgm"
#define THRESHOLD 150
#define HEIGHT 64
#define WIDTH 64

// Sobel operator kernels
const int Gx[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

const int Gy[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

// Image data
/*int img[16][16] = {
    {200, 205, 208, 209, 211, 213, 216, 168, 129, 187, 215, 214, 212, 209, 205, 202},
    {202, 205, 209, 210, 212, 213, 205, 32, 25, 32, 143, 215, 212, 209, 207, 202},
    {204, 206, 209, 212, 214, 214, 157, 42, 54, 107, 84, 162, 202, 207, 204, 200},
    {203, 206, 210, 187, 100, 97, 55, 75, 126, 185, 104, 129, 143, 202, 203, 201},
    {204, 209, 192, 64, 29, 29, 28, 141, 44, 162, 70, 90, 143, 155, 202, 200},
    {203, 174, 48, 27, 26, 28, 29, 61, 40, 135, 129, 129, 104, 199, 198, 196},
    {174, 38, 29, 27, 26, 27, 26, 38, 49, 66, 200, 144, 145, 196, 197, 195},
    {117, 25, 28, 26, 26, 25, 36, 50, 27, 38, 62, 31, 173, 194, 193, 191},
    {192, 73, 23, 26, 28, 39, 42, 26, 26, 38, 66, 55, 188, 194, 192, 190},
    {196, 155, 22, 24, 26, 34, 43, 28, 38, 36, 136, 98, 172, 194, 192, 190},
    {197, 109, 25, 24, 26, 27, 30, 30, 171, 81, 168, 174, 110, 218, 197, 189},
    {193, 34, 27, 28, 28, 29, 28, 72, 188, 111, 171, 152, 99, 196, 217, 196},
    {79, 26, 26, 26, 27, 28, 27, 122, 148, 136, 172, 195, 147, 108, 154, 173},
    {52, 27, 28, 27, 27, 28, 30, 157, 96, 107, 137, 133, 113, 69, 116, 94},
    {27, 26, 27, 28, 27, 29, 50, 171, 91, 147, 157, 156, 160, 125, 119, 155},
    {26, 26, 27, 29, 27, 29, 93, 161, 70, 158, 159, 153, 149, 150, 80, 143},
};*/

// Function to convert the image data to PGM format
void convert(int [WIDTH][HEIGHT], const char *); 
// Function to decode the PGM image
void read(int [WIDTH][HEIGHT], const char *);   

int main(void) {
    int img[WIDTH][HEIGHT];
    read(img, INP_IMG_NAME);

    int edges[WIDTH][HEIGHT];
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            int sumX = 0, sumY = 0;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sumX += img[y + i][x + j] * Gx[1 + i][1 + j];
                    sumY += img[y + i][x + j] * Gy[1 + i][1 + j];
                }
            }

            int magnitude = abs(sumX) + abs(sumY);
            edges[y][x] = (magnitude > THRESHOLD) ? 255 : 0;
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        edges[x][0] = 0;
        edges[x][HEIGHT - 1] = 0;
    }

    for (int y = 0; y < HEIGHT; y++) {
        edges[0][y] = 0;
        edges[WIDTH - 1][y] = 0;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            printf("%d ", edges[y][x]);
        printf("\n");
    }

    convert(edges, IMG_EDGED_NAME);
    return 0;
}

void convert(int img[WIDTH][HEIGHT], const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "P2\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            fprintf(fp, "%d ", img[y][x]);
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void read(int img[WIDTH][HEIGHT], const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char magic[3];
    int width, height, max_val;
    fscanf(fp, "%2s", magic);
    fscanf(fp, "%d %d", &width, &height);
    fscanf(fp, "%d", &max_val);

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            fscanf(fp, "%d", &img[y][x]);

    fclose(fp);
}