/*********
   CTIS164 - Template Source Program
----------
STUDENT : Kerem Teðiz
SECTION : 02
HOMEWORK: 03
----------
PROBLEMS: Can not calculate lightning values correctly.
----------
ADDITIONAL FEATURES:None
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

float A = 100, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle1 = rand() % 359 + 1,
angle2 = rand() % 359 + 1,
angle3 = rand() % 359 + 1;


bool circle1 = true,
circle2 = true,
circle3 = true,
continueCondition = true;

typedef struct {
	float r, g, b;
} color_t;
typedef struct {
	vec_t   pos;
	color_t color;
	vec_t   vel;
} light_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;




float num1 = rand() % 5 + 1,
num2 = rand() % 4 + 1,
num3 = rand() % 3 + 1;



light_t light[3] =
{
   { {    0, 0 }, { 1, 0, 0 }, { 1,  1} },
   { {  0, 0 }, { 0, 1, 0 }, {1, 1} },
   { { 0, 0}, { 0, 0, 1 }, { 1, 1} }
};



//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}
color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

// To add distance into calculation
// when distance is 0  => its impact is 1.0
// when distance is 350 => impact is 0.0
// Linear impact of distance on light calculation.
double distanceImpact(double d) {
	return (-1.0 / 350.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = dotP(uL, v.N) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
}


// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
}

void myCircle1(float x, float y, float r) {
	glColor3f(1, 0, 0);
	circle(x, y, 15);
}

void myCircle2(float x, float y, float r) {
	glColor3f(0, 0, 1);
	circle(x, y, 15);
}

void myCircle3(float x, float y, float r) {
	glColor3f(0, 1, 0);
	circle(x, y, 15);
}


//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);



	glColor3f(0.5, 0.5, 1.0);
	circle_wire(0, 0, 200);

	glColor3f(0.5, 0.5, 1.0);
	circle_wire(0, 0, 250);

	glColor3f(0.5, 0.5, 1.0);
	circle_wire(0, 0, 300);

	if (circle1)
		myCircle1(light[0].pos.x, light[0].pos.y, 200);
	if (circle2)
		myCircle2(light[1].pos.x, light[1].pos.y, 250);
	if (circle3)
		myCircle3(light[2].pos.x, light[2].pos.y, 300);

	for (int x = 0; x <= 100; x++) {
		vertex_t P = { { x * cos(angle1 * D2R) ,x * sin(angle1 * D2R)  }, { 0, 1 } };
		vertex_t P2 = { { x * cos(angle2 * D2R) ,x * sin(angle2 * D2R)  }, { 0, 1 } };
		vertex_t P3 = { { x * cos(angle3 * D2R) ,x * sin(angle3 * D2R)  }, { 0, 1 } };

		color_t res = { 1, 0, 0 };
		color_t res2 = { 0, 0, 1 };
		color_t res3 = { 0, 1, 0 };

		for (int i = 0; i < 3; i++) {
			res = addColor(res, calculateColor(light[i], P));

			res2 = addColor(res2, calculateColor(light[i], P2));

			res3 = addColor(res3, calculateColor(light[i], P3));

		}

		// line segments will be used to paint the area in depth
		if (circle1) {
			for (int i = angle1 - 45; i < angle1 + 45; i++) {
				glBegin(GL_LINES);
				glColor3f(res.r, res.g, res.b);

				glVertex2f(x * cos(i * D2R), x * sin(i * D2R));
				//to the bottom
				glColor3f(0, 0, 0);
				glVertex2f(0, 0);
				glEnd();
			}
		}
		if (circle2) {
			for (int i = angle2 - 45; i < angle2 + 45; i++) {
				glBegin(GL_LINES);
				glColor3f(res2.r, res2.g, res2.b);

				glVertex2f(x * cos(i * D2R), x * sin(i * D2R));
				//to the bottom
				glColor3f(0, 0, 0);
				glVertex2f(0, 0);
				glEnd();
			}
		}
		if (circle3) {
			for (int i = angle3 - 45; i < angle3 + 45; i++) {
				glBegin(GL_LINES);
				glColor3f(res3.r, res3.g, res3.b);

				glVertex2f(x * cos(i * D2R), x * sin(i * D2R));
				//to the bottom
				glColor3f(0, 0, 0);
				glVertex2f(0, 0);
				glEnd();
			}
		}
	}
	glColor3f(255, 255, 255);
	vprint(-winWidth / 2 + 20, winHeight / 2 - 20, GLUT_BITMAP_9_BY_15, "Kerem Tegiz");
	vprint(-winWidth / 2 + 20, winHeight / 2 - 35, GLUT_BITMAP_9_BY_15, "Homework 3");
	vprint(-winWidth / 2 + 20, -winHeight / 2 + 20, GLUT_BITMAP_9_BY_15, "F1/F2/F3 to disable balls // F4 to Pause // F5 to Restart");


	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{

	// Write your codes here.
	switch (key) {
	case GLUT_KEY_F1:
		circle1 = !circle1;
		break;
	case GLUT_KEY_F2:
		circle2 = !circle2;
		break;
	case GLUT_KEY_F3:
		circle3 = !circle3;
		break;
	case GLUT_KEY_F4:
		continueCondition = !continueCondition;
		break;


	case GLUT_KEY_F5:
		angle1 = rand() % 359 + 1;
		angle2 = rand() % 359 + 1;
		angle3 = rand() % 359 + 1;
		num1 = rand() % 5 + 1,
			num2 = rand() % 4 + 1,
			num3 = rand() % 3 + 1;
		break;

	}





	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {

	case GLUT_KEY_UP:
		up = false;
		break;

	case GLUT_KEY_DOWN:
		down = false;
		break;

	case GLUT_KEY_LEFT:
		left = false;
		break;

	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	if (circle1 && continueCondition)
	{
		light[0].pos.x = 200 * cos(angle1 * D2R);
		light[0].pos.y = 200 * sin(angle1 * D2R);

		angle1 += num1;
		if (angle1 > 360)
			angle1 = 0;
	}

	if (circle2 && continueCondition)
	{
		light[1].pos.x = 250 * cos(angle2 * D2R);
		light[1].pos.y = 250 * sin(angle2 * D2R);

		angle2 -= num2;
		if (angle2 > 360)
			angle2 -= 360;
	}

	if (circle3 && continueCondition)
	{
		light[2].pos.x = 300 * cos(angle3 * D2R);
		light[2].pos.y = 300 * sin(angle3 * D2R);
		angle3 += num3;
		if (angle3 > 360)
			angle3 -= 360;
	}



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Balls Moving HMW3");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}