#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"
#include "Utils.h"
#include "Torus.h"
#include "ImportedModel.h"
using namespace std;

/*
	Contributor: 1809853Z-I011-0045 Wang Yuyang, Kennard
*/

#define numVAOs 1
#define numVBOs 12

// variable allocation for display
bool keys[1024];
int width, height;
float lightPos[3];
float rotAmt = 0.0f;
float aspect, rotLROffset = 0.0f, rotUDOffset = 0.0f, scaleOffset = 1.0472f;
float cameraX, cameraY, cameraZ;
float camMovX = 0.0f, camMovY = 0.0f, camMovZ = 0.0f;
float sphLocX, sphLocY, sphLocZ;
float torLocX, torLocY, torLocZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram, renderingProgramCubeMap;
GLuint skyboxTexture;  
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc, projLoc, nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat, tmpMat, rotMat;
glm::vec3 currentLightPos, transformed;

// object initialization
Sphere mySphere = Sphere(48);
Torus myTorus(0.5f, 0.2f, 48);
int numTorusVertices = myTorus.getNumVertices();
int numTorusIndices = myTorus.getNumIndices();

// store view matrix, used for translation 
stack<glm::mat4> mvStack;

// white light
float globalAmbient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// bright material
float matAmb[4] = { 0.9f, 0.9f, 0.9f, 0.5 };
float matDif[4] = { 0.75f, 0.6f, 0.23f, 1 };
float matSpe[4] = { 0.6f, 0.6f, 0.37f, 1 };
float matShi = 99999999.0f;  // large number means omitting the highlight

// planet texture
GLuint sunTexture;
GLuint earthTexture;
GLuint moonTexture;
GLuint mercuryTexture;
GLuint venusTexture;
GLuint marsTexture;
GLuint jupiterTexture;
GLuint saturnTexture;
GLuint uranusTexture;
GLuint neptuneTexture;
GLuint shuttleTexture;

ImportedModel myModel("spacecraft.obj");

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void setupVertices(void) {

	// set sphere
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	// set torus
	std::vector<int> ind2 = myTorus.getIndices();
	std::vector<glm::vec3> vert2 = myTorus.getVertices();
	std::vector<glm::vec2> tex2 = myTorus.getTexCoords();
	std::vector<glm::vec3> norm2 = myTorus.getNormals();

	std::vector<float> pvalues2;
	std::vector<float> tvalues2;
	std::vector<float> nvalues2;

	for (int i = 0; i < myTorus.getNumVertices(); i++) {
		pvalues2.push_back(vert2[i].x);
		pvalues2.push_back(vert2[i].y);
		pvalues2.push_back(vert2[i].z);
		tvalues2.push_back(tex2[i].s);
		tvalues2.push_back(tex2[i].t);
		nvalues2.push_back(norm2[i].x);
		nvalues2.push_back(norm2[i].y);
		nvalues2.push_back(norm2[i].z);
	}

	// set skybox
	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	float cubeTextureCoord[72] =
	{ 1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
	};

	// set model object
	std::vector<glm::vec3> vert3 = myModel.getVertices();
	std::vector<glm::vec2> tex3 = myModel.getTextureCoords();
	std::vector<glm::vec3> norm3 = myModel.getNormals();

	std::vector<float> pvalues3;
	std::vector<float> tvalues3;
	std::vector<float> nvalues3;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues3.push_back((vert3[i]).x);
		pvalues3.push_back((vert3[i]).y);
		pvalues3.push_back((vert3[i]).z);
		tvalues3.push_back((tex3[i]).s);
		tvalues3.push_back((tex3[i]).t);
		nvalues3.push_back((norm3[i]).x);
		nvalues3.push_back((norm3[i]).y);
		nvalues3.push_back((norm3[i]).z);
	}

	// bind vbos
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	// sphere
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    // skybox
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);

	// torus
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, pvalues2.size() * 4, &pvalues2[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, tvalues2.size() * 4, &tvalues2[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, nvalues2.size() * 4, &nvalues2[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind2.size() * 4, &ind2[0], GL_STATIC_DRAW);

	// model object
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, pvalues3.size() * 4, &pvalues3[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, tvalues3.size() * 4, &tvalues3[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, nvalues3.size() * 4, &nvalues3[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	
	// initialization
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingProgramCubeMap = Utils::createShaderProgram("vertShader_skybox.glsl", "fragShader_skybox.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 0.0f;
	sphLocX = 0.0f; sphLocY = 0.0f; sphLocZ = 0.0f;
	torLocX = 0.0f; torLocY = 0.0f; torLocZ = -1.0f;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	// load skybox
	skyboxTexture = Utils::loadTexture("texture/galaxy.jpg");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// load planet texture
	sunTexture = Utils::loadTexture("texture/sun.jpg");
	earthTexture = Utils::loadTexture("texture/earth.bmp");
	moonTexture = Utils::loadTexture("texture/moon.bmp");
	mercuryTexture = Utils::loadTexture("texture/mercury.bmp");
	venusTexture = Utils::loadTexture("texture/venus.bmp");
	marsTexture = Utils::loadTexture("texture/mars.bmp");
	jupiterTexture = Utils::loadTexture("texture/jupiter.bmp");
	saturnTexture = Utils::loadTexture("texture/saturn.bmp");
	uranusTexture = Utils::loadTexture("texture/uranus.bmp");
	neptuneTexture = Utils::loadTexture("texture/neptune.bmp");
	shuttleTexture = Utils::loadTexture("texture/spstob.jpg");
}

void installLights(glm::mat4 vMatrix) {
	
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

void set_skybox(double currentTime) {

	glUseProgram(renderingProgramCubeMap);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat *= rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(0.0, 1.0, 0.0)); // skybox rotation
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
	mvMat = vMat * mMat;

	mvLoc = glGetUniformLocation(renderingProgramCubeMap, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgramCubeMap, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
}

void set_planet(double currentTime) {

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(sphLocX, sphLocY, sphLocZ));
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ)); 
	rotMat = rotate(glm::mat4(1.0f), rotLROffset, glm::vec3(0.0, 1.0, 0.0));  // set rotation view on y axis
	vMat *= rotMat;
	rotMat = rotate(glm::mat4(1.0f), rotUDOffset, glm::vec3(1.0, 0.0, 0.0));  // set rotation view on x axis
	vMat *= rotMat;
	mvStack.push(vMat);

	// ----------------------  sphere == sun
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + camMovX, -5.0f + camMovY, -70.0f + camMovZ));    // camera view
	mvStack.push(mvStack.top());
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));      // size of the sun

	vMat = mvStack.top();
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();

	//-----------------------  sphere == mercury  
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 2.0f) * 4.0, 0.0f, cos((float)currentTime * 2.0f) * 4.0));   // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(0.0, 1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.38f, 0.38f, 0.38f));      // size of the mercury

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 2.0f) * 4.0, 0.0f, cos((float)currentTime * 2.0f) * 4.0))* rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mercuryTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == venus
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 1.43) * 7.2, 0.0f, cos((float)currentTime * 1.43) * 7.2));   // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 0.5f, glm::vec3(0.0, -1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.95f, 0.95f, 0.95f));      // size of the venus

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 1.43) * 7.2, 0.0f, cos((float)currentTime * 1.43) * 7.2)) * rotate(glm::mat4(1.0f), (float)currentTime * 0.5f, glm::vec3(0.0, -1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, venusTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == mars
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.7) * 15.2, 0.0f, cos((float)currentTime * 0.7) * 15.2)); // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), toRadians(25.2), glm::vec3(0.0, 0.0, 1.0));  // rotation axis
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 1.05f, glm::vec3(0.0, 1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.85f, 0.85f, 0.85f));      // size of the mars

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.7) * 15.2, 0.0f, cos((float)currentTime * 0.7) * 15.2)) * rotate(glm::mat4(1.0f), toRadians(25.2), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime * 1.05f, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, marsTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == jupiter
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.5) * 25, 0.0f, cos((float)currentTime * 0.5) * 25));  // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));      // size of the jupiter

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.5) * 25, 0.0f, cos((float)currentTime * 0.5) * 25))* rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));	
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, jupiterTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == saturn
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.4) * 39, 0.0f, cos((float)currentTime * 0.4) * 39));  // planet revolution
	tmpMat = mvStack.top();
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0));  // rotation axis
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f));      // size of the saturn

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.4) * 39, 0.0f, cos((float)currentTime * 0.4) * 39)) * rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, saturnTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	// saturn ring
	vMat = tmpMat;
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0f, 12.0f));
	vMat *= rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0));  // rotation axis
	vMat *= scale(glm::mat4(1.0f), glm::vec3(12.0f, 1.0f, 12.0f));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX, torLocY, torLocZ));

	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[8]);
	glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);

	//-----------------------  sphere == uranus
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.2) * 53, 0.0f, cos((float)currentTime * 0.2) * 53));  // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 6, glm::vec3(1.0, 0.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));      // size of the uranus

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.2) * 53, 0.0f, cos((float)currentTime * 0.2) * 53)) * rotate(glm::mat4(1.0f), (float)currentTime * 6, glm::vec3(1.0, 0.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, uranusTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == neptune
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.15) * 63, 0.0f, cos((float)currentTime * 0.15) * 63));  // planet revolution
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), toRadians(28.3), glm::vec3(0.0, 0.0, 1.0));  // rotation axis
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 6, glm::vec3(0.0, 1.0, 0.0));  // self rotation
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(1.7f, 1.7f, 1.7f));      // size of the neptune

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.15) * 63, 0.0f, cos((float)currentTime * 0.15) * 63)) * rotate(glm::mat4(1.0f), toRadians(28.3), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime * 6, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, neptuneTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();

	//-----------------------  sphere == earth  
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 10, 0.0f, cos((float)currentTime) * 10));   // planet revolve
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), toRadians(23.4), glm::vec3(0.0, 0.0, 1.0));  // rotation axis
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));  // self rotation

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 10, 0.0f, cos((float)currentTime) * 10)) * rotate(glm::mat4(1.0f), toRadians(23.4), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();

	//-----------------------  sphere == moon, do not need self rotation
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));  // planet revolve
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));  // size of the moon

	vMat = mvStack.top();
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);  // texture
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();  // the final pop is for the view matrix


	// model object
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.7f, -2.2f));  // initial view

	installLights(vMat);
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(-glm::inverse(mvMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shuttleTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());

}

void display(GLFWwindow* window, double currentTime) {

	// clear window
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// display planet and background
	set_skybox(currentTime);
	set_planet(currentTime);
}

void spacecraftMove(){

	GLfloat moveSpeed = 0.15f;
	
	if (keys[GLFW_KEY_LEFT])
		camMovX += moveSpeed;  // move left
	if (keys[GLFW_KEY_RIGHT])
		camMovX -= moveSpeed;  // move right
	if (keys[GLFW_KEY_UP])
		camMovY -= moveSpeed;  // move up
	if (keys[GLFW_KEY_DOWN])
		camMovY += moveSpeed;  // move down
	if (keys[GLFW_KEY_Z])
		camMovZ += moveSpeed;  // move forward
	if (keys[GLFW_KEY_C])
		camMovZ -= moveSpeed;  // move back
	if (keys[GLFW_KEY_W])
		rotUDOffset -= 0.005f;  // rotate up
	if (keys[GLFW_KEY_S])
		rotUDOffset += 0.005f;  // rotate down
	if (keys[GLFW_KEY_A])
		rotLROffset -= 0.005f;  // rotate left
	if (keys[GLFW_KEY_D])
		rotLROffset += 0.005f;  // rotate right
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	if (scaleOffset >= 0.5f && scaleOffset <= 1.2f)  // scroll scale
		scaleOffset -= yoffset * 0.02f;
	if (scaleOffset <= 0.5f)
		scaleOffset = 0.5f;
	if (scaleOffset >= 1.2f)
		scaleOffset = 1.2f;

	pMat = glm::perspective(scaleOffset, aspect, 0.1f, 1000.0f);  // modify project matrix
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);  // Q to quit
	if (action == GLFW_PRESS)  // while press
		keys[key] = true;
	else if (action == GLFW_RELEASE)  // while release
		keys[key] = false;
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "CS104 Project - Travel in the Solar System", NULL, NULL); // modify the window name
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	// call back event
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // optional : hide the cursor

	init(window);

	while (!glfwWindowShouldClose(window)) {	
		spacecraftMove();
		display(window, glfwGetTime());	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}