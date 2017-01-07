#include "julia.h"
#include <GL/freeglut.h>
#include <stdio.h>

#define PI 3.141592654

float phi[] = {0,0,0};
float factor = 1.0;

void savePPM()
{
    unsigned char *frame = (unsigned char*)malloc(3*width*height*sizeof(unsigned char));
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,frame);

    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    fwrite(frame, sizeof(unsigned char), 3 * width * height, f);
    fclose(f);

    free(frame);
}

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0 ,0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0,width,0,height,0,1);
}

void displayF()
{
    float* data = (float*)malloc(3*width*height*sizeof(float));

    unsigned int k;
    for(k = 0; k < width*height; k++){
        data[3*k+0] = sin(factor * pixels[k]*PI/100+phi[0]);
        data[3*k+1] = sin(factor * pixels[k]*PI/100+phi[1]+PI/3);
        data[3*k+2] = sin(factor * pixels[k]*PI/100+phi[2]+2*PI/3);
    }
    glRasterPos2i(0,0);
    glDrawPixels(width,height,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();

    free(data);
}

void keyboardF(unsigned char key, int mouseX, int mouseY)
{
    switch(key)
    {
        case 'p': case 'P':
            savePPM();
            break;
        case 'q': case 'Q': case 27:
            clear();
            exit(EXIT_SUCCESS);
        case ' ':
            resetView();
            compute();
            phi[0] = phi[1] = phi[2] = 0;
            break;
        case '+':
            zoomIn();
            break;
        case '-':
            zoomOut();
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
        case 'f':
            factor+=0.01;
            break;
        case 'F':
            factor-=0.01;
            break;
        case 'w':
            w();
            break;
        case 'a':
            a();
            break;
        case 's':
            s();
            break;
        case 'd':
            d();
            break;

        case ',':
            divergence += 1;
            compute();
            break;
        case '.':
            halt += 1;
            compute();
            break;
        case 'j':
            jexp *= 1.1;
            compute();
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
            moveUp();
            break;
        case GLUT_KEY_LEFT:
            moveLeft();
            break;
        case GLUT_KEY_DOWN:
            moveDown();
            break;
        case GLUT_KEY_RIGHT:
            moveRight();
            break;
    }
}

int main(int argc, char *argv[])
{
    init();

    glutInit(&argc,argv);
    glutInitWindowSize(width,height); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("julia"); 
    GLInit(); 
    glutDisplayFunc(displayF); 
    glutReshapeFunc(updateSize);
    glutIdleFunc(glutPostRedisplay); 
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyF);
    glutMainLoop();
    return 0;
}
