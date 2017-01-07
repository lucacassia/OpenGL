#include"../lib/point.h"
#include<cstdlib>
#include<unistd.h>
#include<GL/glut.h>

#define _WW 600
#define _WH 600

bool ACTIVE=1,REFRESH=1;
const double ds=0.5,ER=0.1;
const point p(_WW/2,2*_WH/5,0);
point a,b,c;

double area(point a,point b,point c){
	return fabs(a.x*b.y+b.x*c.y+c.x*a.y-a.y*b.x-b.y*c.x-c.y*a.x)/2;
}

int reset(){
	a.x=0.5*500;
	a.y=-500*sqrt(3)/6;
	b.x=0;
	b.y=500*sqrt(3)/3;
	c.x=-500*0.5;
	c.y=-500*sqrt(3)/6;
	return 1;
}

void displayCB(){
	if(ACTIVE){
		if(REFRESH)	glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POLYGON);
		glColor3d(1,0,0);glVertex2d(a.x+p.x,a.y+p.y);
		glColor3d(0,1,0);glVertex2d(b.x+p.x,b.y+p.y);
		glColor3d(0,0,1);glVertex2d(c.x+p.x,c.y+p.y);
		glEnd();
		glutSwapBuffers();
		if(area(a,b,c)<ER)
			reset();
		point	A = a+((b-a)/(b-a).mod())*ds,
		        B = b+((c-b)/(c-b).mod())*ds,
		        C = c+((a-c)/(a-c).mod())*ds;
		a = A;
		b = B;
		c = C;
	}
}

void keyCB(unsigned char key, int x, int y){
	if((key=='q')or(key=='q'))
		exit(0);
	if((key==' ')or(key=='p')or(key=='P'))
		ACTIVE=!ACTIVE;
	if((key=='r')or(key=='R'))
		ACTIVE=reset();
	if((key=='f')or(key=='F'))
		REFRESH=!REFRESH;

}

void idleCB(void){
	usleep(1000);
	glutPostRedisplay();
}

int main(int argc, char *argv[]){
	reset();
	int win;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(_WW,_WH);
	win = glutCreateWindow("SNAILS");

	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(0,_WW,0,_WH);

	glutDisplayFunc(displayCB);
	glutIdleFunc(idleCB);
	glutKeyboardFunc(keyCB);

	glutMainLoop();
	return 0;
}

