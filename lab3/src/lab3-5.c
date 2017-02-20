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

// Defs 

#define LOAD_SHADERS(NAME) loadShaders("shaders/" NAME ".vert",  "shaders/" NAME ".frag")
#define LOAD_MODEL(NAME) LoadModel("models/" NAME)
#define LOAD_TEXTURE(NAME, TEX) LoadTGATextureSimple("textures/" NAME ".tga", TEX)

//#define SET_PARAM(PROGRAM, TYPE, NUM, PARAM) \
    glUniform ## TYPE ## (glGetUniformLocation(PROGRAM, #PARAM), NUM, GL_TRUE, PARAM);

struct TransModel;
struct TransModel {
    GLuint texObjID[2];
    Model * model;
    mat4 trans;
    struct TransModel * parent;
};
typedef struct TransModel TransModel;

void createModel(char * modelName, TransModel * transModel) {
    transModel->model = LoadModel(modelName);
    transModel->trans = IdentityMatrix();
    transModel->parent = NULL;
    (*transModel).texObjID[0] = -1;
    (*transModel).texObjID[1] = -1;
}


// Globals
#define NUM_LIGHTS 4
Point3D lightPosDir[NUM_LIGHTS] = { 
    {0,5,15},
    {15,5,0},
    {-15,5,0},
    //{-10,5,0},
    //{0.528247,0.803410,-0.274750}
    {-10,7,-6}
//    {0.593813,0.750454,0.290181}
//    {-0.765939,-0.471729,-0.436817}
};
Vector3f lightColor[NUM_LIGHTS] = {
    /*{0.5f, 0.8f, 1.0f},
    {1.0f, 0.8f, 0.5f},
    {0.8f, 1.0f, 0.5f},
    {0.8f, 0.5f, 1.0f},
    */
    {1.0f, 0.0f, 0.0f},
    {0.0f ,1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.8f}
};
GLint lightIsDir[NUM_LIGHTS] = {
    0,
    0,
    0,
   // 0,

    1
};
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
    -far,0,-far,
    -far,0,far,
    far,0,far,
    far,0,-far
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
GLuint skyboxTexObjID[2];
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

    // Load and compile shader
    program = LOAD_SHADERS("lab3-5");
    skyboxShader = LOAD_SHADERS("skybox2");
    printError("init shader");

    // Init lights
    glUseProgram(program);
    glUniform3fv(glGetUniformLocation(program, "lightPosDir"),
            NUM_LIGHTS, &(lightPosDir[0].x));
    glUniform3fv(glGetUniformLocation(program, "lightColor"),
            NUM_LIGHTS, &(lightColor[0].x));
    glUniform1iv(glGetUniformLocation(program, "lightIsDir"),
            NUM_LIGHTS, &lightIsDir);
    // Create models

    createModel("models/windmill/blade.obj", &objects[BLADE1]);
    createModel("models/windmill/blade.obj", &objects[BLADE2]);
    createModel("models/windmill/blade.obj", &objects[BLADE3]);
    createModel("models/windmill/blade.obj", &objects[BLADE4]);
    createModel("models/various/bunnyplus.obj", &objects[WINDMILL]);
    ScaleModel(objects[WINDMILL].model, 10, 10, 10);
    objects[WINDMILL].trans = T(0,5,0);
    LOAD_TEXTURE("SkyBox512", &objects[WINDMILL].texObjID[0]);

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
    LOAD_TEXTURE("dirt",&(groundPlane->texObjID[0]));
    LOAD_TEXTURE("grass",&(groundPlane->texObjID[1]));

    skybox = LoadModel("models/skybox.obj");
    LOAD_TEXTURE("SkyBox512", &skyboxTexObjID[0]);
    LOAD_TEXTURE("grass", &skyboxTexObjID[1]);

    int i;
    // fix blades
    for(i = BLADE1; i <= BLADE4; ++i) {
        objects[i].trans = Mult(T(0,1,0),Rx(i * M_PI / 2));
        objects[i].parent = &objects[WINDMILL];
    }
    // fix body
    //objects[WINDMILL].trans = IdentityMatrix();

    // Allocate and activate Vertex Array Object (VAO)
    glGenVertexArrays(NUM_OBJS, &vertexArrayObjID);

    // VBO for vertex data 
    glUseProgram(program);
    for(i = 0; i < SKYBOX; ++i) {
        init_object(vertexArrayObjID[i], objects[i].model, program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, objects[i].texObjID[0]);
        glUniform1i(glGetUniformLocation(program, "texUnit0"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, objects[i].texObjID[1]);
        glUniform1i(glGetUniformLocation(program, "texUnit1"), 1);
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTexObjID[0]);

    glUniform1i(glGetUniformLocation(skyboxShader, "texUnit0"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skyboxTexObjID[1]);

    glUniform1i(glGetUniformLocation(skyboxShader, "texUnit1"), 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // End of upload of geometry

    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardCallbackRelease);
    glutPassiveMotionFunc(mouseCallback);

    printError("init arrays");
}

GLfloat t = 0;
void updateWorld() 
{
    GLfloat new_t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    GLfloat dt = new_t - t;
    t = new_t;
    printError("updateWorld");
}

void updateCameraStuff() {

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

    //printError("updateCameraStuff");
}

void useModelTransform(int i)
{      
    glBindVertexArray(vertexArrayObjID[i]);	// Select VAO
    mat4 transMat = objects[i].trans;
    if( objects[i].parent != NULL )
        transMat = Mult(objects[i].parent->trans, transMat);

    mat4 modelViewMatrix = Mult(camera.matrix, transMat);
    mat3 normalMatrix = mat4tomat3(modelViewMatrix);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, transMat.m);
    //printError("1");
    glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"), 1, GL_TRUE, modelViewMatrix.m);
    //printError("2");
    glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_TRUE, normalMatrix.m);
    //printError("3");
    
    //printError("useModelTransform");
}

float blade_angle = 0;
float blade_speed = 0.001f;
void updateWindmill()
{
    blade_angle = t * blade_speed;
    int i;
    for(i = 0; i <= BLADE4; ++i) {
        objects[i].trans = Mult(T(blade_x,blade_y,blade_z),Mult(S(blade_s,blade_s,blade_s),Mult(Rx(blade_angle*2),Rx(i * M_PI / 2))));
    }
}

void display(void)
{
    printError("pre display");

    updateWorld();
    updateWindmill();
    updateCameraStuff();

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw skybox
    glUseProgram(skyboxShader);
    glBindVertexArray(vertexArrayObjID[SKYBOX]); // Select VAO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyboxTexObjID[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skyboxTexObjID[1]);
    glDrawElements(GL_TRIANGLES, skybox->numIndices, GL_UNSIGNED_INT, 0L);
    //glBindTexture(GL_TEXTURE_2D, skyboxTexObjID[1]);
    //glDrawElements(GL_TRIANGLES, skybox->numIndices, GL_UNSIGNED_INT, 0L);

    //glActiveTexture(GL_TEXTURE0);

    int i;
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    for(i = 0; i < SKYBOX; ++i)
    {
        useModelTransform(i);

        if( objects[i].texObjID[0] != -1 ) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, objects[i].texObjID[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, objects[i].texObjID[1]);
            //glUniform1i(glGetUniformLocation(program, "texUnit0"), 0);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        }
        glDrawElements(GL_TRIANGLES, objects[i].model->numIndices, GL_UNSIGNED_INT, 0L);
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
