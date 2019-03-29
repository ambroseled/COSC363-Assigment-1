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
float angle = -10.0;  //Rotation angle for viewing
float cam_hgt = 100;
float cannonAngle = 45;
float ball_pos[3] = {38.88, 64, 0};
GLuint texId[1];
float theta = 20;
float shipHeight = 0;


void loadGLTextures()				// Load bitmaps And Convert To Textures
{
	glGenTextures(1, texId); 		// Create texture ids
	// *** left ***
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadBMP("brickTexture.bmp");
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


void drawShip()
{
	glPushMatrix();
		glTranslatef(20, shipHeight, 10);
		glutSolidCube(20);
	glPopMatrix();
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

//--Draws a character model constructed using GLUT objects ------------------
void drawModel()
{
    glColor3ub(-theta, theta + 69, theta);		//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidCube(1.4);
	glPopMatrix();

    glColor3ub(theta * 324, theta + 69, -theta);			//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

    glColor3ub(-theta, theta - 69, -theta);			//Right leg
	glPushMatrix();
      glTranslatef(-0.8, 4, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(0.8, -4, 0);
	  glTranslatef(-0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

    glColor3ub(theta + 167, theta + 69, theta);			//Left leg
	glPushMatrix();
      glTranslatef(0.8, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(-0.8, -4, 0);
      glTranslatef(0.8, 2.2, 0);
	  glScalef(1, 4.4, 1);
	  glutSolidCube(1);
	glPopMatrix();

    glColor3ub(-theta, theta + 69, -theta);			//Right arm
	glPushMatrix();
      glTranslatef(-2, 6.5, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(2, -6.5, 0);
	  glTranslatef(-2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();

    glColor3ub(-theta * 8, theta + 69, theta * 17);			//Left arm
	glPushMatrix();
      glTranslatef(2, 6.5, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(-2, -6.5, 0);
	  glTranslatef(2, 5, 0);
	  glScalef(1, 4, 1);
	  glutSolidCube(1);
	glPopMatrix();
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


void drawWall(float length, float height, float width)
{
	glEnable(GL_TEXTURE_2D);
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
    glRotatef(cannonAngle, 0, 0, 1); //Rotation
    glTranslatef(20, -30, 0);
    drawCannon();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslatef(ball_pos[0], ball_pos[1], ball_pos[2]);
    glutSolidSphere(5, 36, 18);
    glPopMatrix();
}




void drawTower(int n, float x, float y) {
    /*
        Function drw_polygon:
        Arguments:
            n - number of sides
            arg - starting angle (not so important at all)
            mult - multiplying sides to incrase their length
            v - cylinder height
    */
    // Cylinder Bottom
    glBegin(GL_POLYGON);
        glColor4f(1.0, 0.0, 0.0, 1.0);
        for(int i = 0; i <= (360); i += (360 / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), 0.0);
        }
    glEnd();

    // Cylinder Top
    glBegin(GL_POLYGON);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        for(int i = 0; i <= (360); i += (360 / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();

    // Cylinder "Cover"
    glBegin(GL_QUAD_STRIP);
        glColor4f(1.0, 1.0, 0.0, 1.0);
        for(int i = 0; i < 480; i += (360 / n)) {
            float a = i * M_PI / 180; // degrees to radians
            glVertex3f(x * cos(a), x * sin(a), 0.0);
            glVertex3f(x * cos(a), x * sin(a), y);
        }
    glEnd();
}


void newCastle()
{

	glDisable(GL_TEXTURE_2D);


	// Front wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-35., 0., -50.);
		drawWall(20, 40, 5);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(20., 0., -50.);
		drawWall(20, 40, 5);
	glPopMatrix();


	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-15., 30., -50.);
		drawWall(35, 10, 5);
	glPopMatrix();



	// Back wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-35., 0., 25.);
		drawWall(80, 40, 5);
	glPopMatrix();

	// left wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(45., 0., 25.);
		glRotatef(90, 0., 1., 0.);
		drawWall(80, 40, 5);
	glPopMatrix();

	// right wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-35., 0., 25.);
		glRotatef(90, 0., 1., 0.);
		drawWall(80, 40, 5);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	// Back Right Tower
	glPushMatrix();
		//glRotatef(90, 1, 0, 0);
		glTranslatef(-37, 50, 22);
		glRotatef(90, 1, 0, 0);
		drawTower(30, 5, 50);
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
		drawTower(30, 5, 50);
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
		drawTower(30, 5, 50);
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
		drawTower(30, 5, 50);
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
	glEnable(GL_TEXTURE_2D);
}



// Old castledrawModel()
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
	gluLookAt(0, cam_hgt, 200, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

	glDisable(GL_TEXTURE_2D);
	drawFloor();
	glEnable(GL_TEXTURE_2D);

    //drawCannonBody();
	glPushMatrix();
		glScalef(1.5, 1.5, 1.5);
    	newCastle();
	glPopMatrix();

	glPushMatrix();
		glScalef(3, 3, 3);
    	drawModel();
	glPopMatrix();

	drawShip();
	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	loadGLTextures();
    loadMeshFile("Cannon.off");				//Specify mesh file name here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1, 50, 1000);  //The camera view volume
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 5;
    else if(key == GLUT_KEY_RIGHT) angle += 5;
	else if(key == GLUT_KEY_UP) cam_hgt += 5;
	else if(key == GLUT_KEY_DOWN) cam_hgt -= 5;

	if(cam_hgt > 200) cam_hgt = 200;
	else if(cam_hgt < 10) cam_hgt = 10;


	glutPostRedisplay();
}

void moveCannon(int value) {


	if (value == 1) {
		cannonAngle += 0.4;
		if (cannonAngle >= 65) value = 0;
	} else {
		cannonAngle -= 0.4;
		if (cannonAngle <= 5) value = 1;
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
   glutTimerFunc(50, moveCannon, 1);
   glutMainLoop();
   return 0;
}
