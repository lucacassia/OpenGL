#include<GL/glut.h>
#include<ctime>
#include<unistd.h>
#include"animalList.h"

#define _WW 1000
#define _WH 500

bool ACTIVE=1,REFRESH=1;
double dt=0.5;

animalList* Prey;
animalList* Predator;

void init()
{
	Prey=new animalList("prey",200,_WW,_WH,1);
	Predator=new animalList("predator",10,_WW,_WH,2,150);
}

void DrawCircle(point p,double r,point color)
{
	glColor3d(color.x,color.y,color.z);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2*3.141;i+=3.141/90)
		glVertex2d(r*cos(i)+p.x,r*sin(i)+p.y);
	glEnd();
}

void displayCB()
{
	if(ACTIVE){
		if(REFRESH) glClear(GL_COLOR_BUFFER_BIT);
		point preyColor(1,1,1),predatorColor(1,0,0);
		Prey->Run(Predator->Head(),dt*ACTIVE,preyColor,&DrawCircle);
		Predator->Run(Prey->Head(),dt*ACTIVE,predatorColor,&DrawCircle);
	    glutSwapBuffers();
	}
}

void keyCB(unsigned char key, int x, int y)
{
	if((key=='q')or(key=='Q')){
		delete(Prey);
		delete(Predator);
		exit(0);
	}
	if((key=='p')or(key=='P')or(key==' '))
		ACTIVE=!ACTIVE;
	if((key=='f')or(key=='F'))
		REFRESH=!REFRESH;
	if((key=='r')or(key=='R')){
		delete(Prey);
		delete(Predator); 
		init();
		ACTIVE=1;
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void idleCB(void)
{
	usleep(1000);
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	init();

	int win;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(_WW,_WH);
	win = glutCreateWindow("SAVANA");

	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(0,_WW,0,_WH);

	glutDisplayFunc(displayCB);
	glutIdleFunc(idleCB);
	glutKeyboardFunc(keyCB);

	glutMainLoop();
	return 0;
}
