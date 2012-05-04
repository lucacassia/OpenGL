#include "distribution.h"
#include "color.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>

distribution *Sheet;
int isActive, modeView;
size_t time;

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,Sheet->width,0,Sheet->height,0,1);
}

void displayF()
{
    float* data = (float*)malloc(3*(Sheet->size)*sizeof(float));
    if(data == NULL)
        printf("\n\nmalloc fail!!!\n\n");
    rgb_t color;
    int k;
    for(k = 0; k < Sheet->size; k++){
        if(modeView == 1)
            color = d2rgb(_complex_mod(Sheet->pixels[k]));
        if(modeView == 2)
            color = d2rgb(Sheet->pixels[k].re);
        if(modeView == 3)
            color = d2rgb(Sheet->pixels[k].im);
        data[3*k+0] = color.r;
        data[3*k+1] = color.g;
        data[3*k+2] = color.b;
    }
    glRasterPos2i(0,0);
    glDrawPixels(Sheet->width,Sheet->height,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void idleF()
{
    time += isActive;
    if(isActive)
        distribution_compute(Sheet);
    glutPostRedisplay();
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            distribution_free(Sheet);
            exit(EXIT_SUCCESS);
        case ' ':
            isActive =! isActive;
            break;
        case '+':
            distribution_compute(Sheet);
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
    double kx, ky, omega;
    kx = 1.4e2;
    ky = 0;
    omega = sqrt(kx*kx+ky*ky);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        for(n = 0; n < Sheet->size; n++){
            double rx = (x-n%Sheet->width), ry = (y-n/Sheet->width);
            Sheet->pixels[n].re += exp(-(rx*rx+ry*ry)/10000)*cos(rx*kx+ry*ky-omega*time);
            Sheet->pixels[n].im += exp(-(rx*rx+ry*ry)/10000)*sin(rx*kx+ry*ky-omega*time);
        }
}

int main(int argc, char *argv[])
{
    Sheet = distribution_alloc(600,600);
    time = modeView = isActive = 1;
    glutInit(&argc, argv);
    glutInitWindowSize(Sheet->width,Sheet->height); 
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
