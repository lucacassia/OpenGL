#include <GL/freeglut.h>
#include "ode.h"
#include "color.h"

int active = 1;

plist *list = NULL;

float scalef = 1.5;
float ds;
float a;
plist b;

void drawLine(plist *trail, rgb_t color)
{
    glBegin(GL_LINE_STRIP);
    while(trail != NULL){
        double factor = 1;
        if(trail->z <= 0)
            factor = 0;
        if(trail->z > 0 && trail->z < 50)
            factor = trail->z/50;
        glColor3d(color.r*factor,color.g*factor,color.b*factor);
        glVertex3d(trail->x,trail->y,trail->z);
        trail = trail->next;
    }
    glEnd();
}

void drawCircle(plist head, double r, rgb_t color)
{
    glColor3d(color.r,color.g,color.b);
    glBegin(GL_POLYGON);
    int i;
    for(i = 0; i < 360; i++)
        glVertex3d(head.x+r*cos(i*3.14159265/180), head.y+r*sin(i*3.14159265/180), head.z);
    glEnd();
}

void displayF()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    rgb_t color;
    color.r = 0;
    color.g = 96/173.0;
    color.b = 1;
    drawLine(list, color);
    drawCircle(*list, 0.02, color);
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w*a/h+b.x,w*a/h+b.x,-a+b.y,a+b.y,a+b.z,-a+b.z);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,0);
}

void init()
{
    plist_add_front(&list,0,0,0,0,0,0,0);

    active = 1;
    b.x = b.y = b.z = 0;
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
            glTranslatef(b.x,b.y,b.z);
            glScalef(scalef,scalef,scalef);
            glTranslatef(-b.x,-b.y,-b.z);
            ds/=scalef;
            a/=scalef;
            break;
        case '-':
            glTranslatef(b.x,b.y,b.z);
            glScalef(1/scalef,1/scalef,1/scalef);
            glTranslatef(-b.x,-b.y,-b.z);
            ds*=scalef;
            a*=scalef;
            break;
        case 'p': case 'P': case ' ':
            active=!active;
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
        case GLUT_KEY_PAGE_UP:
            glTranslatef(0,0,-ds);
            b.z+=ds;
            break;
        case GLUT_KEY_PAGE_DOWN:
            glTranslatef(0,0,ds);
            b.z-=ds;
            break;
    }
}

plist getPosition(int x, int y)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3]-y;
    plist tmp;
    tmp.x = ((x*2.0/viewport[2]-1)*viewport[2]/viewport[3]) * a + b.x;
    tmp.y = (2.0*y/viewport[3]-1) * a + b.y;
    tmp.z = b.z;
    tmp.t = 0;
    return tmp;
}

void mouseF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        plist tmp = getPosition(x,y);
        plist_erase(&list);
        plist_add_front(&list, tmp.x, tmp.y, tmp.z, 0, 0, 0, tmp.t);
    }
}

void idleF(void)
{
    if(active)
        plist_evolve_ode(&list, NULL);
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

