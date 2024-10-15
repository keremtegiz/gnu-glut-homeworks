/*********
   CTIS164 - Template Source Program
----------
STUDENT : YUSUF KEREM TEGIZ
SECTION :02
HOMEWORK:1ST
----------
ADDITIONAL FEATURES: Firing animations while planes are going up. There is a track and there are two hangars above the track. User can only create a plane above the hangar level.
                     Clouds are moving. User can resize window and project will not be corrupted. 
----------
PROBLEMS:Nothing according to the minimum requirements.
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

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define PASSIVE 0
#define ACTIVE 1

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
    int x, y;
}point_t;

int count = -1, firing = 0, minimumHeight = 0;
point_t plane[10];
double cloudanimation = 0;


bool mode = PASSIVE;

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

void land() {

    glColor3f(102 / 255., 102 / 255.,0);
    circle(-winWidth / 2 + 250, -winHeight / 2 + 150, 125);
    circle(winWidth / 2 - 250, -winHeight / 2 + 150, 125);

    glColor3f(110 / 255., 110 / 255., 110 / 255.);
    glRectf(-winWidth / 2, -winHeight / 2, winWidth / 2, -winHeight / 2 + 150);

    glColor3f(1, 1, 1);
    glRectf(-winWidth / 2, -winHeight / 2 + 125, winWidth / 2, -winHeight/2 + 110);

    glColor3f(1, 1, 1);
    glRectf(-winWidth / 2, -winHeight / 2 + 40, winWidth / 2, -winHeight / 2 + 25);

    glColor3f(1,1,51/255.);
    circle(winWidth / 2, winHeight / 2, 80);
}

void planeLoc(point_t click) {
    glColor3f(224 / 255., 224 / 255., 224 / 255.);
    circle(click.x, click.y + 45 + firing, 15);

    glBegin(GL_QUADS);
    glVertex2f(click.x - 15, click.y + 45 + firing);
    glVertex2f(click.x + 15, click.y + 45 + firing);
    glVertex2f(click.x + 12, click.y - 45 + firing);
    glVertex2f(click.x - 12, click.y - 45 + firing);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(click.x + 13, click.y + 30 + firing);
    glVertex2f(click.x + 60, click.y + firing);
    glVertex2f(click.x + 12, click.y + 5 + firing);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(click.x - 13, click.y + 30 + firing);
    glVertex2f(click.x - 60, click.y + firing);
    glVertex2f(click.x - 12, click.y + 5 + firing);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(click.x + 30, click.y - 47 + firing);
    glVertex2f(click.x ,     click.y - 25 + firing);
    glVertex2f(click.x - 30, click.y - 47 + firing);
    glEnd();
    
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(click.x + 33, click.y + firing + 3);
    glVertex2f(click.x + 33, click.y + firing - 4);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(click.x + 27, click.y + firing + 3);
    glVertex2f(click.x + 27, click.y + firing - 4);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(click.x - 33, click.y + firing + 3);
    glVertex2f(click.x - 33, click.y + firing - 4);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(click.x - 27, click.y + firing + 3);
    glVertex2f(click.x - 27, click.y + firing - 4);
    glEnd();

    if (mode == ACTIVE) {
        if (firing % 45 > 22 ) {
            glPointSize(2);
            glBegin(GL_POINTS);
            //right side
            glVertex2f(click.x + 33, click.y + firing - 7);
            glVertex2f(click.x + 37, click.y + firing - 10);
            glVertex2f(click.x + 27, click.y + firing - 8);
            glVertex2f(click.x + 31, click.y + firing - 12);
            glVertex2f(click.x + 25, click.y + firing - 13);
            //left side
            glVertex2f(click.x - 33, click.y + firing - 7);
            glVertex2f(click.x - 37, click.y + firing - 10);
            glVertex2f(click.x - 27, click.y + firing - 8);
            glVertex2f(click.x - 31, click.y + firing - 12);
            glVertex2f(click.x - 25, click.y + firing - 13);
            glEnd();
        }
    }
}

void clouds() {
    //cloud 1
    glColor3f(1, 1, 1);
    circle(winWidth / 2 - 200 + cloudanimation, winHeight / 2 - 250, 35);
    circle(winWidth / 2 - 165 + cloudanimation, winHeight / 2 - 275, 25);
    circle(winWidth / 2 - 125 + cloudanimation, winHeight / 2 - 245, 30);
    circle(winWidth / 2 - 170 + cloudanimation, winHeight / 2 - 230, 30);

    //cloud 2
    glColor3f(1, 1, 1);
    circle(winWidth / 2 + cloudanimation, winHeight / 2 - 210, 35);
    circle(winWidth / 2 + 35 + cloudanimation, winHeight / 2 - 235, 25);
    circle(winWidth / 2 + 75 + cloudanimation, winHeight / 2 - 205, 30);
    circle(winWidth / 2 + 30 + cloudanimation, winHeight / 2 - 190, 30);

    //cloud 3
    glColor3f(1, 1, 1);
    circle(winWidth / 2 + 200 + cloudanimation, winHeight / 2 - 290, 35);
    circle(winWidth / 2 + 235 + cloudanimation, winHeight / 2 - 315, 25);
    circle(winWidth / 2 + 275 + cloudanimation, winHeight / 2 - 285, 30);
    circle(winWidth / 2 + 230 + cloudanimation, winHeight / 2 - 270, 30);
}
void displayName() {
    glColor3f(0, 0, 0);
    vprint(-winWidth / 2+10, winHeight / 2-30, GLUT_BITMAP_HELVETICA_18, "Yusuf Kerem Tegiz");
    vprint(-winWidth / 2 + 10, winHeight / 2 - 60, GLUT_BITMAP_HELVETICA_18, "22002645");
    vprint(-winWidth / 2 + 10, winHeight / 2 - 90, GLUT_BITMAP_HELVETICA_18, "Homework 1");
}

void displayInfo() {
    glColor3f(0, 0, 0);
    vprint(-winWidth / 2 + 10, -winHeight / 2 + 8, GLUT_BITMAP_HELVETICA_18, "Click on the screen to create an object (maximum 10 objects)");
    if(count != -1)
    vprint(winWidth / 2 - 200, -winHeight / 2 + 8, GLUT_BITMAP_HELVETICA_18, "F1 - Fire all objects");
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(35/255., 159/255., 191/255., 0);
    glClear(GL_COLOR_BUFFER_BIT);

    land();
    clouds();
    if(count <= 10)
        for(int i = 0;i<=count;i++)
            planeLoc(plane[i]);
    else
        for (int i = 0; i <= 10; i++)
            planeLoc(plane[i]);

    displayName();
    displayInfo();
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

    if (key == GLUT_KEY_F1)
        mode = ACTIVE;

    // Write your codes here.
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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
        if (winHeight / 2 - y > -winHeight / 2 + 322 && mode == PASSIVE) {
            count++;
            plane[count].x = x - winWidth / 2;
            plane[count].y = winHeight / 2 - y;

            if (mode == PASSIVE && plane[count].y < plane[minimumHeight].y)
                minimumHeight = count;
        }
      
    


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

    glutTimerFunc(TIMER_PERIOD, onTimer, 1);
    // Write your codes here.
    cloudanimation-=0.3;

    if (mode == ACTIVE) {
        firing += 3;
    }

    if (plane[minimumHeight].y + firing - 47 > winHeight / 2) {
        mode = PASSIVE;
        count = -1;
        firing = 0;
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
    glutCreateWindow("CTIS164 HMW1");

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