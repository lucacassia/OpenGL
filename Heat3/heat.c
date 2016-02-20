#include "heat.h"

#include <math.h>
#include <GL/freeglut.h>

int ACTIVE = 0;

float* data = NULL;

void savePPM()
{
    FILE *f = fopen("screenshot.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    unsigned int i,j;
    unsigned char color[3];
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            color[0] = (char)(255*pixels[0][HEIGHT-i-1][j]);
            color[1] = (char)(255*pixels[1][HEIGHT-i-1][j]);
            color[2] = (char)(255*pixels[2][HEIGHT-i-1][j]);
            fwrite(color, sizeof(unsigned char), 3, f);
        }
    fclose(f);
    printf("\nPPM file saved to screenshot.ppm\n");
}

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f ,0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0.0, WIDTH, 0.0, HEIGHT, 0.0, 1.0);

    if(data) free(data);
    data = (float*)malloc(3 * size() * sizeof(float));
}

void displayF()
{
    unsigned int i,j;
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            data[3*(i*WIDTH+j)+0] = (float)pixels[0][CENTER];
            data[3*(i*WIDTH+j)+1] = (float)pixels[1][CENTER];
            data[3*(i*WIDTH+j)+2] = (float)pixels[2][CENTER];
        }
    glRasterPos2i(0,0);
    glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();
}

void idleF()
{
    if(ACTIVE){
        evolve();
        glutPostRedisplay();
    }
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
            free(data);
            printf("\n[EXIT]\n");
            exit(EXIT_SUCCESS);
        case ' ':
            ACTIVE = !ACTIVE;
            if(ACTIVE) printf("\n[ACTIVE]\n");
            else printf("\n[PAUSED]\n");
            break;
        case '+':
            DIFFUSION *= 1.1;
            break;
        case '-':
            DIFFUSION /= 1.1;
            break;
    }
}

void mouseF(int button, int state, int x, int y)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3] - y;

    int c,i,j;

    if( ( y < HEIGHT ) && ( x < WIDTH ) )
        if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
            for(i = 0; i < HEIGHT; i++)
                for(j = 0; j < WIDTH; j++)
                    for(c = 0; c < 3; c++)
                        pixels[c][CENTER] += exp( -((x-j)*(x-j) + (y-i)*(y-i)) / sqrt( WIDTH * HEIGHT ) );
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    if(argc == 2) initFile(argv[1]);
    else          initRand(WIDTH,HEIGHT);

    glutInitWindowSize(WIDTH,HEIGHT); 
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
