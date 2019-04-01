//  ========================================================================
//  COSC363: Computer Graphics (2019); CSSE  University of Canterbury.
//
//  FILE NAME: Skybox.cpp
//  See Lab03.pdf for details
//  ========================================================================

//TODO: New four legged robot use the leg animation from lab, maybe make it a
	  //trailer like thing with balls in the back
//TODO: Need to cast a shadow of something
//TODO: Fix the spotlights on the ship could add to the new robot
//TODO: Need something animated in the castle
//TODO: Fix second camera mode
//TODO: Billboarding for some trees or something
//TODO: Collison detection of the castle maybe, if there is time


#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include <fstream>
#include <climits>
#include <math.h>
#include "loadTGA.h"
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

GLuint texId[6];


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
	glGenTextures(6, texId); 		// Create texture ids
	// *** left ***
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadTGA("skybox/iceflats_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** front ***
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	loadTGA("skybox/iceflats_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	loadTGA("skybox/iceflats_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** back***
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	loadTGA("skybox/iceflats_bk.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	loadTGA("skybox/iceflats_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, texId[5]);
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


void drawCannonBody()
{
	//--start here
    glPushMatrix();
    glTranslatef(-10.0, 5.0, 17.0);
    glScalef(80.0, 10.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20.0, 25.0, 17.0);
    glScalef(40.0, 30.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0, 5.0, -17.0);
    glScalef(80.0, 10.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20.0, 25.0, -17.0);
    glScalef(40.0, 30.0, 6.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20, 30, 0); //Pivot point coordinates
    glRotatef(30, 0, 0, 1); //Rotation
    glTranslatef(20, -30, 0);
    drawCannonBody();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(ball_pos[0], ball_pos[1], ball_pos[2]);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();
}



void skybox(){
	glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glColor3f(0, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0., -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size, -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size,  size, -size);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(size,  0, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0, size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(size,  size,  -size);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glColor3f(1, 1, 0);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f( size, 0, size);
  glTexCoord2f(1., 0.);  glVertex3f(-size, 0,  size);
  glTexCoord2f(1., 1.);  glVertex3f(-size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f( size, size, size);
  glEnd();

  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glColor3f(1, 0, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size, size, -size);
  glTexCoord2f(1., 0.);  glVertex3f(size, size,  -size);
  glTexCoord2f(1., 1.);  glVertex3f(size, size,  size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, size, size);
  glEnd();

  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glColor3f(0, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0., 0.);  glVertex3f(-size, 0., size);
  glTexCoord2f(1., 0.);  glVertex3f(size, 0.,  size);
  glTexCoord2f(1., 1.);  glVertex3f(size, 0., -size);
  glTexCoord2f(0., 1.);  glVertex3f(-size, 0., -size);
  glEnd();

}


void drawCastle()
{
    GLUquadric *q;
    q = gluNewQuadric();


    glDisable(GL_TEXTURE_2D);
    // Front wall
	glPushMatrix();
        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(0., 43., -10.);
            glScalef(15.0, 3, 1);
            glutSolidCube(5);
        glPopMatrix();

        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(25., 25., -10.);
            glScalef(3.0, 10, 1);
            glutSolidCube(5);
        glPopMatrix();

        glPushMatrix();
            glColor3f(0, 0, 1);
            glTranslatef(-25., 25., -10.);
            glScalef(3.0, 10, 1);
            glutSolidCube(5);
        glPopMatrix();

	glPopMatrix();

    // Back wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(0., 25., 60.);
		glScalef(15.0, 10, 1);
    	glutSolidCube(5);
	glPopMatrix();

    // left wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(35., 25., 25.);
        glRotatef(90, 0., 1., 0.);
		glScalef(15.0, 10, 1);
    	glutSolidCube(5);
	glPopMatrix();

    // right wall
	glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(-35., 25., 25.);
        glRotatef(90, 0., 1., 0.);
        glScalef(15.0, 10, 1);
        glutSolidCube(5);
	glPopMatrix();

    // tower 1
    glPushMatrix();
        glPushMatrix();
            glTranslatef(35, 60, -12);
            glRotatef(90, 0, 1, 0);
            glScalef(0.15, 0.15, 0.15);
            drawCannonBody();
        glPopMatrix();

       glRotatef(90, 1, 0, 0);
       glTranslatef(-35, -10, -60);
       gluCylinder(q, 5, 5, 60, 20, 3);
       gluQuadricDrawStyle(q, GLU_FILL);
       glRotatef(90, 0, 0, 1);
       glTranslatef(20, 10, -20);
   glPopMatrix();


   // tower 1
   glPushMatrix();
       glPushMatrix();
           glTranslatef(-35, 60, -12);
           glRotatef(90, 0, 1, 0);
           glScalef(0.15, 0.15, 0.15);
           drawCannonBody();
       glPopMatrix();

      glRotatef(90, 1, 0, 0);
      glTranslatef(35, -10, -60);
      gluCylinder(q, 5, 5, 60, 20, 3);
      gluQuadricDrawStyle(q, GLU_FILL);
      glRotatef(90, 0, 0, 1);
      glTranslatef(20, 10, -20);
  glPopMatrix();


  // tower 1
  glPushMatrix();
     glRotatef(90, 1, 0, 0);
     glTranslatef(-35, 60, -60);
     gluCylinder(q, 5, 5, 60, 20, 3);
     gluQuadricDrawStyle(q, GLU_FILL);
     glRotatef(90, 0, 0, 1);
     glTranslatef(20, 10, -20);
 glPopMatrix();



 // tower 1
 glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(35, 60, -60);
    gluCylinder(q, 5, 5, 60, 20, 3);
    gluQuadricDrawStyle(q, GLU_FILL);
    glRotatef(90, 0, 0, 1);
    glTranslatef(20, 10, -20);
glPopMatrix();


glPushMatrix();
    glTranslatef(25, 0, -35);
    glRotatef(90, 0, 1, 0);
    glScalef(0.25, 0.25, 0.25);
    drawCannonBody();
glPopMatrix();


glPushMatrix();
    glTranslatef(-25, 0, -35);
    glRotatef(90, 0, 1, 0);
    glScalef(0.25, 0.25, 0.25);
    drawCannonBody();
glPopMatrix();


    glEnable(GL_TEXTURE_2D);
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

	drawCastle();

	drawCannon();

	skybox();



	glFlush();
}

//--------------------------------------------------------------
 void special(int key, int x, int y)
 {
	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
 	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
 	else if(key == GLUT_KEY_DOWN)
 	{  //Move backward
 		eye_x -= 0.1*sin(angle);
 		eye_z += 0.1*cos(angle);
 	}
 	else if(key == GLUT_KEY_UP)
 	{ //Move forward
 		eye_x += 0.1*sin(angle);
 		eye_z -= 0.1*cos(angle);
 	}

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

   glutMainLoop();
   return 0;
}
