// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>

// 4.1) No
// 4.2) same?
// 4.3) Gouraud shading, could use phong instead

// Globals
// Data would normally be read from files

GLfloat vertices[] =
{
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f
};
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f   
};

GLfloat myMatrix[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.5f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
};


// vertex array object
unsigned int vertexArrayObjID;
// Reference to shader program
GLuint program;

void init(void)
{

	// vertex buffer object, used for uploading the geometry
	GLuint vertexBufferObjID;
        GLuint colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-4.vert", "lab1-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
        glGenBuffers(1, &colorBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

        // VBO for color data
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
        GLuint colorLoc = glGetAttribLocation(program, "in_Color"); 
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);


	// End of upload of geometry

	printError("init arrays");
}

GLfloat t;
GLfloat speed = 0.001;
void update(void)
{      
    GLfloat new_t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    GLfloat dt = new_t - t;
    t = new_t;

    GLfloat tt = speed * t;
    myMatrix[0][0] = cos(tt);
    myMatrix[0][1] = -sin(tt);
    myMatrix[1][0] = sin(tt);
    myMatrix[1][1] = cos(tt);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
}

void display(void)
{
	printError("pre display");
        update();

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

	printError("display");

	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
        printf("%s\n",__FILE__);
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}