//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: Cannon.cpp
//  See Lab02.pdf for details
//
//  Program displays the model of a cannon (mesh data from Cannon.off)
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

//--Globals ---------------------------------------------------------------
float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters
float cannonAngle = 45;
float ball_pos[3] = {38.88, 64, 0};
GLuint texId[6];
float theta = 20;
float shipHeight = 0;
float shipChange = 0;
bool shipLaunched = false;
float robot_z = 0;
float robot_angle = 90;
float cannonRadians = 0.785398;
float cannonVelocity = 30;
bool fireCannon = false;
int cannonCount = 1;
GLUquadricObj*	q;
bool shipUp = true;
float doorAngle = 30;
bool doorOut = true;

void loadGLTextures()				// Load bitmaps And Convert To Textures
{
	glGenTextures(5, texId); 		// Create texture ids
	// *** left ***
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

	glBindTexture(GL_TEXTURE_2D, texId[5]);
	loadBMP("doorTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


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


	// Doors right
	glPushMatrix();
		glTranslatef(-15., 0., -50.);
		glRotatef(doorAngle, 0, 1, 0);
		drawWall(17, 30, 5, 5);
	glPopMatrix();

	// Doors left
	glPushMatrix();
		glTranslatef(20., 0., -55.);
		glRotatef(-doorAngle + 180, 0, 1, 0);
		drawWall(17, 30, 5, 5);
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

void drawRobot()
{ // Teapot robot
	// Wheel left
	glPushMatrix();
		glTranslatef(0, 2.5, -5);
		glutSolidSphere(2.5, 50, 10);
	glPopMatrix();

	// Wheel right
	glPushMatrix();
		glTranslatef(0, 2.5, 5);
		glutSolidSphere(2.5, 50, 10);
	glPopMatrix();

	// Leg left (not really a leg)
	glPushMatrix();
		glTranslatef(0, 20, -5);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 1);
	glPopMatrix();

	// Leg right (not really a leg)
	glPushMatrix();
		glTranslatef(0, 20, 5);
		glRotatef(90, 1, 0, 0);
		drawTower(50, 1, 20, 1);
	glPopMatrix();

	// body
	glPushMatrix();
		glTranslatef(2.5, 20, 7.5);
		glRotatef(90, 0, 1, 0);
		drawWall(15, 15, 4, 2);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	// Head / Teapot
	glPushMatrix();
		glTranslatef(0, 38, 0);
		glutSolidTeapot(4);
	glPopMatrix();

	// Eyes
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(3, 39, -2);
		glutSolidSphere(0.5, 50, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(3, 39, 2);
		glutSolidSphere(0.5, 50, 10);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void drawRobot3()
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

//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -400; x <= 400; x += 20)
	{
		for(int z = -400; z <= 400; z += 20)
		{
			if(flag) glColor3f(0.6, 1.0, 0.8);
			else glColor3f(0.8, 1.0, 0.6);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+20);
			glVertex3f(x+20, 0, z+20);
			glVertex3f(x+20, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}


void drawUfo()
{
	float lgt_pos[] = {0., 0., 0.};
	float lgt_dir[] = {1., -1., 0.};
	float lgt_dir1[] = {-1., -1., 0.};


	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, 35, 0);
		glRotatef(90, 1, 0, 0);
		glutSolidCone(100, 15, 50, 10);
	glPopMatrix();

	glPushMatrix();

		glTranslatef(0, 38, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(100, 15, 50, 10);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPushMatrix();
		glRotatef(shipChange, 0, 1, 0);
		glTranslatef(0, 40, 0);
		glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt_dir1);
		glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lgt_dir);
		gluSphere(q, 30, 50, 10);
	glPopMatrix();
}


void drawRobot4()
{

	glPushMatrix();
		glColor3f(1, 1, 0);
		glTranslatef(0, 5.9, 2);
		glutSolidCube(1.4);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(0.25, 6.1, 2.7);
		glutSolidSphere(0.15, 50, 10);
	glPopMatrix();
	glPushMatrix();
		glColor3f(0, 1, 0);
		glTranslatef(-0.25, 6.1, 2.7);
		glutSolidSphere(0.15, 50, 10);
	glPopMatrix();

			//Torso
	glPushMatrix();
	  glColor3f(0, 0, 0);
	  glTranslatef(0, 4.5, 0);
	  glScalef(3, 1.4, 5.4);
	  glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(0, 10, 0);
		glRotatef(robot_angle, 1, 0, 0);
		drawTower(50, 0.1, 5, 1);
	glPopMatrix();

	//Right leg
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-0.8, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(-0.8, 2.2, -2);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	//Left leg
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-0.8, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(0.8, 2.2, -2);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	//Right leg front
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-0.8, 4, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(-0.8, 2.2, 2);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

	//Left leg front
	glPushMatrix();
		glColor3f(1, 0, 0);
      glTranslatef(-0.8, 4, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(0.8, 2.2, 2);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();
}

void timmerFunc(int val) {
	if (shipLaunched) {
		shipHeight += 5;
	} else {
		if (shipUp) {
			shipHeight += 0.25;
			if (shipHeight >= 40) {
				shipUp = false;
			}
		} else {
			shipHeight -= 0.25;
			if (shipHeight <= 0) {
				shipUp = true;
			}
		}
		shipChange += 2.5;
	}

	if (doorOut) {
		doorAngle += 1;
		if (doorAngle >= 85) {
			doorOut = false;
		}
	} else {
		doorAngle -= 1;
		if (doorAngle  <= 0) {
			doorOut = true;
		}
	}

	robot_angle += 5;



    glutTimerFunc(50, timmerFunc, val);
    glutPostRedisplay();
}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
	float lpos[4] = {100., 100., 100., 1.0};  //light's position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, 100, eye_z,  look_x, 100, look_z,   0, 1, 0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

	glScalef(2, 2, 2);

	glDisable(GL_TEXTURE_2D);
		drawFloor();
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glScalef(1.5, 1.5, 1.5);
    	//drawCastle();
	glPopMatrix();

	//drawCannonBody();

	drawUfo();

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, 5, 0);
		glScalef(10, 10, 10);
		//drawRobot4();
	glPopMatrix();

	//drawShip();
	glPushMatrix();
		glTranslatef(0, 10, 0);
		//animatedThing();
	glPopMatrix();

	glPushMatrix();
		glRotatef(robot_angle, 0, 1, 0);
		glTranslatef(40, 0, 0);
		glRotatef(90, 0, 1, 0);
		//drawRobot2();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-50, 0, -120);
		glRotatef(90, 0, 1, 0);
		//drawRobot2();
	glPopMatrix();

	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	float black[4] = {0.0, 0.0, 0.0, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

	loadGLTextures();
    loadMeshFile("Cannon.off");				//Specify mesh file name here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	q =  gluNewQuadric ( );

 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT,10.1);

	glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, black);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10.1);


    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	gluQuadricDrawStyle (q, GLU_FILL );
	gluQuadricNormals	(q, GLU_SMOOTH );
	gluQuadricTexture (q, GL_TRUE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1, 50, 1000);  //The camera view volume
}

void launchShip(int value) {
	shipHeight += 5;
    glutPostRedisplay();
    glutTimerFunc(50, launchShip, value);
}


//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 10*sin(angle);
		eye_z += 10*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 10*sin(angle);
		eye_z -= 10*cos(angle);
	}

	look_x = eye_x + 100*sin(angle);
	look_z = eye_z - 100*cos(angle);
	glutPostRedisplay();
}

void keys(unsigned char key_t, int x, int y)
{
	if (key_t == 115) {
		if (shipHeight == 0) {
			shipLaunched = true;
		}
	}
	if (key_t == 99) {
		printf("%s\n", "noot");
		fireCannon = true;
	}
}

void moveCannon(int value) {
	if (fireCannon) {
		ball_pos[0] = cannonVelocity * 0.05 * cannonCount * cos(cannonRadians) + 38.88;
		ball_pos[1] = -0.5*9.81*pow((0.05 * cannonCount), 2) + cannonVelocity * (0.05 * cannonCount) * sin(cannonRadians) + 64;
		cannonCount++;
		if (ball_pos[1] <= 0) {
			fireCannon = false;
		}
	}


	if (value == 1) {
        theta += 1;
        if (theta >= 20) {
            theta = 20;
            value = 0;
        }
    } else {
        theta -= 1;
        if (theta <= -20) {
            theta = -20;
            value = 1;
        }
    }


    glutPostRedisplay();
    glutTimerFunc(50, moveCannon, value);
}



//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Cannon");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keys);
   glutTimerFunc(50, moveCannon, 1);
   glutTimerFunc(50, timmerFunc, 1);
   glutMainLoop();
   return 0;
}
