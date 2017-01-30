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
#include "loadobj.h"
#include <math.h>
#include <stdlib.h>

// 1.1) r = g = x+y
// 1.2) cosine / sine with time

// Globals
// Data would normally be read from files

#define LOAD_SHADERS(NAME) loadShaders("shaders/" NAME ".vert",  "shaders/" NAME ".frag")
#define LOAD_MODEL(NAME) LoadModel("models/" NAME)

Model * m;

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

    // vertex buffer object, used for uploading the geometry
    GLuint vertexBufferObjID;
    GLuint normalBufferObjID;
    GLuint indexBufferObjID;
    GLuint bunnyTexCoordBufferObjID;

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");


    // Load model
    m = LOAD_MODEL("various/bunnyplus.obj");
    //CenterModel(m);

    // Load and compile shader
    program = LOAD_SHADERS("lab2-1");
    printError("init shader");

    glEnable(GL_DEPTH_TEST);

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);
    // Allocate Vertex Buffer Objects
    glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &normalBufferObjID);
    glGenBuffers(1, &indexBufferObjID);
    glGenBuffers(1, &bunnyTexCoordBufferObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 3 * m->numVertices * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

    printError("vertexArray");

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 3 * m->numVertices * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    GLuint normalLoc = glGetAttribLocation(program, "in_Normal"); 
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normalLoc);

    // VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

    // VBO vertex data
    if (m->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));
    }

    // End of upload of geometry

    printError("init arrays");
}

GLfloat t = 0;
GLfloat speed = 0.002;
float angleX = 0;
float angleY = 0;
void update(void)
{      
    GLfloat new_t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    GLfloat dt = new_t - t;
    t = new_t;

    angleX += speed;
    angleY += 2 * speed;
    mat4 rotX = Rx(angleX);
    mat4 rotY = Ry(angleY);
    mat4 rotMat = Mult(rotX, rotY);

    glUniform1f(glGetUniformLocation(program, "time"), 0.001 * t);
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, rotMat.m);
}

void display(void)
{
    printError("pre display");
    update();

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID);	// Select VAO
    //glDrawArrays(GL_TRIANGLES, 0, 3 * m->numVertices);	// draw object
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutCreateWindow ("GL3 white triangle example");
    glutDisplayFunc(display);
    init ();
    glutTimerFunc(20, &OnTimer, 0);
    glutMainLoop();
    return 0;
}
