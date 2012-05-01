#include <GL/freeglut.h>
#include <stdio.h>
#include "matrix_t.h"

matrix_t *fraq;
double omega = 3.14159265;

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,fraq->width,0,fraq->height,0,1);
}

void displayF()
{
    float* data = (float*)malloc(3*fraq->size*sizeof(float));
    if(data == NULL)
        printf("\n\nmalloc fail!!!\n\n");
    int k;
    for(k = 0; k < fraq->size; k++){
        data[3*k+0] = (sin(fraq->pixels[k]*omega+fraq->phase[0]));
        data[3*k+1] = (sin(fraq->pixels[k]*omega+fraq->phase[1]));
        data[3*k+2] = (sin(fraq->pixels[k]*omega+fraq->phase[2]));
    }
    glRasterPos2i(0,0);
    glDrawPixels(fraq->width,fraq->height,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void reshapeF(int w,int h)
{
    matrix_updateSize(fraq,w,h);
}

void idleF(){}

inline unsigned char toUb(double x)
{
    return (unsigned char)(255*(x+1)/2);
}

void saveToFile()
{
    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", fraq->width, fraq->height);
    int k;
    for(k = 0; k < fraq->size; k++){
        static unsigned char color[3];
        color[0] = toUb(sin(fraq->pixels[k]*omega+fraq->phase[0]));
        color[1] = toUb(sin(fraq->pixels[k]*omega+fraq->phase[1]));
        color[2] = toUb(sin(fraq->pixels[k]*omega+fraq->phase[2]));
        fwrite(color, 1, 3, f);
    }
    fclose(f);
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
            matrix_free(fraq);
            exit(0);
        case 's': case 'S':
            saveToFile();
            break;
        case ' ':
            matrix_resetView(fraq);
            matrix_compute(fraq);
            break;
        case '+':
            matrix_pp(fraq);
            break;
        case '-':
            matrix_mm(fraq);
            break;
        case 'r':
            fraq->phase[0]+=0.1;
            break;
        case 'g':
            fraq->phase[1]+=0.1;
            break;
        case 'b':
            fraq->phase[2]+=0.1;
            break;
        case 'c':
            fraq->phase[0]+=0.1;
            fraq->phase[1]+=0.1;
            fraq->phase[2]+=0.1;
            break;
        case 'R':
            fraq->phase[0]-=0.1;
            break;
        case 'G':
            fraq->phase[1]-=0.1;
            break;
        case 'B':
            fraq->phase[2]-=0.1;
            break;
        case 'C':
            fraq->phase[0]-=0.1;
            fraq->phase[1]-=0.1;
            fraq->phase[2]-=0.1;
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
            matrix_up(fraq);
            break;
        case GLUT_KEY_LEFT:
            matrix_left(fraq);
            break;
        case GLUT_KEY_DOWN:
            matrix_down(fraq);
            break;
        case GLUT_KEY_RIGHT:
            matrix_right(fraq);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    fraq = matrix_alloc(600,600);
    glutInitWindowSize(fraq->width,fraq->height); 
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
