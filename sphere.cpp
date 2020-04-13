/**** General Makefile to complie and execute a file X assigned to the input

#for linux users
all: $(input)
  g++ -o output $(input) -lglut -lGL

#for windows users
all: $(input)
  g++ -o output $(input) -IC:\freeglut\include -LC:\freeglut\lib -lopengl32 -lfreeglut

#compile commmand
make input=X.cpp

****/

#include "angel/Angel.h"
#include <GL/freeglut.h>
#include <math.h>

using namespace std;

const int N = 342;

vec3 quad_data[N]; // 8 rows of 18 quads


void display(void);

void keyPressed(unsigned char key, int x, int y);

void writeBitmapString(void *font, char *string);


int main(int argc, char** argv) {

  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_SINGLE);
  glutCreateWindow("Sphere");

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;
}


/*

  1) we start off by converting the Degrees to Radians because it works better with the standard trigonometric functions (sin, cos, etc..)

  2) we create nested for loops with degrees right why ?

      * if we were to put the values of these 2 loops inside an array we can call it a grid right then to imagine why we need this in
    a sphere read the next point.

      * take this flat two-dimensional grid and take it and kind of wrap it around a sphere you know it's this way this is
    like the map of the earth that we've taken and wrapped around a sphere and each point along the sphere is represented
    by a longitude(phi from -80 to 80) meaning which one of these columns so to speak by a latitude(theta from -180 to 180)
    which one of these rows so to speak.

      * you might ask yourself why do we increase either the theta or phi by 20 ?
     # By generating points at fixed increments of θ and φ, we can specify quadrilaterals


  3) you might find that we get the x,y,z by using  sin, cos and here's why

  * there are 2 known coordinate systems

    # Cartesian => (x, y)


    # Polar => (r, Θ)  => by using a radius r and a theta we can go anywhere like x,y => we're going to be using the polar
       and convert it to cartesian.

                |
                |      / |
                |     /  |
                | r  /   | y
                |   /    |
                |  /     |
    ___________ |_/Θ_____|_______
                |     x
                |
                |
                |


       # sin(Θ) = y / r

         y = r * sin(Θ)

       # cos(Θ) = x / r

         x = r * cos(Θ)

      # so using a similar analogy like the above but what's different this time is that we have 2 angles the longitude(θ), latitude(φ)
        and we have the z dimension we can deduce the following formulat like the book :

            x(θ , φ) = sin θ cos φ,
            y(θ , φ) = cos θ cos φ,
            z(θ , φ) = sin φ.

  */


void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0, 1.0, 1.0);

  glRasterPos3f(-0.9, 0.5, 0.0);
  writeBitmapString(GLUT_BITMAP_8_BY_13, "Please press a keyboard key for your desired point in question 1 => a or b or c or d or e");
  glFlush();

  /*

   1) what's a radian : another unit for measuring angles and why do we need it ?
        because it works better with circular functions (sin, cost,etc..)

     *  How do we convert to radian =>  Degree * π / 180 that's why saved the conversion in a variable and we'll use it later in the code to convert a degree to radian

  */

  const float DEGREE_TO_RADIAN = M_PI / 180.0; // M_PI = 3.14159...

  const float PHI_DEGREE = 80.0;

  const float THETA_DEGREE = 180.0;

  const float INCREASING_VALUE = 20.0;

  int k = 0;

  for (float phi = -PHI_DEGREE ; phi <= PHI_DEGREE ; phi += INCREASING_VALUE)
  {

    // convert phi degree to radian
    float phir = phi * DEGREE_TO_RADIAN;

    // add 20 to the degree then convert it to radian to have the shape of the sphere not circles inside each other
    float phir20 = (phi + INCREASING_VALUE) * DEGREE_TO_RADIAN;

    for (float theta = -THETA_DEGREE ; theta <= THETA_DEGREE ; theta += INCREASING_VALUE)
    {

      // convert theta degree to radian
      float thetar = theta * DEGREE_TO_RADIAN;

      // save the x, y, z coordinates to a vector
      quad_data[k] = vec3(sin(thetar) * cos(phir), cos(thetar) * cos(phir), sin(phir));
      k++;


      // save the x, y, z coordinates to the vector again but with adding the phir20
      quad_data[k] = vec3(sin(thetar) * cos(phir20), cos(thetar) * cos(phir20), sin(phir20));
      k++;

    }

  }

  glutKeyboardFunc(keyPressed);

}

void writeBitmapString(void *font, char *string)
{
  char *c;
  for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void keyPressed(unsigned char key, int x, int y) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  /*
      A polygon has two sides---front and back---and might be rendered differently
      depending on which side is facing the viewer. This allows you to have
      cutaway views of solid objects in which there is an obvious distinction
      between the parts that are inside and those that are outside. By default,
      both front and back faces are drawn in the same way. To change this, or to
      draw only outlines or vertices, use glPolygonMode(). it controls the drawing mode for
      a polygon’s front and back faces. The parameter face must be GL_FRONT_AND_BACK; while
      mode can be GL_POINT, GL_LINE, GL_FILL to indicate whether the polygon should
      be drawn as points, outlined, or filled. By default, both the front and
      back faces are drawn filled.
  */
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


  switch (key) {
  case 'a' :

    glColor3f(1.0, 1.0, 1.0);

    /*
         remember that GL_LINES works by connecting each two points but it won't connect each 4 points
         together that's why we need to redraw the points that hasn't been connected again to let it
         connect(the first if condition) you can see how GL_POLYGON works by reducing the N.
         and by observing the GL_POLYGON you will find that it connects the last point to the first point
         which exactly what the second if condition does.
    */

    glBegin(GL_LINES);

    for (int i = 0; i < N; i = i + 1 ) {

      glVertex3d(quad_data[i][0], quad_data[i][1], quad_data[i][2]);


      if ( i != 0  &&  i + 1 != N )
      {

        glVertex3f(quad_data[i][0], quad_data[i][1], quad_data[i][2]);
      }


      if ( i + 1 == N )
      {

        glVertex3f(quad_data[i][0], quad_data[i][1], quad_data[i][2]);
        glVertex3f(quad_data[0][0], quad_data[0][1], quad_data[0][2]);
      }


    }

    glEnd();

    break;

  case 'b':

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);

    for (int i = 0; i < N; i++)
      glVertex3d(quad_data[i][0], quad_data[i][1], quad_data[i][2]);
    glEnd();

    break;

  case 'c':

    glColor3d(0.1, 0.7, 0.6);

    glBegin(GL_POLYGON);

    for (int i = 0; i < N; i++)
      glVertex3d(quad_data[i][0], quad_data[i][1], quad_data[i][2]);
    glEnd();

    break;

  case 'd':

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);

    for (int i = 0; i < N; i++) {
      glColor3d((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
      glVertex3d(quad_data[i][0], quad_data[i][1], quad_data[i][2]);
    }
    glEnd();

    break;

  case 'e':

    glColor3f(1.0, 1.0, 1.0);

    glRasterPos3f(-0.95, 0.5, 0.0);
    writeBitmapString(GLUT_BITMAP_8_BY_13, "(e) answer => Lighting gives more illusion of a 3d shape and shows what's near and what's far");

    break;

  default:

    glColor3f(1.0, 1.0, 1.0);

    glRasterPos3f(-0.2, 0.5, 0.0);
    writeBitmapString(GLUT_BITMAP_8_BY_13, "Key is not available");

    break;

  }

  glFlush();

}
