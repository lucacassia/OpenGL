#include <GL/freeglut.h>
#include <stdio.h>
#include "ode.h"

int mouseDown = 0;
int active = 1;

int width = 500;
int height = 500;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

plist *list = NULL;

float scale = 1.5f;
float shift = 0.05f;
float scaling = 1.0f;
plist translation = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
plist haxis = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
plist vaxis = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

void savePPM(unsigned char *frame)
{
    FILE *f = fopen("image.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    fwrite(frame, sizeof(unsigned char), 3 * width * height, f);
    fclose(f);
}

void saveToFile(plist *trail)
{
    FILE *f = fopen("image.dat", "w");
    while(trail != NULL){
        fprintf(f, "%lf\t%lf\t%lf\n", trail->x, trail->y, trail->z);
        trail = trail->next;
    }
    fclose(f);
}

void drawLine(plist *trail, double *color)
{
    glBegin(GL_LINE_STRIP);
    while(trail != NULL){
        double factor = 1;
        if(trail->z <= 0)
            factor = 1;
        if(trail->z > 0 && trail->z < 50)
            factor = 1-trail->z/50;
        glColor3d(color[0]*factor,color[1]*factor,color[2]*factor);
        glVertex3d(trail->x,trail->y,trail->z);
        trail = trail->next;
    }
    glEnd();
}

void drawCircle(plist head, double r, double *color)
{
    glColor3d(color[0],color[1],color[2]);
    glBegin(GL_POLYGON);
    int i;
    for(i = 0; i < 360; i++)
        glVertex3d(head.x+r*cos(i*3.14159265/180), head.y+r*sin(i*3.14159265/180), head.z);
    glEnd();
}

int init()
{
    plist_add_front(&list,0.5,0,0,0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);

    return 1;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
    0.0f, 0.0f, 3.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

    glScalef(scaling,scaling,scaling);
    glTranslatef(translation.x,translation.y,translation.z);
    glRotatef(xrot, haxis.x, haxis.y, haxis.z);
    glRotatef(yrot, vaxis.x, vaxis.y, vaxis.z);

    double color[3] = {0, 0.134, 1};
    drawLine(list, color);
    drawCircle(*list, 0.02, color);

    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    width = w;
    height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle()
{
    if(active)
        plist_evolve_ode(&list, NULL);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            shift /= scale;
            scaling *= scale;
            break;
        case '-':
            shift *= scale;
            scaling /= scale;
            break;
        case 'p': case 'P': case ' ':
            active = !active;
            break;
        case 'f': case 'F':
            glutFullScreenToggle();
            break;
        case 'q': case 'Q': case 27:
            plist_erase(&list);
            exit(0);
            break;
    }
}

void specialKeyboard(int key, int x, int y)
{
    unsigned char *frame = NULL;
    switch(key)
    {
        case GLUT_KEY_F1:
            frame = (unsigned char*)malloc(3*width*height*sizeof(unsigned char));
            glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,frame);
            savePPM(frame);
            free(frame);
            break;
        case GLUT_KEY_F2:
            saveToFile(list);
            break;
        case GLUT_KEY_F11:
            glutFullScreenToggle();
            break;
        case GLUT_KEY_UP:
            translation.y -= shift;
            break;
        case GLUT_KEY_DOWN:
            translation.y += shift;
            break;
        case GLUT_KEY_LEFT:
            translation.x += shift;
            break;
        case GLUT_KEY_RIGHT:
            translation.x -= shift;
            break;
        case GLUT_KEY_PAGE_UP:
            translation.z -= shift;
            break;
        case GLUT_KEY_PAGE_DOWN:
            translation.z += shift;
            break;
    }
}

plist getPosition(int x, int y)
{
    plist tmp;
    tmp.x = scaling * ((x*2.0/width-1)*width)/height + translation.x;
    tmp.y = scaling * (y*2.0/height-1) + translation.y;
    tmp.z = translation.z;
    tmp.t = 0;
    return tmp;
}

void mouse(int button, int state, int x, int y)
{
    if ((mouseDown = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)))
    {
        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        plist tmp = getPosition(x,height-y);
        plist_erase(&list);
        plist_add_front(&list, tmp.x, tmp.y, tmp.z, tmp.t);
    }
}

void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(width, height);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("ODE");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);

    if (!init())
        return 1;

    glutMainLoop();

    return 0;
}
