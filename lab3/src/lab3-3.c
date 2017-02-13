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

// 3.1) 

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

mat4 cameraMatrix;

#define near 1.0
#define far 60.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {
2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
0.0f, 0.0f, -1.0f, 0.0f };

#define NUM_OBJS 6
// vertex array object
enum { 
    GROUND_PLANE,
    OBJ_END
};
unsigned int vertexArrayObjID[NUM_OBJS];
// Reference to shader program
GLuint program;

TransModel objects[NUM_OBJS];

char active_key = 's';
float x,y,z,s,c;

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
GLuint skyboxTexObjID;

void init(void)
{

    x = 3.8f;
    y = 9.0f;
    z = 0.0f;
    s = 0.8f;
    c = 30.0f;

    // vertex buffer object, used for uploading the geometry
    GLuint texBufferObjID;

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    createModel("models/skybox.obj", &objects[6]);
    skybox = LoadModel("models/skybox.obj");
    LOAD_TEXTURE("SkyBox512", &skyboxTexObjID);

    createModel("models/windmill/blade.obj", &objects[0]);
    createModel("models/windmill/blade.obj", &objects[1]);
    createModel("models/windmill/blade.obj", &objects[2]);
    createModel("models/windmill/blade.obj", &objects[3]);
    createModel("models/windmill/windmill-walls.obj", &objects[4]);
    
    groundPlaneModel.vertexArray = vertexArray;
    groundPlaneModel.normalArray = normalArray;
    groundPlaneModel.indexArray = indexArray;

    TransModel * groundPlane = &objects[5];
    groundPlane->trans = IdentityMatrix();
    groundPlane->parent = NULL;
    groundPlane->model = &groundPlaneModel;
    groundPlane->model->numVertices = 4;
    groundPlane->model->numIndices = 6;
    groundPlane->model->texCoordArray = texCoordArray;
    LOAD_TEXTURE("dirt",&(groundPlane->texObjID));


    int i;
    // fix blades
    for(i = 0; i < 4; ++i) {
        objects[i].trans = Mult(T(0,1,0),Rx(i * M_PI / 2));
        objects[i].parent = &objects[4];
    }
    // fix body
    objects[4].trans = IdentityMatrix();
    cameraMatrix = lookAt(0,0,-10, 0,0,-1, 0,1,0); // TODO

    // Load and compile shader
    program = LOAD_SHADERS("lab3-3");
    printError("init shader");

    glEnable(GL_DEPTH_TEST);

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object (VAO)
    glGenVertexArrays(NUM_OBJS, &vertexArrayObjID);

    // VBO for vertex data

    for(i = 0; i < NUM_OBJS; ++i) {
        printf("on index = %d\n", i);
        init_object(vertexArrayObjID[i], objects[i].model, program);
    }
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
    // End of upload of geometry

    //glEnable(GL_TEXTURE_2D);

    printError("init arrays");
}

GLfloat t = 0;
GLfloat speed = 0.002;
float angleX = 0;
float angleY = 0;
void update(int modelNum)
{      
    GLfloat new_t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    GLfloat dt = new_t - t;
    t = new_t;

    angleX += speed;
    angleY += 2 * speed;
    cameraMatrix = lookAt(c * sin(angleX),5, c * cos(angleX), 0,5,0, 0,1,0); // TODO
    glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

    mat4 transMat = objects[modelNum].trans;
    if( objects[modelNum].parent != NULL ) {
        transMat = Mult(objects[modelNum].parent->trans, transMat);
    }
    glUniform1f(glGetUniformLocation(program, "time"), 0.001 * t);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, transMat.m);
}

void processEvents(){
    if(glutKeyIsDown('x'))
        active_key =  'x';
    else if(glutKeyIsDown('y'))
        active_key =  'y';
    else if(glutKeyIsDown('z'))
        active_key =  'z';
    else if(glutKeyIsDown('s'))
        active_key =  's';
    else if(glutKeyIsDown('c'))
        active_key =  'c';
    else if(glutKeyIsDown('+'))
        switch(active_key){
            case 'x': x += 0.1;
                printf("x = %f\n", x);
                break;
            case 'y': y += 0.1;
                printf("y = %f\n", y);
                break;
            case 'z': z += 0.1;
                printf("z = %f\n", z);
                break;
            case 's': s += 0.025;
                printf("s = %f\n", s); 
                break;
            case 'c': c += 0.25;
                printf("c = %f\n", c); 
                break; 
        }
    else if(glutKeyIsDown('-'))
        switch(active_key){
            case 'x': x -= 0.1;
                printf("x = %f\n", x);
                break;
            case 'y': y -= 0.1;
                printf("y = %f\n", y);
                break;
            case 'z': z -= 0.1;
                printf("z = %f\n", z);
                break;
            case 's': s -= 0.025;
                printf("s = %f\n", s); 
                break;
            case 'c': c -= 0.25;
                printf("c = %f\n", c); 
                break; 
        }

}

void display(void)
{
    printError("pre display");

    processEvents();
    int i;
    for(i = 0; i < 4; ++i) {
        objects[i].trans = Mult(T(x,y,z),Mult(S(s,s,s),Mult(Rx(angleX*2),Rx(i * M_PI / 2))));
    }
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    for(i = 0; i < NUM_OBJS; ++i) {
        glBindVertexArray(vertexArrayObjID[i]);	// Select VAO
        update(i);
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
