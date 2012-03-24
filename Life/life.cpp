#include <GL/freeglut.h>
#include "Conway.h"

Conway gol(50,50);

bool leftClick;

void drawSquare(double x,double y,double l,double r,double g,double b){
	glColor3d(r,g,b);
	glBegin(GL_POLYGON);
	glVertex2d(x,y);
	glVertex2d(x+l,y);
	glVertex2d(x+l,y+l);
	glVertex2d(x,y+l);
	glEnd();
}

void GLInit()
{
    glDisable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0,gol.width(),0,gol.height());
}

void displayF()
{
    for(int i = 0; i < gol.height(); i++)
        for(int j = 0; j < gol.width(); j++)
            drawSquare(j,i,1,!gol.bitmap[i*gol.width()+j],!gol.bitmap[i*gol.width()+j],!gol.bitmap[i*gol.width()+j]);
    glutSwapBuffers();
}

void reshapeF(int w,int h)
{

}

void idleF()
{
  	glutPostRedisplay();
}

void keyboardF(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q': case 'Q': case 27:
			exit(0);
       	case ' ':
    	    gol++;
       		break;
    }
}

void mouseF(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(!leftClick)
        {
            int viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            y = viewport[3]-y;
            gol.bitmap[gol.height()*y/viewport[3]*gol.width()+(gol.width()*x/viewport[2])] = !gol.bitmap[gol.height()*y/viewport[3]*gol.width()+(gol.width()*x/viewport[2])];
            leftClick = true;
        }
    }
    else
        leftClick = false;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(gol.width()*10,gol.height()*10); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Game of Life"); 
    GLInit(); 
    glutDisplayFunc(displayF); 
    glutReshapeFunc(reshapeF);
    glutIdleFunc(idleF); 
    glutKeyboardFunc(keyboardF);
    glutMouseFunc(mouseF);
    glutMainLoop();
	return 0;
}
