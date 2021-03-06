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
#include "init_object.h"
#include "camera.h"

// 3.1) 
// 3.2) 
// 3.3)
// 3.4) Not complete

// Globals
// Data would normally be read from files

#define LOAD_SHADERS(NAME) loadShaders("shaders/" NAME ".vert",  "shaders/" NAME ".frag")
#define LOAD_MODEL(NAME) LoadModel("models/" NAME)
#define LOAD_TEXTURE(NAME, TEX) LoadTGATextureSimple("textures/" NAME ".tga", TEX)

struct TransModel;
struct TransModel {
    GLuint texObjID;
    Model * model;
    mat4 trans;
    struct TransModel * parent;
};
typedef struct TransModel TransModel;

void createModel(char * modelName, TransModel * transModel) {
    transModel->model = LoadModel(modelName);
    transModel->trans = IdentityMatrix();
    transModel->parent = NULL;
    transModel->texObjID = -1;
}

Camera camera;

#define near 1.0
#define far 180.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {
    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
    0.0f, 0.0f, -1.0f, 0.0f };

//#define NUM_OBJS 7
enum OBJS {
    BLADE1,
    BLADE2,
    BLADE3,
    BLADE4,
    WINDMILL,
    GROUND,
    SKYBOX,
    NUM_OBJS
};

// vertex array object
unsigned int vertexArrayObjID[NUM_OBJS];
// Reference to shader program
GLuint program;

TransModel objects[NUM_OBJS-1];

Model groundPlaneModel;
GLfloat vertexArray[] = {
    -far,-0.1f,-far,
    -far,-0.1f,far,
    far,-0.1f,far,
    far,-0.1f,-far
};
GLfloat normalArray[] = {
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0
};
GLuint indexArray[] = {
    0,1,2,
    0,2,3
};

GLfloat texCoordArray[] = {
    0,0,
    0,1,
    1,1,
    1,0   
};

Model * skybox;
GLuint skyboxTexObjID;
GLuint skyboxShader;
vec4 skyboxOffset;
mat4 skyboxCameraMatrix;

float mouseX, mouseY;
float moveZ, moveX;
void updateKey(unsigned char event, int up)
{ 
    switch(event)
    {
        case 'w': moveZ = up ? 0 : -1; break;
        case 'a': moveX = up ? 0 : -1; break; 
        case 's': moveZ = up ? 0 : 1; break;
        case 'd': moveX = up ? 0 : 1; break; 
    }
}
void keyboardCallback(unsigned char event, int x, int y)
{
    updateKey(event, 0);
}
void keyboardCallbackRelease(unsigned char event, int x, int y)
{
    updateKey(event, 1);
}
void mouseCallback(int mx, int my) {
    mouseX = mx;
    mouseY = my;
    int w,h;
    glutGetWindowSize(&w, &h);
    float wf = ((float)w) / 2.0f;
    float hf = ((float)h) / 2.0f;
    mouseX = ((float)mouseX - wf) / wf;
    mouseY = ((float)mouseY - hf) / hf;
}

float blade_x = 3.8f;
float blade_y = 9.0f;
float blade_z = 0.0f;
float blade_s = 0.8f;

void init(void)
{

    dumpInfo();

    camera = newCamera();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    printError("GL inits");

    createModel("models/windmill/blade.obj", &objects[BLADE1]);
    createModel("models/windmill/blade.obj", &objects[BLADE2]);
    createModel("models/windmill/blade.obj", &objects[BLADE3]);
    createModel("models/windmill/blade.obj", &objects[BLADE4]);
    createModel("models/windmill/windmill-walls.obj", &objects[WINDMILL]);

    groundPlaneModel.vertexArray = vertexArray;
    groundPlaneModel.normalArray = normalArray;
    groundPlaneModel.indexArray = indexArray;

    TransModel * groundPlane = &objects[GROUND];
    groundPlane->trans = IdentityMatrix();
    groundPlane->parent = NULL;
    groundPlane->model = &groundPlaneModel;
    groundPlane->model->numVertices = 4;
    groundPlane->model->numIndices = 6;
    groundPlane->model->texCoordArray = texCoordArray;
    LOAD_TEXTURE("grass",&(groundPlane->texObjID));

    skybox = LoadModel("models/skybox.obj");
    LOAD_TEXTURE("SkyBox512", &skyboxTexObjID);

    int i;
    // fix blades
    for(i = BLADE1; i <= BLADE4; ++i) {
        objects[i].trans = Mult(T(0,1,0),Rx(i * M_PI / 2));
        objects[i].parent = &objects[WINDMILL];
    }
    // fix body
    objects[WINDMILL].trans = IdentityMatrix();

    // Load and compile shader
    program = LOAD_SHADERS("lab3-3");
    skyboxShader = LOAD_SHADERS("skybox");
    printError("init shader");

    // Allocate and activate Vertex Array Object (VAO)
    glGenVertexArrays(NUM_OBJS, &vertexArrayObjID);

    // VBO for vertex data 
    glUseProgram(program);
    for(i = 0; i < SKYBOX; ++i) {
        init_object(vertexArrayObjID[i], objects[i].model, program);
    }
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

    // skyboxshader
    glUseProgram(skyboxShader);
    CenterModel(skybox);
    init_object(vertexArrayObjID[SKYBOX], skybox, skyboxShader);
    skyboxOffset.x = 0;
    skyboxOffset.y = 1.8;
    skyboxOffset.z = 0;
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projMatrix"), 1, GL_TRUE, projectionMatrix);

    // End of upload of geometry

    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardCallbackRelease);
    glutPassiveMotionFunc(mouseCallback);

    printError("init arrays");
}

GLfloat t = 0;
void updateCameraStuff() {
    GLfloat new_t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    GLfloat dt = new_t - t;
    t = new_t;

    cameraSetRotateVel(&camera, mouseY, mouseX);
    cameraSetMoveVel(&camera, moveX, 0, moveZ);
    updateCamera(&camera);

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, camera.matrix.m);

    glUseProgram(skyboxShader);
    skyboxCameraMatrix = camera.matrix;
    skyboxCameraMatrix.m[3] = skyboxOffset.x;
    skyboxCameraMatrix.m[4 + 3] = skyboxOffset.y;
    skyboxCameraMatrix.m[8 + 3] = skyboxOffset.z;
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "cameraMatrix"), 1, GL_TRUE, skyboxCameraMatrix.m);

}

void update(int modelNum)
{      
    mat4 transMat = objects[modelNum].trans;
    if( objects[modelNum].parent != NULL ) {
        transMat = Mult(objects[modelNum].parent->trans, transMat);
    }
    glUniform1f(glGetUniformLocation(program, "time"), 0.001 * t);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, transMat.m);
}

float blade_angle = 0;
float blade_speed = 0.01f;
void updateWindmill()
{
    blade_angle += blade_speed;
    int i;
    for(i = 0; i <= BLADE4; ++i) {
        objects[i].trans = Mult(T(blade_x,blade_y,blade_z),Mult(S(blade_s,blade_s,blade_s),Mult(Rx(blade_angle*2),Rx(i * M_PI / 2))));
    }
}

void display(void)
{
    printError("pre display");

    updateWindmill();

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    updateCameraStuff();

    glUseProgram(skyboxShader);
    glBindVertexArray(vertexArrayObjID[SKYBOX]); // Select VAO
    glBindTexture(GL_TEXTURE_2D, skyboxTexObjID);
    glUniform1i(glGetUniformLocation(skyboxShader, "texUnit"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glDrawElements(GL_TRIANGLES, skybox->numIndices, GL_UNSIGNED_INT, 0L);

    
    int i;
    if(1)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        for(i = 0; i < SKYBOX; ++i) {
            glBindVertexArray(vertexArrayObjID[i]);	// Select VAO
            update(i);
            //printf("texture[%d] = %d\n",i,objects[i].texObjID);
            /*
               if( i == 0 )
               glActiveTexture(GL_TEXTURE0);
               else
               glActiveTexture(GL_TEXTURE1);
               */
            // Bind texture
            //glBindTexture(GL_TEXTURE_2D, tex[i]);
            //glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            if( objects[i].texObjID != -1 ) {
                glBindTexture(GL_TEXTURE_2D, objects[i].texObjID);
                glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            }
            glDrawElements(GL_TRIANGLES, objects[i].model->numIndices, GL_UNSIGNED_INT, 0L);
        }
    }
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
