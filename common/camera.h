#ifndef __CAM_MOT__
#define __CAM_MOT__

#include "VectorUtils3.h"

typedef struct 
{
    vec3 pos, vel;
    GLfloat speed;
    GLfloat pitch, yaw, roll;
    GLfloat pitchSpeed, yawSpeed;
    GLfloat rotSpeed;
    GLfloat rotThresh;
    //vec3 dir;
    //vec3 up;
    mat4 matrix;
} Camera;

Camera newCamera(void) 
{
    Camera camera;
    camera.pos = SetVector(0,5,30);
    camera.vel = SetVector(0,0,0);
    camera.speed = 1.0f;
    //camera.dir = SetVector(0,5,0);
    //camera.up = SetVector(0,1,0);
    //camera.matrix = lookAtv(camera.pos, camera.dir, camera.up);
    camera.pitch = 0;
    camera.yaw = 0;
    camera.rotSpeed = 0.05f;
    camera.rotThresh = 0.01f;
    camera.matrix = IdentityMatrix();
    //camera.rot = IdentityMatrix();
    return camera;
}


mat4 matFromAngles(GLfloat pitch, GLfloat yaw)
{
    mat4 rotx = Rx(pitch);
    mat4 roty = Ry(yaw);
    mat4 rot = Mult(rotx, roty);
    return rot; 
}

// Does not work?
vec3 vecFromAngles(GLfloat pitch, GLfloat yaw)
{
    //GLfloat z = -cos(pitch) * cos(yaw);
    //GLfloat x = -cos(pitch) * sin(yaw);
    //GLfloat y = -sin(pitch);
    mat4 R = matFromAngles(pitch, yaw);
    vec4 v0;
    v0.x = 0; v0.y = 0; v0.z = -1; v0.w = 1;
    vec4 v1 = MultVec4(R,v0);
    v1.x /= v1.w;
    v1.y /= v1.w;
    v1.z /= v1.w;
    return vec4tovec3(v1);
}

void cameraLookAt(Camera * cam, vec3 dir)
{
    vec3 at = ScalarMult(dir, 10000);
    cam->matrix = lookAtv(cam->pos, at, SetVector(0,1,0));
}

void updateCamera(Camera * camera) 
{
    //vec3 dir = vecFromAngles(camera->pitch, camera->yaw);
    //camera->matrix = lookAtv(camera->pos, dir, camera->up);
    camera->pos = VectorAdd(camera->pos, camera->vel);
    camera->pitch += camera->pitchSpeed;
    camera->yaw += camera->yawSpeed;
    mat4 R = matFromAngles(camera->pitch, camera->yaw);
    mat4 n = T( - camera->pos.x, - camera->pos.y, - camera->pos.z); // obs : negative
    camera->matrix = Mult(R, MatrixAdd(IdentityMatrix(), n));
    vec3 dir = vecFromAngles(camera->pitch, camera->yaw);
    //printf("dir = %f,%f,%f\n", -dir.x, -dir.y, -dir.z);
    //printf("pitch = %f, yaw = %f\n", camera->pitch, camera->yaw);
}

void cameraSetRotateVel(Camera * camera, GLfloat v_pitch, GLfloat v_yaw)
{
    v_pitch *= camera->rotSpeed;
    v_yaw *= camera->rotSpeed;
    if(v_pitch < 0 && camera->pitch <= (-M_PI/2+0.05)) 
        camera->pitchSpeed = 0;
    else if(v_pitch > 0 && camera->pitch >= (M_PI/2+0.05)) 
        camera->pitchSpeed = 0;
    else if( fabs(v_pitch) > camera->rotThresh )
        camera->pitchSpeed = v_pitch;
    else
        camera->pitchSpeed = 0;
    if( fabs(v_yaw) > camera->rotThresh )
        camera->yawSpeed = v_yaw;
    else
        camera->yawSpeed = 0;
    //camera->dir = vecFromAngles(camera->pitch, camera->yaw);
}

void cameraSetMoveVel(Camera * camera, GLfloat vx, GLfloat vy, GLfloat vz)
{
    vx *= camera->speed;
    vy *= camera->speed;
    vz *= camera->speed;
    vec4 moveVecLocal = vec3tovec4(SetVector(vx,vy,vz));
    mat4 baseMat = Transpose(matFromAngles(camera->pitch, camera->yaw));
    vec4 moveVecGlobal = MultVec4(baseMat, moveVecLocal);
    moveVecGlobal.y = 0; // inte flyga
    camera->vel = vec4tovec3(moveVecGlobal);
}

void cameraSetTargetY(Camera * camera, float targetY)
{
    float alpha = 0.1;
    camera->pos.y *= (1.0 - alpha);
    camera->pos.y += alpha * targetY;
}

#endif
