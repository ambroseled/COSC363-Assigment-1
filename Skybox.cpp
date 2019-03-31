//  ========================================================================
//  COSC363: Computer Graphics (2019); CSSE  University of Canterbury.
//
//  FILE NAME: Skybox.cpp
//  See Lab03.pdf for details
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include <fstream>
#include <climits>
#include <math.h>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
float lookAngle = 0.0;		//Camera rotation
float movePos = 0.0;
float size = 300000;
float angle=0, look_x, look_z=-1., eye_x=-80000, eye_z=110000;  //Camera parameters

float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float ball_pos[3] = {38.88, 64, 0};

GLuint texId[11];
float cannonAngle = 45;
float shipHeight = 0;
float shipChange = 0;
bool shipLaunched = false;
bool changeCamera = false;
float lightAngle = 0;
float robotAngle = 0;
bool fireCannon = false;
float gravAccel = 9.80665;
float cannonRadians = 0.785398;
float cannonVelocity = 100;
int cannonCount = 1;
GLUquadricObj*	q;
float robot_1_pos[3] = {-200000.0, 0.0, 180000.0};
bool shipUp = true;
float robotAngle1 = 90;
int robotState = 0;
float theta = 0;
bool gaurdForward = true;
float robot2_z = -70;
float gaurdAngle = 0;
//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}


void loadGLTextures()				// Load bitmaps And Convert To Textures
{
	glGenTextures(11, texId); 		// Create texture ids

	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadBMP("brickTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glBindTexture(GL_TEXTURE_2D, texId[1]);
	loadBMP("brassTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glBindTexture(GL_TEXTURE_2D, texId[2]);
	loadBMP("metalTexture2.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glBindTexture(GL_TEXTURE_2D, texId[3]);
	loadBMP("redPaint.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texId[4]);
	loadBMP("paintedLines.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// *** left ***
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	loadTGA("skybox/iceflats_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** front ***
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	loadTGA("skybox/iceflats_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	loadTGA("skybox/iceflats_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** back***
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	loadTGA("skybox/iceflats_bk.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, texId[9]);
	loadTGA("skybox/iceflats_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, texId[10]);
	loadTGA("skybox/iceflats_dn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

// Above here - move o middleware module

//========================================================================================

//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{
	glColor3f(0.4, 0.5, 0.4);

    //Construct the object model here using triangles read from OFF file
	glBegin(GL_TRIANGLES);
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
	glEnd();
}

void drawWall(float length, float height, float width, int texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[texture]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);


    // front
    glTexCoord2f(0., 0.); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(2., 0.); glVertex3f(length, 0.0f, 0.0f);
    glTexCoord2f(2., 2.); glVertex3f(length, height, 0.0f);
    glTexCoord2f(0., 2.); glVertex3f(0.0f, height, 0.0f);
    // back
    glTexCoord2f(0., 0.); glVertex3f(0.0f, 0.0f, -width);
    glTexCoord2f(2., 0.); glVertex3f(length, 0.0f, -width);
    glTexCoord2f(2., 2.); glVertex3f(length, height, -width);
    glTexCoord2f(0., 2.); glVertex3f(0.0f, height, -width);
    // right
    glTexCoord2f(0., 0.); glVertex3f(length, 0.0f, 0.0f);
    glTexCoord2f(2., 0.); glVertex3f(length, 0.0f, -width);
    glTexCoord2f(2., 2.); glVertex3f(length, height, -width);
    glTexCoord2f(0., 2.); glVertex3f(length, height, 0.0f);
    // left
    glTexCoord2f(0., 0.); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(2., 0.); glVertex3f(0.0f, 0.0f, -width);
    glTexCoord2f(2., 2.); glVertex3f(0.0f, height, -width);
    glTexCoord2f(0., 2.); glVertex3f(0.0f, height, 0.0f);
    // top
    glTexCoord2f(0., 0.); glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(2., 0.); glVertex3f(length, height, 0.0f);
    glTexCoord2f(2., 2.); glVertex3f(length, height, -width);
    glTexCoord2f(0., 2.); glVertex3f(0.0f, height, -width);
    // bottom
    glTexCoord2f(0., 0.); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(2., 0.); glVertex3f(length, 0.0f, 0.0f);
    glTexCoord2f(2., 2.); glVertex3f(length, 0.0f, -width);
    glTexCoord2f(0., 2.); glVertex3f(0.0f, 0.0f, -width);
glEnd();
}

void drawCannonBody()
{
	glDisable(GL_TEXTURE_2D);
    //--start here
    glPushMatrix();
		glColor3ub(102, 102, 102);
	    glTranslatef(-10.0, 5.0, 17.0);
	    glScalef(80.0, 10.0, 6.0);
	    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
		glColor3ub(102, 102, 102);
	    glTranslatef(-20.0, 25.0, 17.0);
	    glScalef(40.0, 30.0, 6.0);
	    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
		glColor3ub(102, 102, 102);
	    glTranslatef(-10.0, 5.0, -17.0);
	    glScalef(80.0, 10.0, 6.0);
	    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
		glColor3ub(102, 102, 102);
	    glTranslatef(-20.0, 25.0, -17.0);
	    glScalef(40.0, 30.0, 6.0);
	    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
	    glTranslatef(-20, 30, 0); //Pivot point coordinates
	    glRotatef(30, 0, 0, 1); //Rotation
	    glTranslatef(20, -30, 0);
	    drawCannon();
    glPopMatrix();

    glPushMatrix();
		glColor3ub(0, 0, 0);
	    glTranslatef(ball_pos[0], ball_pos[1], 0);
	    glutSolidSphere(5, 36, 18);
    glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void drawFin()
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glVertex3f(0., 1., 0.);
      glVertex3f(-0.2, -1., 2.);
      glVertex3f(0.2, -1., 2);

      // Right
      glVertex3f(0., 1., 0.);
      glVertex3f(0.2, -1., 2.);
      glVertex3f(0.2, -1., 0);

      // Back
      glVertex3f(0., 1., 0.);
      glVertex3f(0.2, -1., 0);
      glVertex3f(-0.2, -1., 0.);

      // Left
      glVertex3f( 0., 1., 0.);
      glVertex3f(-0.2, -1., 0.);
      glVertex3f(-0.2, -1., 2.);
   glEnd();
   glEnable(GL_TEXTURE_2D);
}

void drawTower(int n, float x, float y, int texture) {
	glBindTexture(GL_TEXTURE_2D, texId[texture]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1, 1, 1);
	int numSegments = 360;
	float textInc = 1.0f / numSegments;
    // Cylinder Bottom
    glBegin(GL_POLYGON);
        for(int i = 0; i <= (numSegments); i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), 0.0);
        }
    glEnd();

    // Cylinder Top
    glBegin(GL_POLYGON);
        for(int i = 0; i <= (numSegments); i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();

    // Cylinder "Cover"
    glBegin(GL_QUAD_STRIP);
        for(int i = 0; i < 480; i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glTexCoord2f(textInc * i, 0); glVertex3f(x * cos(a), x * sin(a), 0.0);
            glTexCoord2f(textInc * i, 1); glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();
}

void drawRobot()
{
	//Head
	glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(0, 7.7, 0);
		glutSolidCube(1.4);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(0.25, 7.9, 0.7);
		glutSolidSphere(0.15, 50, 10);
	glPopMatrix();
	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(-0.25, 7.9, 0.7);
		glutSolidSphere(0.15, 50, 10);
	glPopMatrix();

			//Torso
	glPushMatrix();
		glColor3f(1, 1, 1);
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	//Right leg
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-0.8, 4, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(-0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

//Left leg
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(0.8, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

//Right arm
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-2, 6.5, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(2, -6.5, 0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	//Left arm
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(2, 6.5, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(-2, -6.5, 0);
	  glTranslatef(2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
}

void drawShip()
{
	float lgt_pos[] = {0., 0., 0.};

	// TODO rotating light in the towers
	// Ship body
	glPushMatrix();
		glTranslatef(0, 20 + shipHeight, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 10, 20, 1);
	glPopMatrix();

	// Body center

	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(0, 40 + shipHeight, 0);
		glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 2, 20, 4);

	glPopMatrix();

	// Center edges
	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(0, 40 + shipHeight, 9);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 2);
	glPopMatrix();

	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(0, 40 + shipHeight, -9);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 2);
	glPopMatrix();

	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(9, 40 + shipHeight, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 2);
	glPopMatrix();

	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(-9, 40 + shipHeight, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 2);
	glPopMatrix();

	// Body top
	glPushMatrix();
		glTranslatef(0, 60 + shipHeight, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 10, 20, 1);
	glPopMatrix();

	// Top
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glColor3ub(148, 0, 211);
		glTranslatef(0, 60 + shipHeight, 0);
	    glRotatef(-90, 1, 0, 0);
		glutSolidCone(10, 30, 50, 10);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	// Fins
	glPushMatrix();
		glColor3ub(148, 0, 211);
		glTranslatef(0, 10 + shipHeight, 10);
		glScalef(10, 10, 10);
		drawFin();
	glPopMatrix();

	glPushMatrix();
		glColor3ub(148, 0, 211);
		glTranslatef(0, 10 + shipHeight, -10);
		glRotatef(180, 0, 1, 0);
		glScalef(10, 10, 10);
		drawFin();
	glPopMatrix();

	glPushMatrix();
		glColor3ub(148, 0, 211);
		glTranslatef(-10, 10 + shipHeight, 0);
		glRotatef(-90, 0, 1, 0);
		glScalef(10, 10, 10);
		drawFin();
	glPopMatrix();

	glPushMatrix();
		glColor3ub(148, 0, 211);
		glTranslatef(10, 10 + shipHeight, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(10, 10, 10);
		drawFin();
	glPopMatrix();
}

void drawCastle()
{
	glEnable(GL_TEXTURE_2D);
	// Front wall
	glPushMatrix();
		glTranslatef(-35., 0., -50.);
		drawWall(20, 40, 5, 0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20., 0., -50.);
		drawWall(20, 40, 5, 0);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15., 30., -50.);
		drawWall(35, 10, 5, 0);
	glPopMatrix();



	// Back wall
	glPushMatrix();
		glTranslatef(-35., 0., 25.);
		drawWall(80, 40, 5, 0);
	glPopMatrix();

	// left wall
	glPushMatrix();
		glTranslatef(45., 0., 25.);
		glRotatef(90, 0., 1., 0.);
		drawWall(80, 40, 5, 0);
	glPopMatrix();

	// right wall
	glPushMatrix();
		glTranslatef(-35., 0., 25.);
		glRotatef(90, 0., 1., 0.);
		drawWall(80, 40, 5, 0);
	glPopMatrix();

	// Back Right Tower
	glPushMatrix();
		//glRotatef(90, 1, 0, 0);
		glTranslatef(-37, 50, 22);
		glRotatef(90, 1, 0, 0);
		drawTower(30, 5, 50, 0);
		glPushMatrix();
			glRotatef(180, 0, 1, 0);
			glutSolidCone(5, 10, 30, 10);
		glPopMatrix();
	glPopMatrix();

	// Back Left Tower
	glPushMatrix();
		//glRotatef(90, 1, 0, 0);
		glTranslatef(42, 50, 22);
		glRotatef(90, 1, 0, 0);
		drawTower(30, 5, 50, 0);
		glPushMatrix();
			glRotatef(180, 0, 1, 0);
			glutSolidCone(5, 10, 30, 10);
		glPopMatrix();
	glPopMatrix();

	// Front Right Tower
	glPushMatrix();
		//glRotatef(90, 1, 0, 0);
		glTranslatef(-37, 50, -52);
		glRotatef(90, 1, 0, 0);
		drawTower(30, 5, 50, 0);
		glPushMatrix();
			glRotatef(180, 0, 1, 0);
			glutSolidCone(5, 10, 30, 10);
		glPopMatrix();
	glPopMatrix();

	// Front Left Tower
	glPushMatrix();
		//glRotatef(90, 1, 0, 0);
		glTranslatef(42, 50, -52);
		glRotatef(90, 1, 0, 0);
		drawTower(30, 5, 50, 0);
		glPushMatrix();
			glRotatef(180, 0, 1, 0);
			glutSolidCone(5, 10, 30, 10);
		glPopMatrix();
	glPopMatrix();

	// Cannons
	glPushMatrix();
		glTranslatef(25, 0, -70);
		glRotatef(90, 0, 1, 0);
		glScalef(0.25, 0.25, 0.25);
		glPushMatrix();
			glScalef(0.75, 0.75, 0.75);
			drawCannonBody();
		glPopMatrix();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-25, 0, -70);
		glRotatef(90, 0, 1, 0);
		glScalef(0.25, 0.25, 0.25);
		glPushMatrix();
			glScalef(0.75, 0.75, 0.75);
			drawCannonBody();
		glPopMatrix();
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);
}

void drawRobot2()
{
	glDisable(GL_TEXTURE_2D);
	// Wheel
	glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(0, 2.5, 0);
		glutSolidSphere(2.5, 50, 10);
	glPopMatrix();


	// Base
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(0, 20, 0);
		glRotatef(90, 1, 0, 0);
		glutSolidCone(5, 20, 50, 5);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);


	// Body
	glPushMatrix();
		glTranslatef(0, 40, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 5, 20, 2);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	// Head / Teapot
	glPushMatrix();
		glTranslatef(0, 43, 0);
		glutSolidTeapot(6);
	glPopMatrix();

	// Eyes
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(5, 44, -2);
		glutSolidSphere(1, 50, 10);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(5, 44, 2);
		glutSolidSphere(1, 50, 10);
	glPopMatrix();


	// Atenna
	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(0, 53, 0);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 0.3, 8, 3);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(0, 53, 0);
		glutSolidSphere(1, 50, 10);
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);

	// Arms
	// Right
	glPushMatrix();
		glTranslatef(0, 38, 6);
		glRotatef(85, 1, 0, 0);
		drawTower(50, 1, 15, 3);
	glPopMatrix();
	// left
	glPushMatrix();
		glTranslatef(0, 38, -6);
		glRotatef(95, 1, 0, 0);
		drawTower(50, 1, 15, 3);
	glPopMatrix();


}

void drawUfo()
{
	float lgt_pos[] = {-160000., 0., 160000.};
	float lgt_dir[] = {1., -1., 0.};
	float lgt_dir1[] = {-1., -1., 0.};


	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(0, 35 + shipHeight, 0);
		glRotatef(90, 1, 0, 0);
		glutSolidCone(100, 15, 50, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(0, 38 + shipHeight, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(100, 15, 50, 10);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(0, 40 + shipHeight, 0);
		glLightfv(GL_LIGHT2, GL_POSITION, lgt_pos);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lgt_dir1);
		glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt_dir);
		gluSphere(q, 30, 50, 10);
	glPopMatrix();
}

void skybox(){
	glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[6]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size,  size, -size);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0, size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(size,  size,  -size);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[8]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f( size, 0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0,  size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f( size, size, size);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size, size, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, size,  -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[10]);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size, 0., size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0.,  size);
  glTexCoord2f(1., 1.);  glVertex3f(size, 0., -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, 0., -size);
  glEnd();

}

//---------------------------------------------------------------------
void initialise(void)
{
	float black[4] = {0.0, 0.0, 0.0, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
	float mat_col[4] = {1.0, 1.0, 0.0, 1.0};

	q =  gluNewQuadric ( );


    loadGLTextures();
	loadMeshFile("Cannon.off");
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, black);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10.1);

	glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, black);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,10.1);

	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT3, GL_SPECULAR, white);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT,10.1);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_col);
 	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
 	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glEnable(GL_COLOR_MATERIAL);


	gluQuadricDrawStyle (q, GLU_FILL );
	gluQuadricNormals	(q, GLU_SMOOTH );

	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture (q, GL_TRUE);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80.0, 1.0, 100000.0, 700000.0);   //Perspective projection
}

//---------------------------------------------------------------------
void display(void)
{
	float xlook, zlook, ylook; //TODO look up and down too
	float cdr=3.14159265/180.0;	//Conversion from degrees to radians
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float lgt_pos0[] = {0.0f, 8000.0f, 150000.0f, 1.0f};
	float lgt_pos3[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float lgt_dir3[] = {-1., -1., 0.};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	xlook = -100.0*sin(lookAngle*cdr);
	zlook = -100.0*cos(lookAngle*cdr);


	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos0);   //light position

	if (changeCamera) {
		// TODO Sort this shit out
		gluLookAt(0, shipChange * 1000, 140000,  0, 0, 140000,   0, 1, 0);
	} else {
		gluLookAt(eye_x, size/2, eye_z,  look_x, size/2, look_z,   0, 1, 0);
	}

	glRotatef(180, 0, 1, 0);

	//printf("%lf x: \n", eye_x);
	//printf("%lf z: \n", eye_z);
	glPushMatrix();
		glTranslatef(-120000, 1000, 160000);
		glScalef(1500, 1500, 1500);
    	drawCastle();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-120000, 0, 160000);
		glScalef(300, 300, 300);
		drawUfo();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, 0, 30000);
		glRotatef(-robotAngle, 0, 1, 0);
		glTranslatef(0, 0, -30000);
		glRotatef(90, 0, 1, 0);
		glScalef(5000, 5000, 5000);
		drawRobot();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glTranslatef(robot_1_pos[0], robot_1_pos[1], robot_1_pos[2]);
		glRotatef(robotAngle1, 0, 1, 0);
		glLightfv(GL_LIGHT3, GL_POSITION, lgt_pos3);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lgt_dir3);
		glScalef(800, 800, 800);
		drawRobot2();
	glPopMatrix();


	skybox();


	glutSwapBuffers();
	glFlush();

}


void setRobotState()
{
	if (robotState == 0 && robot_1_pos[2] <= -(size - 8000)) {
		robotState = 1;
		robotAngle1 -= 90;
	} else if (robotState == 1 && robot_1_pos[0] >= (size - 8000)) {
		robotState = 2;
		robotAngle1 -= 90;
	} else if (robotState == 2 && robot_1_pos[2] >= (size - 8000)) {
		robotState = 3;
		robotAngle1 -= 90;
	} else if (robotState == 3 && robot_1_pos[0] <= -(size - 8000)) {
		robotState = 0;
		robotAngle1 -= 90;
	}
}


void timmerFunc(int val) {
	if (shipLaunched) {
		shipHeight += 5;
		shipChange += 10;
	} else {
		if (shipUp) {
			shipHeight += 0.75;
			if (shipHeight >= 200) {
				shipUp = false;
			}
		} else {
			shipHeight -= 0.75;
			if (shipHeight <= 0) {
				shipUp = true;
			}
		}
		shipChange += 2.5;
	}


	if (val == 1) {
        theta += 1;
        if (theta >= 20) {
            theta = 20;
            val = 0;
        }
    } else {
        theta -= 1;
        if (theta <= -20) {
            theta = -20;
            val = 1;
        }
    }


	lightAngle ++;
	if(lightAngle > 360) lightAngle = 0;

	robotAngle += 2;

	if (fireCannon) {
		float time = 0.05 * cannonCount;
		ball_pos[0] = cannonVelocity * time * cos(cannonRadians) + 38.88;
		ball_pos[1] = -0.5 * gravAccel * pow(time, 2) + cannonVelocity * time * sin(cannonRadians) + 64;
		cannonCount++;
		if (ball_pos[1] <= 0) {
			fireCannon = false;
		}
	}

	if (robotState == 0) {
		robot_1_pos[2] -= 4000;
	} else if (robotState == 1) {
		robot_1_pos[0] += 4000;
	} else if (robotState == 2) {
		robot_1_pos[2] += 4000;
	} else if (robotState == 3) {
		robot_1_pos[0] -= 4000;
	}

	setRobotState();


    glutTimerFunc(50, timmerFunc, val);
    glutPostRedisplay();
}


void keys(unsigned char key_t, int x, int y)
{
	if (key_t == 115) {
		shipLaunched = true;
	}
	if (key_t == 99) {
		printf("%s\n", "noot");
		fireCannon = true;
	}
}

//--------------------------------------------------------------
 void special(int key, int x, int y)
 {
	 float start_x = eye_x;
	 float start_z = eye_z;
	 if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
 	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
 	else if(key == GLUT_KEY_DOWN)
 	{  // Move backward
 		eye_x -= 5000*sin(angle);
 		eye_z += 5000*cos(angle);
 	}
 	else if(key == GLUT_KEY_UP)
 	{ // Move forward
 		eye_x += 5000*sin(angle);
 		eye_z -= 5000*cos(angle);
 	}

	// Bounding box within the sky box
	if (eye_x >= 170000 || eye_x <= -170000) {
		eye_x = start_x;
	}
	if (eye_z >= 170000 || eye_z <= -170000) {
		eye_z = start_z;
	}

	if (key == GLUT_KEY_HOME)
	{
		changeCamera = !changeCamera;
	}

 	look_x = eye_x + 100*sin(angle);
 	look_z = eye_z - 100*cos(angle);
 	glutPostRedisplay();
}
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize (700, 700);
   glutInitWindowPosition (50, 50);

   glutCreateWindow ("Assigment 1");
   initialise();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
	glutKeyboardFunc(keys);
	glutTimerFunc(50, timmerFunc, 1);
   glutMainLoop();
   return 0;
}
