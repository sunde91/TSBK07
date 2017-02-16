#ifndef __CAM_MOT__
#define __CAM_MOT__

#include "VectorUtils3.h"

typedef struct 
{
    vec3 pos;
    vec3 dir;
    vec3 up;
    mat4 matrix;
} Camera;

Camera newCamera(void) 
{
    Camera camera;
    camera.pos = SetVector(0,5,30);
    camera.dir = SetVector(0,5,0);
    camera.up = SetVector(0,1,0);
    camera.matrix = lookAtv(camera.pos, camera.dir, camera.up);
    return camera;
}

void updateCamera(Camera * camera) 
{
    camera->matrix = lookAtv(camera->pos, camera->dir, camera->up);
}

void moveCamera(Camera * camera, GLfloat x, GLfloat y, GLfloat z)
{
    camera->pos = VectorAdd(camera->pos, SetVector(x,y,z));
}

void handleCameraEvent(Camera * camera, unsigned char event, int x, int y) 
{
    printf("event = %c\n",event);
    switch(event)
    {
        case 'w': moveCamera(camera, 0,0,-1); break;
        case 'a': moveCamera(camera, -1,0,0); break;
        case 's': moveCamera(camera, 0,0,1); break;
        case 'd': moveCamera(camera, 1,0,0); break;
    }
    updateCamera(camera);
}

#endif
