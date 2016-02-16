#include "Julia.h"
#include <GL/freeglut.h>
#include <stdio.h>

#define PI 3.141592654

Julia julia;
float phi[] = {0,0,0};

void savePPM()
{
    unsigned char *frame = (unsigned char*)malloc(3*julia.W()*julia.H()*sizeof(unsigned char));
    glReadPixels(0,0,julia.W(),julia.H(),GL_RGB,GL_UNSIGNED_BYTE,frame);

    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", julia.W(), julia.H());
    fwrite(frame, sizeof(unsigned char), 3 * julia.W() * julia.H(), f);
    fclose(f);

    free(frame);
}

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,julia.W(),0,julia.H(),0,1);
}

void displayF()
{
    float* data = NULL;
    data = (float*)malloc(3*julia.W()*julia.H()*sizeof(float));
    if(data == NULL)
        std::cout<<"\n\nmalloc fail!!!\n\n";
    for(unsigned int k=0;k<julia.W()*julia.H();k++){
        data[3*k+0] = sin(julia.pixels[k]*PI/100+phi[0]);
        data[3*k+1] = sin(julia.pixels[k]*PI/100+phi[1]+PI/3);
        data[3*k+2] = sin(julia.pixels[k]*PI/100+phi[2]+2*PI/3);
    }
    glRasterPos2i(0,0);
    glDrawPixels(julia.W(),julia.H(),GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
    free(data);
}

void reshapeF(int w,int h)
{
    julia.updateSize(w,h);
}

void idleF()
{
    glutPostRedisplay();
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{

    switch(key)
    {
        case 'p': case 'P':
            savePPM();
            break;
        case 'q': case 'Q': case 27:
            exit(0);
        case ' ':
            julia.resetView();
            julia.compute();
            phi[0] = phi[1] = phi[2] = 0;
            break;
        case '+':
            julia++;
            break;
        case '-':
            julia--;
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
            julia.up();
            break;
        case GLUT_KEY_LEFT:
            julia.left();
            break;
        case GLUT_KEY_DOWN:
            julia.down();
            break;
        case GLUT_KEY_RIGHT:
            julia.right();
            break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(julia.W(),julia.H()); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Julia"); 
    GLInit(); 
    glutDisplayFunc(displayF); 
    glutReshapeFunc(reshapeF);
    glutIdleFunc(idleF); 
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyF);
    glutMainLoop();
    return 0;
}
