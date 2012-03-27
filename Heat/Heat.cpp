#include "sheet.h"
#include <cmath>
#include <iostream>
#include <GL/freeglut.h>

#define PI 3.141592654

sheet Sheet(500,500);
int isActive=1;
int n=1;

typedef struct {float r;float g;float b;}rgb_t;

rgb_t colorize(double x)
{
    rgb_t tmp;
    if(0<=x && x<1.0/6.0){
        tmp.r=0;
        tmp.g=6*x;
        tmp.b=1;
    }
    if(1.0/6.0<=x && x<1.0/3.0){
        tmp.r=0;
        tmp.g=1;
        tmp.b=2-6*x;
    }
    if(1.0/3.0<=x && x<1.0/2.0){
        tmp.r=6*x-2;
        tmp.g=1;
        tmp.b=0;
    }
    if(1.0/2.0<=x && x<2.0/3.0){
        tmp.r=1;
        tmp.g=4-6*x;
        tmp.b=0;
    }
    if(2.0/3.0<=x && x<5.0/6.0){
        tmp.r=1;
        tmp.g=0;
        tmp.b=6*x-4;
    }
    if(5.0/6.0<=x && x<1){
        tmp.r=6-6*x;
        tmp.g=0;
        tmp.b=1;
    }
    return tmp;
}

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,Sheet.w(),0,Sheet.h(),0,1);
}

void displayF()
{
    float* data = (float*)malloc(3*Sheet.size()*sizeof(float));
    if(data == NULL)
        std::cout<<"\n\nmalloc fail!!!\n\n";
    for(int k=0;k<Sheet.size();k++){
        data[3*k+0] = colorize(Sheet[k]).r;
        data[3*k+1] = colorize(Sheet[k]).g;
        data[3*k+2] = colorize(Sheet[k]).b;
    }
    glRasterPos2i(0,0);
    glDrawPixels(Sheet.w(),Sheet.h(),GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void idleF()
{
    if(isActive){
        for(int i=0;i<n;i++)
            Sheet.compute();
        glutPostRedisplay();
    }
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            exit(0);
        case ' ':
            isActive=!isActive;
            break;
        case '+':
            n+=1;
            break;
        case '-':
            if(n>1) n-=1;
            break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(Sheet.w(),Sheet.h()); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Heat"); 
    GLInit();
    glutDisplayFunc(displayF); 
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutMainLoop();
    return 0;
}
