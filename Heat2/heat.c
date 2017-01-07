#include "heat.h"
#include "color.h"

#include <GL/freeglut.h>

int ACTIVE = 0;

void savePPM()
{
    FILE *f = fopen("screenshot.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    unsigned int i,j;
    rgb_t color;
    unsigned char c[3];
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            color = d2rgb(pixels[HEIGHT-i-1][j]);
            c[0] = (char)(255*color.r);
            c[1] = (char)(255*color.g);
            c[2] = (char)(255*color.b);
            fwrite(c, sizeof(unsigned char), 3, f);
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
}

void displayF()
{
    float* data = (float*)malloc(3 * size() * sizeof(float));

    rgb_t color;
    unsigned int i,j;
    for(i = 0; i < HEIGHT; i++)
        for(j = 0; j < WIDTH; j++){
            color = d2rgb(pixels[CENTER]);
            data[3*(i*WIDTH+j)+0] = color.r;
            data[3*(i*WIDTH+j)+1] = color.g;
            data[3*(i*WIDTH+j)+2] = color.b;
        }
    glRasterPos2i(0,0);
    glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_FLOAT,data);
    glutSwapBuffers();

    free(data);
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
    int i,j,viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3] - y;

    if( ( y < HEIGHT ) && ( x < WIDTH ) )
        if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
            for(i = 0; i < HEIGHT; i++)
                for(j = 0; j < WIDTH; j++)
                    pixels[CENTER] += exp( -((x-j)*(x-j) + (y-i)*(y-i)) / sqrt( WIDTH * HEIGHT ) ); /* maybe old, dunno */
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    if(argc == 2)
        initFile(argv[1]);
    else
        initRand(WIDTH,HEIGHT);

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
