#include "sheet.h"
#include <cmath>
#include <iostream>
#include <GL/freeglut.h>

#define PI 3.141592654

sheet Sheet(300,300);
int isActive=1;
int n=5;

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,Sheet.w(),0,Sheet.h(),0,1);
}

void displayF()
{
    float* data = NULL;
    data = (float*)malloc(3*Sheet.w()*Sheet.h()*sizeof(float));
    if(data == NULL)
        std::cout<<"\n\nmalloc fail!!!\n\n";
    for(int k=0;k<Sheet.w()*Sheet.h();k++){
        data[3*k+0] = exp(-5/Sheet.pixels[k]);
        data[3*k+1] = exp(-5/Sheet.pixels[k]);
        data[3*k+2] = exp(-5/Sheet.pixels[k]);
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
