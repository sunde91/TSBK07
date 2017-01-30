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
#include "VectorUtils3.h"
#include <math.h>
#include <stdlib.h>

// 5.1) Hard to model in code, many triangles
// 5.2) Change index order

// Globals
// Data would normally be read from files

/*
GLfloat vertices[12][3][3] = {
    {
        {0,0,1},
        {0,0,0},
        {0,1,0}
    },
    {
        {0,1,0},
        {0,1,1},
        {0,0,1}
    },
    {
        {0,0,1},
        {0,1,1},
        {1,0,1}
    },
    {
        {1,1,1},
        {1,0,1},
        {0,1,1}
    },
    {
        {1,0,0},
        {1,0,1},
        {1,1,0}
    },
    {
        {1,1,1},
        {1,1,0},
        {1,0,1}
    },
    {
        {0,0,0},
        {0,1,0},
        {1,0,0}
    },
    {
        {1,1,0},
        {1,0,0},
        {0,1,0}
    },
    {
        {0,0,0},
        {0,0,1},
        {1,0,0}
    },
    {
        {1,0,1},
        {1,0,0},
        {0,0,1}
    },
    {
        {0,1,0},
        {0,1,1},
        {1,1,0}
    },
    {
        {1,1,1},
        {1,1,0},
        {0,1,1}
    },
};
*/

GLfloat vertices[] = {
    0,0,0,
    0,0,1,
    0,1,0,
    0,1,1,
    1,0,0,
    1,0,1,
    1,1,0,
    1,1,1
};

GLuint indices[] = {
    0,1,3,
    0,3,2,
    0,2,6,
    0,6,4,
    0,1,5,
    0,5,4,
    1,5,7,
    1,7,3,
    5,4,6,
    5,6,7,
    2,3,7,
    2,7,6
};


GLfloat colors[24];

GLfloat myMatrix[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
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
    
        for(int i = 0; i < 24; ++i)
        {
            colors[i] = ((float)rand()) / ((float)RAND_MAX);
            printf("colors[%d] = %f\n",i,colors[i]);
        };

        for(int i = 0; i < 24; ++i)
            vertices[i] -= 0.5f;

        /*
        for(int i = 0; i < 12; ++i)
        {
            for(int ii = 0; ii < 3; ++ii)
            {
                for(int iii = 0; iii < 3; ++iii)
                {
                    vertices[i][ii][iii] -= 0.5f;
                    printf("%f,",vertices[i][ii][iii]);
                }   
            printf(" ");
            }
        printf("\n");
        }
        */
            

	// vertex buffer object, used for uploading the geometry
	GLuint vertexBufferObjID;
    GLuint colorBufferObjID;
    GLuint indexBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &colorBufferObjID);
    glGenBuffers(1, &indexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

        // VBO for color data
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
        GLuint colorLoc = glGetAttribLocation(program, "in_Color"); 
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// End of upload of geometry

	printError("init arrays");
}

GLfloat t;
GLfloat speed = 0.001;
float angleX = 0;
float angleY = 0;
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

    angleX += 0.01;
    angleY += 0.02;
    mat4 rotX = Rx(angleX);
    mat4 rotY = Ry(angleY);
    mat4 rotMat = Mult(rotX, rotY);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, rotMat.m);
}

void display(void)
{
	printError("pre display");
        update();

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 12 * 3 * 3);	// draw object
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0L);

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
