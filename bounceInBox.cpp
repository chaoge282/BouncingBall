#include <cstdlib>
#include <cstdio>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include"Camera.h"
#include"Vector.h"
using namespace std;


#define WIDTH           1000	// initial window dimensions
#define HEIGHT          800

#define e1   0.4
#define e2   1.0
#define RADIUS     1.0
#define CUBEWIDTH  20.0

Camera *camera;

const float c=CUBEWIDTH/2-RADIUS;
const float v=10.0;
void TimerCallback(int value);

enum COLOR{MAT_WHITE, MAT_RED, MAT_ORANGE, MAT_YELLOW, MAT_GREEN, MAT_BLUE, MAT_VIOLET};
// colors indexed for cycling material colors
float hues[][3] = { {1, 1, 1},    // white
  {1, 0, 0},    // red
  {1, 0.5, 0},  // orange
  {1, 1, 0},    // yellow
  {0, 1, 0},    // green
  {0, 0, 1},    // blue
  {0.5, 0, 1}   // violet
};
const Vector3d n1(1,0,0),n2(-1,0,0),n3(0,-1,0),n4(0,1,0),n5(0,0,-1),n6(0,0,1);
const Vector3d topf(0,c,0),bottomf(0,-c,0),backf(0,0,-c),frontf(0,0,c),leftf(-c,0,0),rightf(c,0,0);

bool stop;
static int Color=0;
static double TimeStep=0.01;
static double TimerDelay=1;
static double CoeffofRestitution=0.76;
static Vector3d Ball(-5,8,6);
static Vector3d Velocity(12.0,-20.0,-18.0);
static Vector3d acceleration(0,-10.0,0);
static Vector3d hitvelocity;
static Vector3d hitball;
static Vector3d newvelocity,newball;

void drawBall(Vector3d& p){
	glColor3f(0,0,0);
	glTranslatef(p.x, p.y, p.z);
	glutSolidSphere(RADIUS,30,40);
//	glutSwapBuffers();
}

void drawBox(){

	glBegin(GL_POLYGON);
	glColor4f(hues[Color][0],hues[Color][1],hues[Color][2],0.1);//front
	glVertex3f(-v,-v,v);
	glVertex3f( v,-v,v);
	glVertex3f( v, v,v);
	glVertex3f(-v, v,v);
	glEnd();


	glBegin(GL_POLYGON);
	glColor4f(hues[Color+1][0],hues[Color+1][1],hues[Color+1][2],0.1);//back
	glVertex3f(-v,-v,-v);
	glVertex3f(-v, v,-v);
	glVertex3f( v, v,-v);
	glVertex3f( v,-v,-v);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(hues[Color+2][0],hues[Color+2][1],hues[Color+2][2],0.1);//top
	glVertex3f(-v, v,-v);
	glVertex3f(-v, v,v);
	glVertex3f( v, v,v);
	glVertex3f( v, v,-v);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(hues[Color+3][0],hues[Color+3][1],hues[Color+3][2],0.1);///bottom
	glVertex3f(-v,-v,-v);
	glVertex3f( v,-v,-v);
	glVertex3f( v,-v,v);
	glVertex3f(-v,-v,v);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(hues[Color+4][0],hues[Color+4][1],hues[Color+4][2],0.1);///right
	glVertex3f(v,-v,-v);
	glVertex3f(v,v,-v);
	glVertex3f(v,v,v);
	glVertex3f(v,-v,v);
	glEnd();

	glBegin(GL_POLYGON);
	glColor4f(hues[Color+5][0],hues[Color+5][1],hues[Color+5][2],0.1);///left
	glVertex3f(-v,-v,-v);
	glVertex3f(-v,-v,v);
	glVertex3f(-v, v,v);
	glVertex3f(-v, v,-v);
	glEnd();
}

void Simulate(){
		newvelocity = Velocity + TimeStep * acceleration;
		newball = Ball + TimeStep * Velocity;

		if((newball.x-leftf.x)*(Ball.x-leftf.x)<=0)
			{
			double f = (Ball.x -leftf.x)/((Ball.x-leftf.x)+(leftf.x-newball.x));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.x=-hitvelocity.x;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;
			}
		if((newball.x-rightf.x)*(Ball.x-rightf.x)<=0)
			{
			double f = (Ball.x -rightf.x)/((Ball.x-rightf.x)+(rightf.x-newball.x));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.x=-hitvelocity.x;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;

		}
		if((newball.y-topf.y)*(Ball.y-topf.y)<=0)
			{
			double f = (Ball.y -topf.y)/((Ball.y-topf.y)+(topf.y-newball.y));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.y=-hitvelocity.y;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;

			}
		if((newball.y-bottomf.y)*(Ball.y-bottomf.y)<=0)
			{
			double f = (Ball.y -bottomf.y)/((Ball.y-bottomf.y)+(bottomf.y-newball.y));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.y=-hitvelocity.y;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;
			if(acceleration * n4<0){
				if(Abs(newball.y-bottomf.y)<e1){
					if(Abs(newvelocity.y)<e2){
						stop=true;
				}
				acceleration.y *=0.9;
				}
			}

		}
		if((newball.z-frontf.z)*(Ball.z-frontf.z)<=0)
			{
			double f = (Ball.z -frontf.z)/((Ball.z-frontf.z)+(frontf.z-newball.z));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.z=-hitvelocity.z;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;

			}
		if((newball.z-backf.z)*(Ball.z-backf.z)<=0)
			{
			double f = (Ball.z -backf.z)/((Ball.z-backf.z)+(backf.z-newball.z));
			hitvelocity = Velocity + f*TimeStep * acceleration;
			hitball = Ball + f * TimeStep * Velocity;
			hitvelocity.z=-hitvelocity.z;
			newvelocity = (hitvelocity+(1-f)*TimeStep *acceleration)*CoeffofRestitution;
			newball = hitball +(1-f)*TimeStep *hitvelocity;
			}


	Velocity = newvelocity;
	Ball = newball;
	glutPostRedisplay();
	glutTimerFunc(TimerDelay,TimerCallback,0);
}

void TimerCallback(int value){
	if(value==0)
{
	if(!stop)
		Simulate();
	}
}

void init(){
	 // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0,10,70), Vector3d(0, 0, 0),
              Vector3d(0, 1, 0));

  // grey background for window
  glClearColor(0.2, 0.2, 0.2, 0.0);
  glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_NORMALIZE);
}

void PerspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	    drawBox();
	drawBall(Ball);
  glutSwapBuffers();
}



//
// Mouse Button Callback
//
void handleButtons(int button, int state, int x, int y){
  camera->HandleMouseEvent(button,state,x,y);
  glutPostRedisplay();
}

void handleMotion(int x,int y){
	camera->HandleMouseMotion(x,y);
	glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y) {
  switch (key) {
  case 't': case 'T':
		stop=false;
	//	glutTimerFunc(TimerDelay,TimerCallback,0);
	Simulate();
	break;
  case 's': case 'S':
		TimeStep +=0.01;
		break;
  case 'l' : case 'L':
 		TimeStep -=0.01;
		break;
  case 'r': case 'R':
    // reset the camera to its initial position
    camera->Reset();
	Ball.set(0,8,6);
	Velocity.set(12,-20,-18);
	acceleration.set(0,-10,0);
    break;
  case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;
  case 'm':           // M -- cycle through material colors
    case 'M':
      Color = (Color == MAT_VIOLET? MAT_WHITE: Color + 1);
      glutPostRedisplay();
      break;

  case 'q': case 'Q':   // q or esc - quit
    exit(0);
  }

  glutPostRedisplay();
}



int main(int argc, char* argv[]){

  // start up the glut utilities
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  // and establish double buffering, RGBA color
  // Depth buffering must be available for drawing the shaded model
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50,50);
  glutCreateWindow("bouncing ball in the box");

  init();
  // register callback to draw graphics when window needs updating
  glutDisplayFunc(PerspDisplay);
  glutMouseFunc(handleButtons);
  glutMotionFunc(handleMotion);
  glutKeyboardFunc(handleKey);
  glutMainLoop();
  return(0);
}
