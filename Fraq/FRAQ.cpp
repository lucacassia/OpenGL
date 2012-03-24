#include "Fraq.h"
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

Fraq fraq;
float phi[] = {0,0,0};

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,fraq.W(),0,fraq.H(),0,1);
}

void displayF()
{
    float* data = NULL;
    data = (float*)malloc(3*fraq.W()*fraq.H()*sizeof(float));
    if(data == NULL)
        std::cout<<"\n\nmalloc fail!!!\n\n";
    for(int k=0;k<fraq.W()*fraq.H();k++){
        data[3*k+0] = (sin(fraq.pixels[k]*Pi/100+phi[0]));
        data[3*k+1] = (sin(fraq.pixels[k]*Pi/100+phi[1]+Pi/3));
        data[3*k+2] = (sin(fraq.pixels[k]*Pi/100+phi[2]+2*Pi/3));
    }
    glRasterPos2i(0,0);
    glDrawPixels(fraq.W(),fraq.H(),GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void reshapeF(int w,int h)
{
    fraq.updateSize(w,h);
}

void idleF()
{
    glutPostRedisplay();
}

inline int toInt(double x){ return int(pow((x+1)/2,1/2.2)*255+.5); }
inline int toINT(double x){ return int(255*(sin(x)+1)/2); }
void saveToFile()
{
    FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d %d\n", fraq.W(), fraq.H(), 255);
    for(int k=0;k<fraq.W()*fraq.H();k++){
        fprintf(f, "%d %d %d ", toINT((fraq.pixels[k]*Pi/100+phi[0])), toINT((fraq.pixels[k]*Pi/100+phi[1]+Pi/3)), toINT((fraq.pixels[k]*Pi/100+phi[2]+2*Pi/3)));
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
            phi[0] = phi[1] = phi[2] = 0;
            break;
        case '+':
            fraq++;
            break;
        case '-':
            fraq--;
            break;
        case 'r':
            phi[0]+=0.1;
            break;
        case 'g':
            phi[1]+=0.1;
            break;
        case 'b':
            phi[2]+=0.1;
            break;
        case 'c':
            phi[0]+=0.1;
            phi[1]+=0.1;
            phi[2]+=0.1;
            break;
        case 'R':
            phi[0]-=0.1;
            break;
        case 'G':
            phi[1]-=0.1;
            break;
        case 'B':
            phi[2]-=0.1;
            break;
        case 'C':
            phi[0]-=0.1;
            phi[1]-=0.1;
            phi[2]-=0.1;
            break;
    }
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
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(fraq.W(),fraq.H()); 
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
