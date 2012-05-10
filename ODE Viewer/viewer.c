#include<GL/freeglut.h>
#include"ode.h"
#include"color.h"

bool ACTIVE = true;

ode test(0,0);

float scalef = 1.5;
float ds;
float a;
point b;

void drawLine(std::vector<point> line)
{
    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < line.size(); i++){
        rgb_t color;
        color = d2rgb(i/1000.0);
        glColor3d(color.r,color.g,color.b);
        glVertex2d(line[i].x,line[i].y);
    }
    glEnd();
}

void drawCircle(point p,double r,point color)
{
    glColor3d(color.x,color.y,color.z);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
        glVertex2d(p.x+r*cos(i*3.14159265/180),p.y+r*sin(i*3.14159265/180));
    glEnd();
}

void displayF()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawLine(test.tail);
    drawCircle(test.pos(),0.01,newPoint(0,1,1));
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w*a/h+b.x,w*a/h+b.x,-a+b.y,a+b.y,1,-1);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.5,0.5,0.5,0);
}

void init()
{
    ACTIVE = true;
    b = newPoint();
    ds = 0.05;
    a = 1;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    reshapeF(viewport[2],viewport[3]);
}

void keyboardF(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            glTranslatef(b.x,b.y,0);
            glScalef(scalef,scalef,1);
            glTranslatef(-b.x,-b.y,0);
            ds/=scalef;
            a/=scalef;
            break;
        case '-':
            glTranslatef(b.x,b.y,0);
            glScalef(1/scalef,1/scalef,1);
            glTranslatef(-b.x,-b.y,0);
            ds*=scalef;
            a*=scalef;
            break;
        case 'p': case 'P': case ' ':
            ACTIVE=!ACTIVE;
            break;
        case 'f': case 'F':
            glutFullScreenToggle();
            break;
        case 'r': case 'R':
            init();
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
        case GLUT_KEY_UP:
            glTranslatef(0,-ds,0);
            b.y+=ds;
            break;
        case GLUT_KEY_DOWN:
            glTranslatef(0,ds,0);
            b.y-=ds;
            break;
        case GLUT_KEY_LEFT:
            glTranslatef(ds,0,0);
            b.x-=ds;
            break;
        case GLUT_KEY_RIGHT:
            glTranslatef(-ds,0,0);
            b.x+=ds;
            break;
    }
}

point getPosition(int x, int y)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3]-y;
    return newPoint((2.0*x/viewport[2]-1)*viewport[2]/viewport[3],2.0*y/viewport[3]-1) * a + b;
}

void mouseF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        ode tmp(getPosition(x,y));
        test = tmp;
    }
}

void idleF(void)
{
    if(ACTIVE)
        test++;
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("ODE Viewer");

    init();

    glutDisplayFunc(displayF);
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyboardF);
    glutMouseFunc(mouseF);
    glutReshapeFunc(reshapeF);

    glutMainLoop();
    return 0;
}

