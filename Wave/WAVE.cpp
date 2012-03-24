#include<GL/freeglut.h>
#include<fstream>
#include<cstdlib>
#include<climits>
#include<fftw3.h>
#include<cmath>

#include"../lib/wave.h"

#define sign(x) (( x > 0 ) - ( x < 0 ))

bool leftClick;
int offset,length;
int dataSize;
short *sample;
float *spectr;
float spectrMAX;

void getFFT(){
    fftw_complex *in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*length);
    fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*length);

    for(int k = 0; k < length; k++){
        in[k][0] = sample[offset+k]/(double)SHRT_MAX;
        in[k][1] = 0;
    }

    fftw_plan p = fftw_plan_dft_1d(length, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_free(in);

    if(spectr!=NULL)
        free(spectr);
    spectr = (float*)malloc(sizeof(float)*(length/2));
    spectrMAX = 0;
    for(int k = 0; k < length/2; k++){
        spectr[k] = (float)sqrt(out[length/2-1-k][0]*out[length/2-1-k][0]+out[length/2-1-k][1]*out[length/2-1-k][1]);
        if(spectr[k] > spectrMAX)
            spectrMAX = spectr[k];
    }

    fftw_free(out);
}

void drawFFT()
{
    glColor3d(1,0.5,0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 7*(length/2)/8; i < length/2; i++)
        glVertex2d(-1+2*log(1+(exp(1)-1)*(i-7*(length/2)/8)/(double)(length/2-7*(length/2)/8-1)),-0.95+0.9*log(1+(exp(1)-1)*spectr[i]/(double)spectrMAX));
    glEnd();
}

void drawSample()
{
    glColor3d(0.0,0.2,0.8);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < length; i++)
        glVertex2d(-1+i*2.0/(length-1), 0.5+0.45*sign(sample[offset+i])*log(1+(exp(1)-1)*fabs(sample[offset+i]/(double)SHRT_MAX)));
    glEnd();
}

void drawCursor()
{
    glColor3d(1,1,1);
    glBegin(GL_LINE_STRIP);
        glVertex2d(-1,0);
        glVertex2d(+1,0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2d(-1+(2*offset+length)/(double)dataSize+0.005,+0.04);
        glVertex2d(-1+(2*offset+length)/(double)dataSize+0.005,-0.04);
        glVertex2d(-1+(2*offset+length)/(double)dataSize-0.005,-0.04);
        glVertex2d(-1+(2*offset+length)/(double)dataSize-0.005,+0.04);
    glEnd();

}

void displayF()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSample();
    drawFFT();
    drawCursor();
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2,0.2,0.2,0.0);
}

void init(const char* path)
{
    wave tmp(path);
    tmp.printInfo();

    std::ifstream file (path , std::ios::in | std::ios::binary);

    file.seekg(40);
    file.read((char*)&dataSize,4);

    spectr = NULL;
    sample = (short*)malloc(dataSize);
    file.read((char*)sample,dataSize);
    dataSize/=2;

    leftClick = false;
    offset = 0;
    length = dataSize;
    getFFT();
}

void keyboardF(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            length /= 2;
            offset += length/2;
            if(length <= 1)
                length = 2;
            getFFT();
            break;
        case '-':
            offset -= length/2;
            length *= 2;
            if(length > dataSize){
                offset = 0;
                length = dataSize;
            }
            if(offset < 0)
                offset = 0;
            if(offset + length > dataSize)
                offset = dataSize - length;
            getFFT();
            break;
        case 'f': case 'F':
            glutFullScreenToggle();
            break;
        case 'r': case 'R':
            leftClick = false;
            offset = 0;
            length = dataSize;
            getFFT();
            break;
        case 'q': case 'Q': case 27:
            exit(0);
            break;
    }
}

void specialKeyboardF(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F11:
            glutFullScreenToggle();
            break;
        case GLUT_KEY_LEFT:
            offset -= length/20;
            if(offset < 0)
                offset = 0;
            getFFT();
            break;
        case GLUT_KEY_RIGHT:
            offset += length/20;
            if(offset + length > dataSize)
                offset = dataSize - length;
            getFFT();
            break;
        case GLUT_KEY_HOME:
            offset = 0;
            getFFT();
            break;
        case GLUT_KEY_END:
            offset = dataSize - length;
            getFFT();
            break;
    }
}

void moveCursor(int x)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    offset = dataSize*x/viewport[2]-length/2;
    if(offset < 0)
        offset = 0;
    if(offset + length > dataSize)
        offset = dataSize - length;
    getFFT();
    glutPostRedisplay();
}

void mouseF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(!leftClick)
        {
            moveCursor(x);
            leftClick = true;
        }
    }
    else if(leftClick)
        leftClick = false;
}

void mouseMotionF(int x, int y)
{
    if(leftClick)
        moveCursor(x);
}

void idleF(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    init(argv[1]);

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(1000, 500);
    glutCreateWindow("WAVE");
    reshapeF(1000,500);

    glutDisplayFunc(displayF);
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyboardF);
    glutMouseFunc(mouseF);
    glutMotionFunc(mouseMotionF);
    glutReshapeFunc(reshapeF);

    glutMainLoop();

    free(sample);
    free(spectr);
    return 0;
}

