// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "camera.h"

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	printf("texture width = (%d,%f), height =(%d,%f)\n",tex->width,tex->texWidth,tex->height,tex->texHeight);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 5.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

			if( x > 1 && z > 1 ) 
			{
				float vx1 = vertexArray[((x) + z * tex->width)*3 + 0];
				float vx2 = vertexArray[((x-1) + z * tex->width)*3 + 0];
				float vx3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 0];
				float vy1 = vertexArray[((x) + z * tex->width)*3 + 1];
				float vy2 = vertexArray[((x-1) + z * tex->width)*3 + 1];
				float vy3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 1];
				float vz1 = vertexArray[((x) + z * tex->width)*3 + 2];
				float vz2 = vertexArray[((x-1) + z * tex->width)*3 + 2];
				float vz3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 2];

				vec3 v1 = SetVector(vx1 - vx2, vy1 - vy2, vz1 - vz2);
				vec3 v2 = SetVector(vx3 - vx2, vy3 - vy2, vz3 - vz2);
				//vec3 v3 = SetVector(vx2 - vx3, vy2 - vy3, vz2 - vz3);
				vec3 normal = CrossProduct(v1,v2);
				normalArray[(x + z * tex->width)*3 + 0] = normal.x;
				normalArray[(x + z * tex->width)*3 + 1] = normal.y;
				normalArray[(x + z * tex->width)*3 + 2] = normal.z;
			}
			else
			{
				normalArray[(x + z * tex->width)*3 + 0] = 0.0;
				normalArray[(x + z * tex->width)*3 + 1] = 1.0;
				normalArray[(x + z * tex->width)*3 + 2] = 0.0;
			}
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width * 8.0; // x
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height *  8.0; // y
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	// Normal vectors
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain
float mouseX, mouseY;
float moveZ, moveX;
Camera camera;
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

void getTriangle(float posX, float posZ, int gridWidth, int * index0, int * index1, int * index2)
{
	int indX0 = (int)(posX + 0.5f);
	int indZ0 = (int)(posZ + 0.5f);
	int diffX = indX0 < posX ? 1 : -1;
	int diffZ = indZ0 < posZ ? 1 : -1;

	*index0 = indX0 + indZ0 * gridWidth;
	*index1 = (indX0+diffX) + indZ0 * gridWidth;
	*index2 = indX0 + (indZ0+diffZ) * gridWidth;
}

float calcDist(vec3 a, vec3 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;
	return sqrt( x*x + y*y + z*z);
}

float getHeightExact(Model * model, TextureData * heightMap, float posX, float posZ)
{
	int index0, index1, index2;
	getTriangle(posX, posZ, heightMap->width, &index0, &index1, &index2);

	float vx1 = model->vertexArray[index0*3 + 0];
	float vx2 = model->vertexArray[index1*3 + 0];
	float vx3 = model->vertexArray[index2*3 + 0];
	float vy1 = model->vertexArray[index0*3 + 1];
	float vy2 = model->vertexArray[index1*3 + 1];
	float vy3 = model->vertexArray[index2*3 + 1];
	float vz1 = model->vertexArray[index0*3 + 2];
	float vz2 = model->vertexArray[index1*3 + 2];
	float vz3 = model->vertexArray[index2*3 + 2];

	vec3 v0 = SetVector(posX, 0, posZ);
	vec3 v1 = SetVector(vx1,0,vz1);
	vec3 v2 = SetVector(vx2,0,vz2);
	vec3 v3 = SetVector(vx3,0,vz3);

	float d1 = calcDist(v0,v1);
	float d2 = calcDist(v0,v2);
	float d3 = calcDist(v0,v3);
	float sum = d1+d2+d3;
	d1 /= sum;
	d1 = 1.0 - d1;
	d2 /= sum;
	d1 = 1.0 - d1;
	d3 /= sum;
	d1 = 1.0 - d1;
	sum = d1+d2+d3;
	d1 /= sum;
	d2 /= sum;
	d3 /= sum;
	float y = d1 * vy1 + d2 * vy2 + d3 * vy3;
	return y;
}

float getHeight(TextureData * heightMap, float scaleFactor, float posX, float posZ) {
	if( posX < 0 || posZ < 0 )
		return 0;
	else if( posX > heightMap->width || posZ > heightMap->height )
		return 0;
	
	int indX = (int)posX;
	int indZ = (int)posZ;
	int ind = ((indX + heightMap->width * indZ) * heightMap->bpp) / 8;
	//printf("posX = %f, posZ = %f, indX = %d, indZ = %d, height = %d\n", posX, posZ, indX, indZ,  heightMap->imageData[ind]);
	return scaleFactor * heightMap->imageData[ ind ];
}

Model * createObject(char ** modelPath)
{
	Model * m = LoadModel(modelPath);
	
	Model* model = LoadDataToModel(
			m->vertexArray,
			m->normalArray,
			m->texCoordArray,
			NULL,
			m->indexArray,
			m->numVertices,
			m->numIndices);
	
	return model;
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

void init(void)
{
	// Load terrain data
	
	LoadTGATextureData("textures/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	// Init Camera
	camera = newCamera();
	camera.pos = SetVector(ttex.width/2,5,ttex.height/2);
	camera.yaw = M_PI / 4;
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1500.0);

	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("textures/maskros512.tga", &tex1);
	
	m2 = createObject("models/octagon.obj");
}

void updateCameraStuff() {

	cameraSetRotateVel(&camera, mouseY, mouseX);
	cameraSetMoveVel(&camera, moveX, 0, moveZ);
	//cameraSetTargetY(&camera, 5.0 + getHeight(&ttex,1.0 / 5.0, camera.pos.x, camera.pos.z));
	float currentHeight = getHeightExact(tm, &ttex, camera.pos.x, camera.pos.z);
	cameraSetTargetY(&camera, 5.0 + currentHeight);
	//printf("camera pos = %f,%f,%f\n",camera.pos.x,camera.pos.y, camera.pos.z);
	updateCamera(&camera);
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	updateCameraStuff();
	mat4 total, modelView, camMatrix;
	
	printError("pre display");
	
	glUseProgram(program);

	// Build matrix
	
	camMatrix = camera.matrix;
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 1");
	modelView = T(128,20,128);
	modelView = Mult(modelView, S(10,10,10));
	total = Mult(camMatrix, modelView); // Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(m2,program,"inPosition", "inNormal", "inTexCoord");

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

/*
void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardCallbackRelease);
	glutPassiveMotionFunc(mouseCallback);

	glutMainLoop();
	exit(0);
}
