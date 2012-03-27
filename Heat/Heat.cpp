#include "sheet.h"
#include "../lib/color.h"
#include <iostream>
#include <GL/freeglut.h>

sheet Sheet(1000,500);
int isActive=1;

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
        rgb_t color(Sheet[k]);
        data[3*k+0] = color.r;
        data[3*k+1] = color.g;
        data[3*k+2] = color.b;
    }
    glRasterPos2i(0,0);
    glDrawPixels(Sheet.w(),Sheet.h(),GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void idleF()
{
    if(isActive)
        Sheet++;
    glutPostRedisplay();
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
