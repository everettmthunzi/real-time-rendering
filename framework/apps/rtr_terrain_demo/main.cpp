#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <stdlib.h> 
#include <vector>

#include "rtr/terrain/Terrain.h"
#include "rtr/terrain/Skybox.h"
#include "rtr/shaders/SimpleShaders.h"
#include "rtr/shaders/TerrainShaders.h"
#include "rtr/shaders/SkyboxShaders.h"
#include "rtr/camera/Camera.h"

using namespace glm;
// Dimensions of the windows
const int width = 1280;
const int height = 720;

const int terrainResolution = 512;	// Size of the terrain: 64 x 64 grid
const int tileNumber = 12;			// No of tiles of terrain => 12 texture tiles
float amplitude = 80.0f;			// Amplitude for noise function
float frequency = 0.013f;			// frequency for noise function
const int terrainTextureRes = 512;	// Resolution of the texture images

const char* terrainFrag = "../resources/shaders/TerrainShader.frag";
const char* terrainVert = "../resources/shaders/TerrainShader.vert";
const char* skyboxFrag = "../resources/shaders/skybox.frag";
const char* skyboxVert = "../resources/shaders/skybox.vert";
std::vector<std::string> textures = { "../resources/textures/Terrain/Mountain1.png", "../resources/textures/Terrain/grassSeemless.png", "../resources/textures/Terrain/snow.png" };
std::vector<std::string> skyboxTextures = {"../resources/textures/Skybox/right.png", "../resources/textures/Skybox/left.png", "../resources/textures/Skybox/top.png", "../resources/textures/Skybox/bottom.png", "../resources/textures/Skybox/back.png", "../resources/textures/Skybox/front.png" };
const vec3 terrainCenter = vec3(static_cast<float>(terrainResolution / 2), 0.0f, static_cast<float>(terrainResolution / 2)); //center of terrain
const vec3 cameraPosition = vec3(0.0f, 80.0f, 0.0f);

Terrain* terrain = nullptr;
TerrainShaders* terrainShaders = nullptr;
Skybox* skybox = nullptr;
SkyboxShaders* skyboxShaders = nullptr;

float rotationXAngle = 0.0f;	// rotation angles for camera movement
float rotationYAngle = 0.0f;
Camera* camera;

// If mouse is moves in direction (x,y)
void mouseMotion(int x, int y)
{
	camera->mouseMove(x, y);
	glutPostRedisplay();
}

// if mouse button button is pressed
void mouse(int button, int state, int x, int y)
{
	camera->mouseButton(button, state, x, y);
	glutPostRedisplay();
}

// Transformation of terrain with angles for x and y axis
mat4 calcTerrainTransformation(float rotationXAngle, float rotationYAngle)
{
	mat4 transformMatrix = mat4(1.0f);
	transformMatrix = rotate(transformMatrix, rotationYAngle, vec3(0.0f, 1.0f, 0.0f));
	transformMatrix = rotate(transformMatrix, rotationXAngle, vec3(1.0f, 0.0f, 0.0f));
	transformMatrix = translate(transformMatrix, -terrainCenter);
	return transformMatrix;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update camera
	camera->update();

	mat4 worldMatrix = calcTerrainTransformation(rotationXAngle, rotationYAngle);

	// render terrain
	terrainShaders->setModelMatrix(worldMatrix * mat4(1.0));	
	terrainShaders->setViewMatrix(camera->getViewMatrix());
	terrainShaders->setProjectionMatrix(camera->getProjectionMatrix());
	terrainShaders->setAmplitude(amplitude);
	terrainShaders->setFrequency(frequency);
	terrainShaders->activate();
	terrain->draw();
	
	glDepthFunc(GL_LEQUAL);								// change depth function so depth test passes when values are equal to depth buffer's content
	mat4 viewMat = mat4(mat3(camera->getViewMatrix())); // remove translation from the view matrix
	skyboxShaders->setViewMatrix(viewMat);
	skyboxShaders->setProjectionMatrix(camera->getProjectionMatrix());
	skyboxShaders->activate();
	skybox->draw();
	glDepthFunc(GL_LESS);	 						// set depth function back to default
	glutSwapBuffers();
}

// Rotate terrain with arrow keys
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		rotationXAngle -= 3.0f;
		break;
	case GLUT_KEY_DOWN:
		rotationXAngle += 3.0f;
		break;
	case GLUT_KEY_LEFT:
		rotationYAngle -= 3.0f;
		break;
	case GLUT_KEY_RIGHT:
		rotationYAngle += 3.0f;
		break;
	}
	calcTerrainTransformation(rotationXAngle, rotationYAngle);
	glutPostRedisplay();
}

// change amplitude and frequency with asdf
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		amplitude += 2.0f;
		break;
	case 'a':
		amplitude -= 2.0f;
		break;
	case 'f':
		frequency += 0.02f;
		break;
	case 'd':
		frequency -= 0.02f;
		break;
	}
	glutPostRedisplay();

}


int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	if (glutCreateWindow("Illuminated Terrain") == 0)
	{
		printf("Glut init failed\n");
		return -1;
	}

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);

	// Init glew so that the GLSL functionality will be available
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW init failed!\n");
		return -1;
	}

	// Terrain  
	terrain = new Terrain(terrainResolution, tileNumber);
	terrainShaders = new TerrainShaders(terrainTextureRes, textures, amplitude, frequency);
	terrainShaders->loadVertexFragmentShaders(terrainVert, terrainFrag);
	terrainShaders->locateUniforms();

	//Skybox
	skybox = new Skybox(terrainResolution);
	skyboxShaders = new SkyboxShaders(terrainTextureRes, skyboxTextures);
	skyboxShaders->loadVertexFragmentShaders(skyboxVert, skyboxFrag);
	skyboxShaders->locateUniforms();
	
	// Camera 
	camera = new Camera((width/height), cameraPosition);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}
