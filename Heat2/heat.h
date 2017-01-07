#ifndef HEAT
#define HEAT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "color.h"

double DIFFUSION = 0.5f;

unsigned int WIDTH = 600;
unsigned int HEIGHT = 600;
double** pixels = NULL;
double** old = NULL;
double** source = NULL;

#define RIGHT  i][(j+1)%WIDTH
#define LEFT   i][(WIDTH+j-1)%WIDTH
#define UP     (HEIGHT+i-1)%HEIGHT][j
#define DOWN   (i+1)%HEIGHT][j
#define CENTER i][j

unsigned int width()  { return WIDTH; }
unsigned int height() { return HEIGHT; }
unsigned int size()   { return WIDTH * HEIGHT; }

void clear()
{
    int i;

    if(pixels){
        for(i = 0; i < HEIGHT; i++)
            if(pixels[i])
                free(pixels[i]);
        free(pixels);
        pixels = NULL;
    }

    if(old){
        for(i = 0; i < HEIGHT; i++)
            if(old[i])
                free(old[i]);
        free(old);
        old = NULL;
    }

    if(source){
        for(i = 0; i < HEIGHT; i++)
            if(source[i])
                free(source[i]);
        free(source);
        source = NULL;
    }
}

void initRand(unsigned int w, unsigned int h)
{
    clear();

    srand(time(NULL));

    WIDTH = w;
    HEIGHT = h;

    pixels = (double**)malloc(HEIGHT * sizeof(double*));
    old = (double**)malloc(HEIGHT * sizeof(double*));
    source = (double**)malloc(HEIGHT * sizeof(double*));

    int i,j;

    for(i = 0; i < HEIGHT; i++){
        pixels[i] = (double*)malloc(WIDTH * sizeof(double));
        old[i] = (double*)malloc(WIDTH * sizeof(double));
        source[i] = (double*)malloc(WIDTH * sizeof(double));
        for(j = 0; j < WIDTH; j++){
            source[i][j] = 0;
            old[i][j] = pixels[i][j] = rand()*1.0/RAND_MAX;
        }
    }
}

void initFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    char buf[256];
    unsigned int i,j,d;
    fgets(buf, 256, file);
    do{ fgets(buf, 256, file); }while ( strncmp(buf, "#", 1) == 0 );
    sscanf(buf, "%u %u", &WIDTH, &HEIGHT);
    fscanf(file, "%u", &d);
    fseek(file, 1, SEEK_CUR);

    initRand(WIDTH,HEIGHT);

    rgb_t color;
    unsigned char cbuf[3];
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            fread(cbuf, 1, 3, file);
            color.r = cbuf[0]/255.0;
            color.g = cbuf[1]/255.0;
            color.b = cbuf[2]/255.0;
            old[HEIGHT-1-i][j] = pixels[HEIGHT-1-i][j] = rgb2d(color);
        }
    fclose(file);
}

void evolve()
{
    int i,j;

    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++)
            pixels[CENTER] += DIFFUSION * ( old[RIGHT] + old[UP] + old[LEFT] + old[DOWN] - 4 * old[CENTER] ) / 4 + source[CENTER];

    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++)
            old[CENTER] = pixels[CENTER];
}

#endif
