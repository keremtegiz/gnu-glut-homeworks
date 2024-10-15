/*********
   CTIS164 - Template Source Program
----------
STUDENT : YUSUF KEREM TEGIZ
SECTION : 02
HOMEWORK: 2nd
----------
PROBLEMS: Nothing according to the minimum requirements.
----------
ADDITIONAL FEATURES:Birds are flapping. There is a counter that counts how many birds you have shooten in a game. After resetting the timer it also resets. The person has two guns so he can shoot two birds at the same time.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAX 100
#define BIRD_CREATE_TIME 360
#define CALM 0
#define GAME 1
#define PAUSE 2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

float y = -330;

int xBird = 0, birdcounter = 0, moveX = 0;
double timeCounter = 1250; // 20 sec

typedef struct {
	float x, y;
	int birdLoc, birdCount;
}bird_t;

typedef struct {
	float left_x, right_x, y;
	bool fire;
	float radius;
}shoot_t;
shoot_t keremsbullet = {-57 + moveX, 57 + moveX, -208, false, 3};

int mode = CALM;
char modeArr[5][10] = { "Calm", "Game" , "Pause"};

int birdcount = 0, shootedbirdcount = 0, timecalculator = BIRD_CREATE_TIME - 1;

bird_t keremsbirds[MAX];




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


void subject() {
	//right weapon
	glColor3f(0, 0, 0);
	glRectf(55 + moveX, -208, 68 + moveX, -215);
	glBegin(GL_POLYGON);
	glVertex2f(55 + moveX, -208);
	glVertex2f(68 + moveX, -215);
	glVertex2f(73 + moveX, -210);
	glVertex2f(60 + moveX, -203);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(60 + moveX, -208);
	glVertex2f(79 + moveX, -179);
	glVertex2f(64 + moveX, -173);
	glVertex2f(45 + moveX, -213);
	glEnd();

	//left weapon
	glColor3f(0, 0, 0);
	glRectf(55 + moveX, -208, 68 + moveX, -215);
	glBegin(GL_POLYGON);
	glVertex2f(-55 + moveX, -208);
	glVertex2f(-68 + moveX, -215);
	glVertex2f(-73 + moveX, -210);
	glVertex2f(-60 + moveX, -203);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-60 + moveX, -208);
	glVertex2f(-79 + moveX, -179);
	glVertex2f(-64 + moveX, -173);
	glVertex2f(-45 + moveX, -213);
	glEnd();

	//hair
	glColor3f(86 / 255., 76 / 255., 67 / 255.);
	glBegin(GL_POLYGON);
	glVertex2f(-45 + moveX, 125 + y);
	glVertex2f(-30 + moveX, 100 + y);
	glVertex2f(-25 + moveX, 120 + y);
	glVertex2f(0 + moveX, 120 + y);
	glVertex2f(25 + moveX, 110 + y);
	glVertex2f(30 + moveX, 130 + y);
	glVertex2f(10 + moveX, 110 + y);
	glVertex2f(25 + moveX, 145 + y);
	glVertex2f(-5 + moveX, 105 + y);
	glVertex2f(10 + moveX, 150 + y);
	glVertex2f(-20 + moveX, 113 + y);
	glVertex2f(-5 + moveX, 155 + y);
	glVertex2f(-10 + moveX, 110 + y);
	glVertex2f(-5 + moveX, 125 + y);
	glVertex2f(-15 + moveX, 130 + y);
	glEnd();
	//face
	glColor3f(251 / 255., 211 / 255., 176 / 255.);
	glBegin(GL_POLYGON);
	glVertex2f(-30 + moveX, 100 + y);
	glVertex2f(-25 + moveX, 120 + y);
	glVertex2f(20 + moveX, 120 + y);
	glVertex2f(25 + moveX, 110 + y);
	glVertex2f(25 + moveX, 70 + y);
	glVertex2f(-25 + moveX, 70 + y);
	glEnd();
	//lips
	glColor3f(0, 0, 0);
	circle(-3 + moveX, 86 + y, 8);
	glColor3f(251 / 255., 211 / 255., 176 / 255.);
	circle(-3 + moveX, 90 + y, 8);
	//eyes
	glColor3f(0, 0, 0);
	circle(-15 + moveX, 100 + y, 4);
	glColor3f(0, 0, 0);
	circle(10 + moveX, 100 + y, 4);
	//clothing
	//shirt
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(-28 + moveX, 70 + y);
	glVertex2f(28 + moveX, 70 + y);
	glVertex2f(28 + moveX, 0 + y);
	glVertex2f(-28 + moveX, 0 + y);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(15 + moveX, 70 + y);
	glVertex2f(-15 + moveX, 70 + y);
	glVertex2f(0 + moveX, 50 + y);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(13 + moveX, 70 + y);
	glVertex2f(-13 + moveX, 70 + y);
	glVertex2f(0 + moveX, 49 + y);
	glEnd();
	//kravat
	glColor3f(204 / 255., 92 / 255., 130 / 255.);
	glBegin(GL_TRIANGLES);
	glVertex2f(0 + moveX, 70 + y);
	glVertex2f(-7 + moveX, 60 + y);
	glVertex2f(7 + moveX, 60 + y);
	glEnd();

	glColor3f(242 / 255., 102 / 255., 147 / 255.);
	glBegin(GL_POLYGON);
	glVertex2f(-7 + moveX, 60 + y);
	glVertex2f(7 + moveX, 60 + y);
	glVertex2f(10 + moveX, 20 + y);
	glVertex2f(0 + moveX, 10 + y);
	glVertex2f(-10 + moveX, 20 + y);
	glEnd();

	//kalça
	glColor3f(0, 0, 0);
	glRectf(28 + moveX, 0 + y, -28 + moveX, -20 + y);

	//leg
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-10 + moveX, -20 + y);
	glVertex2f(-7 + moveX, -60 + y);
	glVertex2f(-12 + moveX, -60 + y);
	glVertex2f(-18 + moveX, -20 + y);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(10 + moveX, -20 + y);
	glVertex2f(7 + moveX, -60 + y);
	glVertex2f(12 + moveX, -60 + y);
	glVertex2f(18 + moveX, -20 + y);
	glEnd();

	//feet
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-12 + moveX, -60 + y);
	glVertex2f(-7 + moveX, -60 + y);
	glVertex2f(-16 + moveX, -80 + y);
	glVertex2f(-20 + moveX, -75 + y);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(12 + moveX, -60 + y);
	glVertex2f(7 + moveX, -60 + y);
	glVertex2f(16 + moveX, -80 + y);
	glVertex2f(20 + moveX, -75 + y);
	glEnd();
	//arms
	//right arm
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(23 + moveX, 50 + y);
	glVertex2f(28 + moveX, 45 + y);
	glVertex2f(60 + moveX, 120 + y);
	glVertex2f(55 + moveX, 125 + y);
	glEnd();

	glColor3f(251 / 255., 211 / 255., 176 / 255.);
	circle(57 + moveX, 122 + y, 7);

	//left arm
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(-23 + moveX, 50 + y);
	glVertex2f(-28 + moveX, 45 + y);
	glVertex2f(-60 + moveX, 120 + y);
	glVertex2f(-55 + moveX, 125 + y);
	glEnd();

	glColor3f(251 / 255., 211 / 255., 176 / 255.);
	circle(-57 + moveX, 122 + y, 7);
}

void birdup(bird_t bird) {

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -140 + bird.x + bird.birdLoc, winHeight / 800. * 260 + bird.y, winWidth / 1400. * 35);

	glColor3f(31 / 255., 145 / 255., 188 / 255.);
	circle(winWidth / 1400. * -160 + bird.x + bird.birdLoc, winHeight / 800. * 280 + bird.y, winWidth / 1400. * 35);

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -160 + bird.x + bird.birdLoc, winHeight / 800. * 250 + bird.y, winWidth / 1400. * 35);

	glColor3f(31 / 255., 145 / 255., 188 / 255.);
	circle(winWidth / 1400. * -190 + bird.x + bird.birdLoc, winHeight / 800. * 250 + bird.y, winWidth / 1400. * 35);

	glColor3f(63 / 255., 79  / 255., 95 / 255.);
	glBegin(GL_TRIANGLES);
	glVertex2f(winWidth / 1400. * -250 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -230 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y);
	glVertex2f(winWidth / 1400. * -210 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y);
	glEnd();

	glColor3f(63 / 255., 79  / 255., 95 / 255.);
	circle(winWidth / 1400. * -218 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y, winWidth / 1400. * 12);

	glColor3f(63 / 255., 79 / 255., 95  / 255.);
	glBegin(GL_QUADS);
	glVertex2f(winWidth / 1400. * -210 + bird.x + bird.birdLoc, winHeight / 800. * 236 + bird.y);
	glVertex2f(winWidth / 1400. * -214 + bird.x + bird.birdLoc, winHeight / 800. * 225 + bird.y);
	glVertex2f(winWidth / 1400. * -175 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -175 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y);
	glEnd();

	glColor3f(63 / 255., 79  / 255., 95 / 255.);
	circle(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y, winWidth / 1400. * 18);

	glColor3f(63 / 255., 79  / 255., 95  / 255.);
	glBegin(GL_POLYGON);
	glVertex2f(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 238 + bird.y);
	glVertex2f(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 202 + bird.y);
	glVertex2f(winWidth / 1400. * -142 + bird.x + bird.birdLoc, winHeight / 800. * 195 + bird.y);
	glVertex2f(winWidth / 1400. * -95 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -85 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y);
	glVertex2f(winWidth / 1400. * -132 + bird.x + bird.birdLoc, winHeight / 800. * 245 + bird.y);
	glEnd();
}

void birddown(bird_t bird) {
	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -170 + bird.x + bird.birdLoc, winHeight / 800. * 195 + bird.y, winWidth / 1400. * 35);

	glColor3f(31 / 255., 145 / 255., 188 / 255.);
	circle(winWidth / 1400. * -190 + bird.x + bird.birdLoc, winHeight / 800. * 185 + bird.y, winWidth / 1400. * 35);

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -190 + bird.x + bird.birdLoc, winHeight / 800. * 195 + bird.y, winWidth / 1400. * 35);

	glColor3f(31 / 255., 145 / 255., 188 / 255.);
	circle(winWidth / 1400. * -210 + bird.x + bird.birdLoc, winHeight / 800. * 200 + bird.y, winWidth / 1400. * 35);

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	glBegin(GL_TRIANGLES);
	glVertex2f(winWidth / 1400. * -250 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -230 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y);
	glVertex2f(winWidth / 1400. * -210 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y);
	glEnd();

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -218 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y, winWidth / 1400. * 12);

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	glBegin(GL_QUADS);
	glVertex2f(winWidth / 1400. * -210 + bird.x + bird.birdLoc, winHeight / 800. * 236 + bird.y);
	glVertex2f(winWidth / 1400. * -214 + bird.x + bird.birdLoc, winHeight / 800. * 225 + bird.y);
	glVertex2f(winWidth / 1400. * -175 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -175 + bird.x + bird.birdLoc, winHeight / 800. * 230 + bird.y);
	glEnd();

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	circle(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y, winWidth / 1400. * 18);

	glColor3f(63 / 255., 79 / 255., 95 / 255.);
	glBegin(GL_POLYGON);
	glVertex2f(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 238 + bird.y);
	glVertex2f(winWidth / 1400. * -162 + bird.x + bird.birdLoc, winHeight / 800. * 202 + bird.y);
	glVertex2f(winWidth / 1400. * -142 + bird.x + bird.birdLoc, winHeight / 800. * 195 + bird.y);
	glVertex2f(winWidth / 1400. * -95 + bird.x + bird.birdLoc, winHeight / 800. * 210 + bird.y);
	glVertex2f(winWidth / 1400. * -85 + bird.x + bird.birdLoc, winHeight / 800. * 220 + bird.y);
	glVertex2f(winWidth / 1400. * -132 + bird.x + bird.birdLoc, winHeight / 800. * 245 + bird.y);
	glEnd();
}

void bullet() {
	glColor3f(1,1,0);
	circle(keremsbullet.left_x, keremsbullet.y, keremsbullet.radius);
	circle(keremsbullet.right_x, keremsbullet.y, keremsbullet.radius);
}

void movePlayer(shoot_t *tp, float speed) {
	float dx = speed * cos(67 * D2R);
	float dy = speed * sin(67 * D2R);

	tp->left_x -= dx;
	tp->right_x += dx;
	tp->y += dy;
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(31/255., 145/255., 188/255., 0);
    glClear(GL_COLOR_BUFFER_BIT);
	
    subject();

	if (mode != CALM) {
		for (int i = 0; i < birdcount; i++)
			if (keremsbirds[i].birdCount % 20 > 10)
				birddown(keremsbirds[i]);
			else
				birdup(keremsbirds[i]);
		vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_HELVETICA_18, "Remaining Time: %.1f", timeCounter / (1000. / 16));
		vprint(-winWidth / 2 + 10, winHeight / 2 - 40, GLUT_BITMAP_HELVETICA_18, "Shooted Bird Count = %d", shootedbirdcount);
		vprint(-winWidth / 2 + 10, winHeight / 2 - 60, GLUT_BITMAP_HELVETICA_18, "FIRE -> <Spacebar>");
		vprint(-winWidth / 2 + 10, winHeight / 2 - 80, GLUT_BITMAP_HELVETICA_18, "PAUSE/RESTART -> <F1>");


	}
	else {
		
		vprint2(-265, 0,0.25, "Press <F1> to start a new game.");
	}
	vprint(winWidth / 2 - 180, winHeight / 2 - 20, GLUT_BITMAP_HELVETICA_18, "Yusuf Kerem Tegiz");
	vprint(winWidth / 2 - 140, winHeight / 2 - 40, GLUT_BITMAP_HELVETICA_18, "22002645");

	if(keremsbullet.fire && mode == GAME)
		bullet();

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

	if (key == ' ' && keremsbullet.fire == false)
		if (mode == GAME) {
			keremsbullet.fire = true;
			keremsbullet.left_x = -57 + moveX;
			keremsbullet.right_x = 57 + moveX;
		}

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
	
	if (key == GLUT_KEY_F1)
		if (mode == CALM) {
			shootedbirdcount = 0;
			mode = GAME;
		}
		else if (mode == GAME)
			mode = PAUSE;
		else
			mode = GAME;

	if (key == GLUT_KEY_RIGHT)
		if (moveX < 650)
			moveX += 5;
	if (key == GLUT_KEY_LEFT)
		if (moveX > -650)
			moveX -= 5;

    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
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
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
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
	srand(time(NULL));

	timecalculator++;
	if (mode == GAME) {
		if (timecalculator % (rand() % 100 + 100) == 0) {
			keremsbirds[birdcount].x = 850;
			keremsbirds[birdcount].y = rand() % 400 - 300;
			keremsbirds[birdcount].birdCount = 0;
			keremsbirds[birdcount].birdLoc = 0;
			birdcount++;

			timecalculator = 0;
		}
		for (int i = 0; i < birdcount; i++) {

			(keremsbirds[i].birdCount)++;
			(keremsbirds[i].birdLoc) -= 3;
		}

		timeCounter--;
	}
	if (timeCounter == 0) {
		mode = CALM;
		timeCounter = 1250;
	}

	if (keremsbullet.fire) {
		if (keremsbullet.y > 400) {
			keremsbullet.fire = false;
			keremsbullet.left_x = -58 + moveX;
			keremsbullet.right_x = 58 + moveX;
			keremsbullet.y = -208;
		}
		movePlayer(&keremsbullet, 5);

		for (int i = 0; i < birdcount; i++) {
			if (((keremsbullet.left_x < (winWidth / 1400. * -90 + keremsbirds[i].x + keremsbirds[i].birdLoc) &&
				keremsbullet.left_x >(winWidth / 1400. * -240 + keremsbirds[i].x + keremsbirds[i].birdLoc)) ||
				(keremsbullet.right_x < (winWidth / 1400. * -90 + keremsbirds[i].x + keremsbirds[i].birdLoc) &&
					keremsbullet.right_x >(winWidth / 1400. * -240 + keremsbirds[i].x + keremsbirds[i].birdLoc))) && (keremsbullet.y < (winHeight / 800. * 315 + keremsbirds[i].y) &&
						keremsbullet.y >(winHeight / 800. * 210 + keremsbirds[i].y))) {
				keremsbirds[i].birdLoc -= 5000;
				shootedbirdcount++;
			}
		}
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
    glutCreateWindow("Template File");

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