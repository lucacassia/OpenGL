#include "distribution.h"
#include "color.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>

distribution *pixels;
int isActive, modeView;
size_t time;

void mouseF(int,int,int,int);

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,pixels->width,0,pixels->height,0,1);
    mouseF(GLUT_LEFT_BUTTON, GLUT_DOWN, pixels->width/2, pixels->height/2);
}

void displayF()
{
    float* data = (float*)malloc(3*(pixels->size)*sizeof(float));
    if(data == NULL)
        printf("\n\nmalloc fail!!!\n\n");
    rgb_t color;
    int k;
    for(k = 0; k < pixels->size; k++){
        if(modeView == 1)
            color = d2rgb(_complex_mod(pixels->psi[k]));
        if(modeView == 2)
            color = d2rgb(pixels->psi[k].re);
        if(modeView == 3)
            color = d2rgb(pixels->psi[k].im);
        data[3*k+0] = color.r;
        data[3*k+1] = color.g;
        data[3*k+2] = color.b;
    }
    glRasterPos2i(0,0);
    glDrawPixels(pixels->width,pixels->height,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void idleF()
{
    time += isActive;
    if(isActive)
        distribution_compute(pixels);
    glutPostRedisplay();
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            distribution_free(pixels);
            exit(EXIT_SUCCESS);
        case ' ':
            isActive =! isActive;
            break;
        case '+':
            distribution_compute(pixels);
            break;
        case '1':
            modeView = 1;
            break;
        case '2':
            modeView = 2;
            break;
        case '3':
            modeView = 3;
            break;
    }
}

void mouseF(int button, int state, int x, int y)
{
    int n,viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3]-y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        double kx = 1.4e2, ky = 0, w = sqrt(kx*kx+ky*ky);
        for(n = 0; n < pixels->size; n++){
            double rx = (x-n%pixels->width), ry = (y-n/pixels->width);
            pixels->psi[n].re += exp(-(rx*rx+ry*ry)/10000)*cos(rx*kx+ry*ky-w*time);
            pixels->psi[n].im += exp(-(rx*rx+ry*ry)/10000)*sin(rx*kx+ry*ky-w*time);
        }
    }
}

int main(int argc, char *argv[])
{
    pixels = distribution_alloc(600,600);
    time = modeView = isActive = 1;
    glutInit(&argc, argv);
    glutInitWindowSize(pixels->width,pixels->height); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Schrodinger"); 
    GLInit();
    glutDisplayFunc(displayF); 
    glutIdleFunc(idleF);
    glutMouseFunc(mouseF);
    glutKeyboardFunc(keyboardF);
    glutMainLoop();
    return 0;
}
