#include "Fraq.h"
#include "../lib/Timer.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>

Fraq fraq;
double phase[] = {0,Pi/3,2*Pi/3};
double omega = Pi;

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,fraq.w(),0,fraq.h(),0,1);
}

void displayF()
{
    Timer timer;
    timer.start();
    float* data = (float*)malloc(3*fraq.size()*sizeof(float));
    if(data == NULL)
        printf("\n\nmalloc fail!!!\n\n");
    for(int k=0;k<fraq.size();k++){
        data[3*k+0] = (sin(fraq[k]*omega+phase[0]));
        data[3*k+1] = (sin(fraq[k]*omega+phase[1]));
        data[3*k+2] = (sin(fraq[k]*omega+phase[2]));
    }
    glRasterPos2i(0,0);
    glDrawPixels(fraq.w(),fraq.h(),GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
    timer.stop();
    std::cout<<"Display: "<<timer.getElapsedTime()<<std::endl;
}

void reshapeF(int w,int h)
{
    fraq.updateSize(w,h);
}

void idleF(){}

inline unsigned char toUb(double x)
{
    return (unsigned char)(255*(x+1)/2);
}

void saveToFile()
{
    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", fraq.w(), fraq.h());
    for(int k=0;k<fraq.size();k++){
        static unsigned char color[3];
        color[0] = toUb(sin(fraq[k]*omega+phase[0]));
        color[1] = toUb(sin(fraq[k]*omega+phase[1]));
        color[2] = toUb(sin(fraq[k]*omega+phase[2]));
        fwrite(color, 1, 3, f);
    }
    fclose(f);
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            exit(0);
        case 's': case 'S':
            saveToFile();
            break;
        case ' ':
            fraq.resetView();
            fraq.compute();
            phase[0] = 0;
            phase[1] = Pi/3;
            phase[2] = 2*Pi/3;
            break;
        case '+':
            fraq++;
            break;
        case '-':
            fraq--;
            break;
        case 'r':
            phase[0]+=0.1;
            break;
        case 'g':
            phase[1]+=0.1;
            break;
        case 'b':
            phase[2]+=0.1;
            break;
        case 'c':
            phase[0]+=0.1;
            phase[1]+=0.1;
            phase[2]+=0.1;
            break;
        case 'R':
            phase[0]-=0.1;
            break;
        case 'G':
            phase[1]-=0.1;
            break;
        case 'B':
            phase[2]-=0.1;
            break;
        case 'C':
            phase[0]-=0.1;
            phase[1]-=0.1;
            phase[2]-=0.1;
            break;
    }
    glutPostRedisplay();
}

void specialKeyF(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_F11:
            glutFullScreenToggle();
            glClear(GL_COLOR_BUFFER_BIT);
            break;
        case GLUT_KEY_UP:
            fraq.up();
            break;
        case GLUT_KEY_LEFT:
            fraq.left();
            break;
        case GLUT_KEY_DOWN:
            fraq.down();
            break;
        case GLUT_KEY_RIGHT:
            fraq.right();
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(fraq.w(),fraq.h()); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Fraq"); 
    GLInit(); 
    glutDisplayFunc(displayF); 
    glutReshapeFunc(reshapeF);
    glutIdleFunc(idleF); 
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyF);
    glutMainLoop();
    return 0;
}
