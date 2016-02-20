#ifndef HEAT
#define HEAT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

double DIFFUSION = 0.5f;

unsigned int WIDTH = 600;
unsigned int HEIGHT = 600;
double** pixels[3] = {NULL,NULL,NULL};
double** old[3] = {NULL,NULL,NULL};
double** source[3] = {NULL,NULL,NULL};

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
    int c,i;

    for(c = 0; c < 3; c++){

        if(pixels[c]){
            for(i = 0; i < HEIGHT; i++)
                if(pixels[c][i])
                    free(pixels[c][i]);
            free(pixels[c]);
            pixels[c] = NULL;
        }

        if(old[c]){
            for(i = 0; i < HEIGHT; i++)
                if(old[c][i])
                    free(old[c][i]);
            free(old[c]);
            old[c] = NULL;
        }

        if(source[c]){
            for(i = 0; i < HEIGHT; i++)
                if(source[c][i])
                    free(source[c][i]);
            free(source[c]);
            source[c] = NULL;
        }

    }
}

void initRand(unsigned int w, unsigned int h)
{
    clear();

    srand(time(NULL));

    WIDTH = w;
    HEIGHT = h;

    int c,i,j;

    for(c = 0; c < 3; c++){
        pixels[c] = (double**)malloc(HEIGHT * sizeof(double*));
        old[c] = (double**)malloc(HEIGHT * sizeof(double*));
        source[c] = (double**)malloc(HEIGHT * sizeof(double*));

        for(i = 0; i < HEIGHT; i++){
            pixels[c][i] = (double*)malloc(WIDTH * sizeof(double));
            old[c][i] = (double*)malloc(WIDTH * sizeof(double));
            source[c][i] = (double*)malloc(WIDTH * sizeof(double));
            for(j = 0; j < WIDTH; j++){
                source[c][i][j] = 0;
                old[c][i][j] = pixels[c][i][j] = rand()*1.0/RAND_MAX;
            }
        }

    }
}

void initFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    char buf[256];
    unsigned int c,i,j,d;
    fgets(buf, 256, file);
    do{ fgets(buf, 256, file); }while ( strncmp(buf, "#", 1) == 0 );
    sscanf(buf, "%u %u", &WIDTH, &HEIGHT);
    fscanf(file, "%u", &d);
    fseek(file, 1, SEEK_CUR);

    initRand(WIDTH,HEIGHT);

    unsigned char cbuf[3];
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            fread(cbuf, 1, 3, file);
            for(c = 0; c < 3; c++)
                old[c][HEIGHT-1-i][j] = pixels[c][HEIGHT-1-i][j] = cbuf[c]/255.0;
        }
    fclose(file);
}

void evolve()
{
    int c,i,j;

    for(c = 0; c < 3; c++){

        for(i = 0; i < HEIGHT; i++)
            for(j = 0; j < WIDTH; j++)
                pixels[c][CENTER] += DIFFUSION*( old[c][RIGHT]+old[c][UP]+old[c][LEFT]+old[c][DOWN]-4*old[c][CENTER] )/4 + source[c][CENTER];

        for(i = 0; i < HEIGHT; i++)
            for(j = 0; j < WIDTH; j++)
                old[c][CENTER] = pixels[c][CENTER];

    }
}

#endif
