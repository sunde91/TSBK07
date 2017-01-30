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

// 1.1) glTexParameter (set repeat, clamp etc.), or change in vertex shader
// 1.2) special hardware

// Globals
// Data would normally be read from files

#define LOAD_SHADERS(NAME) loadShaders("shaders/" NAME ".vert",  "shaders/" NAME ".frag")
#define LOAD_MODEL(NAME) LoadModel("models/" NAME)
#define LOAD_TEXTURE(NAME, TEX) LoadTGATextureSimple("textures/" NAME ".tga", TEX)

Model * model;
GLuint * tex;

mat4 transMat;
mat4 rotMat;
mat4 modelMatrix;
mat4 cameraMatrix;

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {
2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
0.0f, 0.0f, -1.0f, 0.0f };

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
    model = LOAD_MODEL("various/bunnyplus.obj");
    
    // Load texture
    LOAD_TEXTURE("maskros512", &tex);

    // Load and compile shader
    program = LOAD_SHADERS("lab2-4");
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
    transMat = T(0,0,-2);
    rotMat = IdentityMatrix();
    modelMatrix = Mult(transMat, rotMat);
    cameraMatrix = lookAt(1,1,-7, 0,0,0, 0,1,0); // TODO

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 3 * model->numVertices * sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

    printError("vertexArray");

    // VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices * sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);

    printError("index");

       // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID); printError("0");
    glBufferData(GL_ARRAY_BUFFER, 3 * model->numVertices * sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);  printError("1");
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0); printError("2");
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal")); printError("3");

    printError("normal");
    
    // VBO vertex data
    if (model->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, model->numVertices*2*sizeof(GLfloat), model->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));
    }
    else {
        printf("no texture coordinates found\n");
    }
    
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    

    printError("tex");
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
    rotMat = Mult(rotX, rotY);

    modelMatrix = Mult(transMat, rotMat);

    glUniform1f(glGetUniformLocation(program, "time"), 0.001 * t);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix.m);
}

void display(void)
{
    printError("pre display");
    update();

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID);	// Select VAO
    //glDrawArrays(GL_TRIANGLES, 0, 3 * m->numVertices);	// draw object
    glDrawElements(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0L);

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
