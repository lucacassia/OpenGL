#include "sheet.h"
#include "../lib/color.h"
#include <iostream>
#include <cmath>
#include <GL/freeglut.h>

sheet Sheet(500,500);
sheet test(Sheet);
int isActive=1;
int evoMode=1;

void GLInit(bool fullscreen=false)
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,Sheet.w(),0,Sheet.h(),0,1);
    if(fullscreen) glutFullScreenToggle();
}

void displayF()
{
    float* data = (float*)malloc((Sheet.size())*sizeof(float)*3);
    if(data == NULL)
        std::cout<<"\n\nmalloc fail!!!\n\n";

    rgb_t color;
    for(int k=0;k<Sheet.size();k++){
        if(evoMode==1)
            color.set(Sheet[k]);
        if(evoMode==2)
            color.set(test[k]);
        if(evoMode==3)
            color.set((Sheet[k]-test[k])*(Sheet[k]-test[k]));
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
    if(isActive){
        Sheet--;test++;
    }
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
        case '1':
            evoMode=1;
            break;
        case '2':
            evoMode=2;
            break;
        case '3':
            evoMode=3;
            break;
    }
}

void mouseF(int button, int state, int x, int y)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3]-y;
    if((y<Sheet.h())&&(x<Sheet.w()))
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            for(int n=0;n<Sheet.size();n++){
                double r=sqrt(((x-n%Sheet.w())*(x-n%Sheet.w())+(y-n/Sheet.w())*(y-n/Sheet.w())));
                Sheet[n]+=exp(-r*r/sqrt(Sheet.size()));
                test[n]+=exp(-r*r/sqrt(Sheet.size()));
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
    glutMouseFunc(mouseF);
    glutKeyboardFunc(keyboardF);
    glutMainLoop();
    return 0;
}
