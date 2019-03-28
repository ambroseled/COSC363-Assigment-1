//  ========================================================================
//  COSC363: Computer Graphics (2019); CSSE  University of Canterbury.
//
//  FILE NAME: Skybox.cpp
//  See Lab03.pdf for details
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "loadTGA.h"
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
float lookAngle = 0.0;		//Camera rotation
float movePos = 0.0;
float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters
float size = 200000;


GLuint texId[6];

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

//========================================================================================

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


void drawCastle(void)
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glColor3f(0, 0, 1);

		glTranslatef(0., 300., -200000.);
		glScalef(500.0, 500, 1);
    	glutSolidCube(500);
	glPopMatrix();
}

//---------------------------------------------------------------------
void initialise(void)
{
    loadGLTextures();

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

   glutCreateWindow ("Sky Box");
   initialise();
   glutDisplayFunc(display);
   glutSpecialFunc(special);

   glutMainLoop();
   return 0;
}
