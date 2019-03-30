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
float size = 200000;
float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters

float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float ball_pos[3] = {38.88, 64, 0};

GLuint texId[11];
float cannonAngle = 45;
float shipHeight = 0;
float shipChange = 0;
bool shipLaunched = false;


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
	glBegin(GL_QUADS);


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
	    glTranslatef(ball_pos[0], ball_pos[1], ball_pos[2]);
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
	int numSegments = 360;
	float textInc = 1.0f / numSegments;
    // Cylinder Bottom
    glBegin(GL_POLYGON);
        glColor4f(1.0, 0.0, 0.0, 1.0);
        for(int i = 0; i <= (numSegments); i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), 0.0);
        }
    glEnd();

    // Cylinder Top
    glBegin(GL_POLYGON);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        for(int i = 0; i <= (numSegments); i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();

    // Cylinder "Cover"
    glBegin(GL_QUAD_STRIP);
        glColor4f(1.0, 1.0, 0.0, 1.0);
        for(int i = 0; i < 480; i += (numSegments / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glTexCoord2f(textInc * i, 0); glVertex3f(x * cos(a), x * sin(a), 0.0);
            glTexCoord2f(textInc * i, 1); glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();
}

void drawShip()
{

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

void skybox(){
	glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[6]);
  glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size,  size, -size);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[7]);
  glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0, size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(size,  size,  -size);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[8]);
  glColor3f(1, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f( size, 0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0,  size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f( size, size, size);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[9]);
  glColor3f(1, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size, size, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, size,  -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[10]);
  glColor3f(0, 1, 1);
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
    loadGLTextures();
	loadMeshFile("Cannon.off");
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);


    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(80.0, 1.0, 10000.0, 500000.0);   //Perspective projection
}

//---------------------------------------------------------------------
void display(void)
{
	float xlook, zlook, ylook; //TODO look up and down too
	float cdr=3.14159265/180.0;	//Conversion from degrees to radians
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	xlook = -100.0*sin(lookAngle*cdr);
	zlook = -100.0*cos(lookAngle*cdr);

	gluLookAt(eye_x, size/2, eye_z,  look_x, size/2, look_z,   0, 1, 0);

	glPushMatrix();
		glTranslatef(0, 1000, 150000);
		glScalef(850, 850, 850);
    	drawCastle();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 0, 150000);
		glScalef(450, 450, 450);
		drawShip();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(25000, 0, 80000);
		glRotatef(90, 0, 1, 0);
		glScalef(450, 450, 450);
		drawRobot2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25000, 0, 80000);
		glRotatef(90, 0, 1, 0);
		glScalef(450, 450, 450);
		drawRobot2();
	glPopMatrix();


	skybox();



	glFlush();
}


void timmerFunc(int val) {
	if (shipLaunched) {
		shipHeight += 5;
		shipChange += 20;
	} else {
		shipChange += 2.5;
	}


    glutTimerFunc(50, timmerFunc, val);
    glutPostRedisplay();
}


void keys(unsigned char key_t, int x, int y)
{
	if (key_t == 115) {
		if (shipHeight == 0) {
			shipLaunched = true;
		}
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
 	{  //Move backward
 		eye_x -= 1000*sin(angle);
 		eye_z += 1000*cos(angle);
 	}
 	else if(key == GLUT_KEY_UP)
 	{ //Move forward
 		eye_x += 1000*sin(angle);
 		eye_z -= 1000*cos(angle);
 	}

	//TODO collison detection on skybox

 	look_x = eye_x + 100*sin(angle);
 	look_z = eye_z - 100*cos(angle);
 	glutPostRedisplay();
}
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH );
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
