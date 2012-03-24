#include<GL/freeglut.h>
#include<ctime>
#include"body.h"

body ball;

bool ACTIVE, leftClick = false;

float scalef = 1.5;
float ds;
float a;
point b;
float RAD = 0.05;

point initial,terminal;

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
    glClear(GL_COLOR_BUFFER_BIT);   //clear screen
    glClearColor(1,1,1,1);  //set white screen
    drawCircle(newPoint(0,0,0),RAD/5,newPoint(0,0,0));   //draw center
    glColor3d(0,0,0);   //draw spring
    glBegin(GL_LINE_STRIP);
    glVertex2d(0,0);
    glVertex2d(ball.pos.x,ball.pos.y);
    glEnd();
    drawCircle(ball.pos,RAD,newPoint(1,0,0));   //draw ball
    if(leftClick)
    {
        ball=newBody(initial,newPoint());
        glColor3d(1,0,0);
        glBegin(GL_LINE_STRIP);
        glVertex2d(initial.x,initial.y);
        glVertex2d(terminal.x,terminal.y);
        glEnd();
    }
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w*a/h+b.x,w*a/h+b.x,-a+b.y,a+b.y);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);
}

void init()
{
    ball=newBody();

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
        case 'k':
            ball.k*=1.5;
            break;
        case 'K':
            ball.k/=1.5;
            break;
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
        if(!leftClick)
        {
            terminal = initial = getPosition(x,y);
            leftClick = true;
        }
    }
    else if(leftClick)
    {
        ball=newBody(initial,(terminal-initial)*1e-3);
        leftClick = false;
    }
}

void mouseMotionF(int x, int y)
{
    if(leftClick)
    {
        terminal = getPosition(x,y);
        glutPostRedisplay();
    }
}

void idleF(void)
{
    if(ACTIVE)
        ball.move();
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Harmonic Oscillator");

    srand(time(NULL));
    init();

    glutDisplayFunc(displayF);
    glutIdleFunc(idleF);
    glutKeyboardFunc(keyboardF);
    glutSpecialFunc(specialKeyboardF);
    glutMouseFunc(mouseF);
    glutMotionFunc(mouseMotionF);
    glutReshapeFunc(reshapeF);

    glutMainLoop();
    return 0;
}

