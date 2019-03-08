#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#include <iostream> //<iostream.h> is deprecated

using namespace std; //added to make cout and cin work without specifying namespace

//basically a point class
struct screenPt
{
	GLint x;
	GLint y;
};

typedef enum { limacon = 1, cardioid, threeLeaf, fourLeaf, spiral } curveName; //an enum that correlates input to the shape to draw

GLsizei winWidth = 600, winHeight = 500;    // Initial display window size.

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //clears the canvas and sets the color to white (Red, Green, Blue, Alpha)

	glMatrixMode(GL_PROJECTION); //projects orthogonally over a range from (0 - 200, 0 - 150)
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

//draws a line between two points
void lineSegment(screenPt pt1, screenPt pt2)
{
	glBegin(GL_LINES);//starts drawing
	glVertex2i(pt1.x, pt1.y);//first point
	glVertex2i(pt2.x, pt2.y);//second point
	glEnd();//stop drawing
}

void drawCurve(GLint curveNum)
{
	/*  The limacon of Pascal is a modification of the circle equation
	 *  with the radius varying as r = a * cos (theta) + b, where a
	 *  and b are constants.  A cardioid is a limacon with a = b.
	 *  Three-leaf and four-leaf curves are generated when
	 *  r = a * cos (n * theta), with n = 3 and n = 2, respectively.
	 *  A spiral is displayed when r is a multiple of theta.
	 */

	const GLdouble twoPi = 6.283185;
	const GLint a = 175, b = 60;

	GLfloat r, theta, dtheta = 1.0 / float(a);
	GLint x0 = 200, y0 = 250;   // Set an initial screen position.  
	screenPt curvePt[2];

	glColor3f(0.0, 0.0, 0.0);       //  Set curve color to black.

	curvePt[0].x = x0;      // Initialize curve position.
	curvePt[0].y = y0;

	switch (curveNum) { //sets the first point to start drawing at depending on which shape
	case limacon:    curvePt[0].x += a + b;  break;
	case cardioid:   curvePt[0].x += a + a;  break;
	case threeLeaf:  curvePt[0].x += a;      break;
	case fourLeaf:   curvePt[0].x += a;      break;
	case spiral:     break;
	default:         break;
	}

	theta = dtheta;
	while (theta < twoPi) { //two_Pi is a typo --> changed to twoPi

		switch (curveNum) { //sets r == what the radius should be at the next point
		case limacon:
			r = a * cos(theta) + b;    break;
		case cardioid:
			r = a * (1 + cos(theta));  break;
		case threeLeaf:
			r = a * cos(3 * theta);    break;
		case fourLeaf:
			r = a * cos(2 * theta);    break;
		case spiral:
			r = (a / 4.0) * theta;      break;
		default:                        break;
		}

		curvePt[1].x = x0 + r * cos(theta);//sets the next point based on the first point and radius
		curvePt[1].y = y0 + r * sin(theta);
		lineSegment(curvePt[0], curvePt[1]);//draws the next line segment

		curvePt[0].x = curvePt[1].x;//the current point is set to the last point to draw from the end of the line
		curvePt[0].y = curvePt[1].y;
		theta += dtheta; //theta is incremented by dtheta
	}
}

//various IO stuff, not much here
void displayFcn(void)
{
	GLint curveNum;

	glClear(GL_COLOR_BUFFER_BIT);   //  Clear display window.

	cout << "\nEnter the integer value corresponding to\n";
	cout << "one of the following curve names.\n";
	cout << "Press any other key to exit.\n";
	cout << "\n1-limacon, 2-cardioid, 3-threeLeaf, 4-fourLeaf, 5-spiral:  ";
	cin >> curveNum;

	if (curveNum == 1 || curveNum == 2 || curveNum == 3 || curveNum == 4
		|| curveNum == 5)
		drawCurve(curveNum);
	else
		exit(0);

	glFlush(); //runs all routines at maximum speed
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION); //resizes orthogonal rendering to the new size
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)newWidth, 0.0, (GLdouble)newHeight);

	glClear(GL_COLOR_BUFFER_BIT); //clears the window
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100); //initializes window to be created at 100, 100 px
	glutInitWindowSize(winWidth, winHeight); //sets the size of the window to be winWidth, winHeight
	glutCreateWindow("Draw Curves"); //makes a window that is titles 'Draw Curves'

	init();
	glutDisplayFunc(displayFcn); //first class function call that sets the glut display function to be displayFcn()
	glutReshapeFunc(winReshapeFcn); //first class function call that sets the glut reshape function to be winReshapeFcn()

	glutMainLoop(); //main loop that updates the window and checks for IO
}
