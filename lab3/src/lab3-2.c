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

// 2.1) rotate around object

// Globals
// Data would normally be read from files

#define LOAD_SHADERS(NAME) loadShaders("shaders/" NAME ".vert",  "shaders/" NAME ".frag")
#define LOAD_MODEL(NAME) LoadModel("models/" NAME)
#define LOAD_TEXTURE(NAME, TEX) LoadTGATextureSimple("textures/" NAME ".tga", TEX)

struct TransModel;
struct TransModel {
    Model * model;
    mat4 trans;
    struct TransModel * parent;
};
typedef struct TransModel TransModel;

void createModel(char * modelName, TransModel * transModel) {
    transModel->model = LoadModel(modelName);
    transModel->trans = IdentityMatrix();
    transModel->parent = NULL;
}

#define NUM_OBJS 5

//mat4 transMat[NUM_OBJS];
//mat4 rotMat;
//mat4 modelMatrix[NUM_OBJS];
mat4 cameraMatrix;
//GLuint tex[NUM_OBJS];

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

// vertex array object
unsigned int vertexArrayObjID[NUM_OBJS];
// Reference to shader program
GLuint program;

TransModel objects[NUM_OBJS];

char active_key = 's';
float x,y,z,s,c;

float px, py;
void mouseCallback(int mx, int my) {
    int w,h;
    glutGetWindowSize(&w, &h);
    px = 2 * M_PI * mx / (float)w - M_PI;
    py =  M_PI / 2 - M_PI * my / (float)h;
}

void init(void)
{

    x = y = z = 0.0f;
    s = 1.0f;
    c = 10.0f;

    // vertex buffer object, used for uploading the geometry
    GLuint texBufferObjID;

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    createModel("models/windmill/blade.obj", &objects[0]);
    createModel("models/windmill/blade.obj", &objects[1]);
    createModel("models/windmill/blade.obj", &objects[2]);
    createModel("models/windmill/blade.obj", &objects[3]);
    createModel("models/windmill/windmill-walls.obj", &objects[4]);


    int i;
    // fix blades
    for(i = 0; i < 4; ++i) {
        objects[i].trans = Mult(T(0,1,0),Rx(i * M_PI / 2));
        objects[i].parent = &objects[4];
    }
    // fix body
    objects[4].trans = IdentityMatrix();
    cameraMatrix = lookAt(0,0,-10, 0,0,-1, 0,1,0); // TODO

    // Load model
    //models[0] = LOAD_MODEL("various/bunnyplus.obj");
    //models[1] = LOAD_MODEL("various/bunnyplus.obj");// LOAD_MODEL("various/cow.obj");
    // Load texture
    //LOAD_TEXTURE("rutor", &tex[0]);
    //LOAD_TEXTURE("dirt", &tex[1]);

    //printf("tex[0] = %d\n", tex[0]);
    //printf("tex[1] = %d\n", tex[1]);

    // Load and compile shader
    program = LOAD_SHADERS("lab3-2");
    printError("init shader");

    glEnable(GL_DEPTH_TEST);

    // Upload geometry to the GPU:

    // Allocate and activate Vertex Array Object (VAO)
    glGenVertexArrays(NUM_OBJS, &vertexArrayObjID);

    // VBO for vertex data

    for(i = 0; i < NUM_OBJS; ++i) {

        printf("on index = %d\n", i);
        glBindVertexArray(vertexArrayObjID[i]);

        GLuint vertexBufferObjID;
        GLuint normalBufferObjID;
        GLuint indexBufferObjID;
        GLuint texBufferObjID;
        // Allocate Vertex Buffer Objects (VBO)
        glGenBuffers(1, &vertexBufferObjID);
        glGenBuffers(1, &normalBufferObjID);
        glGenBuffers(1, &indexBufferObjID);
        glGenBuffers(1, &texBufferObjID);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i].model->numVertices * sizeof(GLfloat), objects[i].model->vertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
        glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, objects[i].trans.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

        printError("vertexArray");

        // VBO for index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, objects[i].model->numIndices * sizeof(GLuint), objects[i].model->indexArray, GL_STATIC_DRAW);

        printError("index");

           // VBO for normal data
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID); printError("0");
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i].model->numVertices * sizeof(GLfloat), objects[i].model->normalArray, GL_STATIC_DRAW);  printError("1");
        glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0); printError("2");
        glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal")); printError("3");

        printError("normal");
        // VBO vertex data
        /*
        if (objects[i].model->texCoordArray != NULL)
        {
            glBindBuffer(GL_ARRAY_BUFFER, texBufferObjID);
            glBufferData(GL_ARRAY_BUFFER, objects[i].model->numVertices*2*sizeof(GLfloat), objects[i].model->texCoordArray, GL_STATIC_DRAW);
            glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));
        }
        else {
            printf("no texture coordinates found\n");
        }
        
        printError("tex");
        */
    }
    // End of upload of geometry

    //glEnable(GL_TEXTURE_2D);

    glutPassiveMotionFunc(mouseCallback);

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
    //cameraMatrix = lookAt(c * sin(angleX),5, c * cos(angleX), 0,5,0, 0,1,0); // TODO
    float camX = c * cos(py) * cos(px);
    float camZ = c * cos(py) * sin(px);
    float camY  = c * sin(py);
    cameraMatrix = lookAt(camX, camY, camZ, 0,5,0, 0,1,0);
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
