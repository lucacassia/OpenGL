#include "sheet.h"
#include "../lib/color.h"
#include <cstdio>
#include <cstring>
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
        printf("\n\nmalloc fail!!!\n\n");

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

sheet readFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    char buf[256];
    unsigned int w, h, d;
    fgets(buf, 256, file);
    do{ fgets(buf, 256, file); }while ( strncmp(buf, "#", 1) == 0 );
    sscanf(buf, "%u %u", &w, &h);
    fscanf(file, "%u", &d);
    fseek(file, 1, SEEK_CUR);
 
    sheet img(w,h);
    rgb_t color;
    unsigned char cbuf[3];
    for(int n=0;n<w*h;n++){
        fread(cbuf, 1, 3, file);
        color.r=cbuf[0]/255.0;
        color.g=cbuf[1]/255.0;
        color.b=cbuf[2]/255.0;
        img[(h-1-n/w)*w+n%w]=rgb2d(color);
    }
    fclose(file);
    return img;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    if(argc==2){Sheet=readFile(argv[1]);test=Sheet;}
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
