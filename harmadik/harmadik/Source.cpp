#include <array>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

extern void cleanUpScene();

GLFWwindow* window;

GLboolean		keyboard[512] = { GL_FALSE };

int				window_width = 600;
int				window_height = 600;
char			window_title[] = "Harmadik beadando EY3455";

unsigned int	modelLoc;
unsigned int	modelLocSphere;
unsigned int	viewLoc;
unsigned int	viewLocSphere;
unsigned int	projectionLoc;
unsigned int	projectionLocSphere;
unsigned int	invTMatrixLoc;
unsigned int	invTMatrixLocSphere;
unsigned int	lightPosLoc;

GLuint		lightXLoc;
GLuint		lightYLoc;
GLuint		lightZLoc;

std::vector<glm::vec3> verticesVec;
std::vector<glm::vec3> verticesSphere;

float vertices[] = {
	-0.2f, -0.4f, -0.2f,  0.0f,  0.0f, -1.0f,
	 0.2f, -0.4f, -0.2f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, -0.2f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, -0.2f,  0.0f,  0.0f, -1.0f,
	-0.2f,  0.4f, -0.2f,  0.0f,  0.0f, -1.0f,
	-0.2f, -0.4f, -0.2f,  0.0f,  0.0f, -1.0f,

	-0.2f, -0.4f,  0.2f,  0.0f,  0.0f, 1.0f,
	 0.2f, -0.4f,  0.2f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  0.2f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  0.2f,  0.0f,  0.0f, 1.0f,
	-0.2f,  0.4f,  0.2f,  0.0f,  0.0f, 1.0f,
	-0.2f, -0.4f,  0.2f,  0.0f,  0.0f, 1.0f,

	-0.2f,  0.4f,  0.2f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f, -0.2f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f, -0.2f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f, -0.2f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  0.2f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f,  0.2f,  -1.0f,  0.0f,  0.0f,

	 0.2f,  0.4f,  0.2f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f, -0.2f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f, -0.2f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f, -0.2f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  0.2f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f,  0.2f,  1.0f,  0.0f,  0.0f,

	-0.2f, -0.4f, -0.2f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f, -0.2f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  0.2f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  0.2f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f,  0.2f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f, -0.2f,  0.0f, -1.0f,  0.0f,

	-0.2f,  0.4f, -0.2f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f, -0.2f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  0.2f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  0.2f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f,  0.2f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f, -0.2f,  0.0f,  1.0f,  0.0f,








	-0.2f, -0.4f, 0.6f,  0.0f,  0.0f, -1.0f,
	 0.2f, -0.4f, 0.6f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, 0.6f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, 0.6f,  0.0f,  0.0f, -1.0f,
	-0.2f,  0.4f, 0.6f,  0.0f,  0.0f, -1.0f,
	-0.2f, -0.4f, 0.6f,  0.0f,  0.0f, -1.0f,

	-0.2f, -0.4f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.2f, -0.4f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  1.0f,  0.0f,  0.0f, 1.0f,
	-0.2f,  0.4f,  1.0f,  0.0f,  0.0f, 1.0f,
	-0.2f, -0.4f,  1.0f,  0.0f,  0.0f, 1.0f,

	-0.2f,  0.4f,  1.0f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f,  0.6f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  0.6f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  0.6f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  1.0f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f,  1.0f,  -1.0f,  0.0f,  0.0f,

	 0.2f,  0.4f,  1.0f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f,  0.6f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  0.6f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  0.6f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  1.0f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f,  1.0f,  1.0f,  0.0f,  0.0f,

	-0.2f, -0.4f,  0.6f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  0.6f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  1.0f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  1.0f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f,  1.0f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f,  0.6f,  0.0f, -1.0f,  0.0f,

	-0.2f,  0.4f,  0.6f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  0.6f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f,  0.6f,  0.0f,  1.0f,  0.0f,







	-0.2f, -0.4f, -1.0f,  0.0f,  0.0f, -1.0f,
	 0.2f, -0.4f, -1.0f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, -1.0f,  0.0f,  0.0f, -1.0f,
	 0.2f,  0.4f, -1.0f,  0.0f,  0.0f, -1.0f,
	-0.2f,  0.4f, -1.0f,  0.0f,  0.0f, -1.0f,
	-0.2f, -0.4f, -1.0f,  0.0f,  0.0f, -1.0f,

	-0.2f, -0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,
	 0.2f, -0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,
	 0.2f,  0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,
	-0.2f,  0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,
	-0.2f, -0.4f,  -0.6f,  0.0f,  0.0f, 1.0f,

	-0.2f,  0.4f,  -0.6f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f,  -1.0f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  -1.0f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  -1.0f,  -1.0f,  0.0f,  0.0f,
	-0.2f, -0.4f,  -0.6f,  -1.0f,  0.0f,  0.0f,
	-0.2f,  0.4f,  -0.6f,  -1.0f,  0.0f,  0.0f,

	 0.2f,  0.4f,  -0.6f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f,  -1.0f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  -1.0f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  -1.0f,  1.0f,  0.0f,  0.0f,
	 0.2f, -0.4f,  -0.6f,  1.0f,  0.0f,  0.0f,
	 0.2f,  0.4f,  -0.6f,  1.0f,  0.0f,  0.0f,

	-0.2f, -0.4f,  -1.0f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  -1.0f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  -0.6f,  0.0f, -1.0f,  0.0f,
	 0.2f, -0.4f,  -0.6f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f,  -0.6f,  0.0f, -1.0f,  0.0f,
	-0.2f, -0.4f,  -1.0f,  0.0f, -1.0f,  0.0f,

	-0.2f,  0.4f,  -1.0f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  -1.0f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  -0.6f,  0.0f,  1.0f,  0.0f,
	 0.2f,  0.4f,  -0.6f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f,  -0.6f,  0.0f,  1.0f,  0.0f,
	-0.2f,  0.4f,  -1.0f,  0.0f,  1.0f,  0.0f
};

#define numVBOs	1
#define numVAOs	1
GLuint			VBO[numVBOs];
GLuint			VAO[numVAOs];
GLuint			VBOSphere[numVBOs];
GLuint			VAOSphere[numVAOs];

GLuint		renderingProgram;
GLuint		renderingProgramSphere;
GLfloat     radian = 9.0f;


glm::mat4	model, view, projection = glm::perspective(glm::radians(55.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
glm::mat4	modelSphere = model;
glm::mat4	viewSphere = view;
glm::mat4	projectionSphere = projection;
glm::mat4	invTmatrix, rotateM, scaleM;
glm::mat4	invTmatrixSphere,rotateMSphere,scaleMSphere;

GLdouble	currentTime, deltaTime, lastTime = 0.0f;
GLfloat		cameraSpeed;
GLfloat     cameraLookAngle;

glm::vec3	cameraPos = glm::vec3(radian, 0.0f, 0.0f),
cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f),
cameraUpVector = glm::vec3(0.0f, 0.0f, 1.0f),
cameraMovingX = glm::vec3(-1.0f, 0.0f, 0.0f),
cameraMovingY = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3	lightPos = glm::vec3(2.0*radian,0.0f,0.0f);
GLfloat     lightLookAngle;

void generateSphere(double r, int lats, int longs) {
	int i, j;
	verticesSphere.clear();
	for (i = 0; i <= lats; i++) {
		double lat0 = 3.14 * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = 3.14 * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * 3.14 * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			verticesSphere.push_back(glm::vec3(x * zr0, y * zr0, z0));
			verticesSphere.push_back(glm::vec3(r * x * zr0, r * y * zr0, r * z0));
			verticesSphere.push_back(glm::vec3(x * zr1, y * zr1, z1));
			verticesSphere.push_back(glm::vec3(r * x * zr1, r * y * zr1, r * z1));
		}
	}
}

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram() {

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	string vertShaderStr = readShaderSource("vertexShader.glsl");
	string fragShaderStr = readShaderSource("fragmentShader.glsl");

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(vfProgram);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return vfProgram;
}

GLuint createShaderProgramSphere() {

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	string vertShaderStr = readShaderSource("vertexShaderSphere.glsl");
	string fragShaderStr = readShaderSource("fragmentShaderSphere.glsl");

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(vfProgram);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return vfProgram;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();

	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
}

void computeModelMatrix() {
	scaleM = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
	model = scaleM;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void computeModelMatrixSphere() {
	scaleMSphere = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	modelSphere = scaleMSphere;
	glUniformMatrix4fv(modelLocSphere, 1, GL_FALSE, glm::value_ptr(modelSphere));
}

void computeCameraMatrix() {
	view = glm::lookAt(cameraPos, cameraTarget, cameraUpVector);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void computeCameraMatrixSphere() {
	viewSphere = glm::lookAt(cameraPos, cameraTarget, cameraUpVector);
	glUniformMatrix4fv(viewLocSphere, 1, GL_FALSE, glm::value_ptr(viewSphere));
}

void generateVertices() {
	verticesVec.clear();
	for (int i = 0; i <= sizeof(vertices) / 3; i += 3)
	{
		verticesVec.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
	}
}


void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	generateVertices();
	glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(glm::vec3), verticesVec.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(renderingProgram);
	modelLoc = glGetUniformLocation(renderingProgram, "model");
	viewLoc = glGetUniformLocation(renderingProgram, "view");
	projectionLoc = glGetUniformLocation(renderingProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	invTMatrixLoc = glGetUniformLocation(renderingProgram, "invTMatrix");
	lightPosLoc = glGetUniformLocation(renderingProgram, "lightPos");

	renderingProgramSphere = createShaderProgramSphere();
	glGenBuffers(numVBOs, VBOSphere);
	glGenVertexArrays(numVAOs, VAOSphere);
	glBindBuffer(GL_ARRAY_BUFFER, VBOSphere[0]);
	generateSphere(0.5,180,180);
	glBufferData(GL_ARRAY_BUFFER, verticesSphere.size() * sizeof(glm::vec3), verticesSphere.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAOSphere[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(renderingProgramSphere);
	lightXLoc = glGetUniformLocation(renderingProgramSphere, "lightX");
	lightYLoc = glGetUniformLocation(renderingProgramSphere, "lightY");
	lightZLoc = glGetUniformLocation(renderingProgramSphere, "lightZ");
	modelLocSphere = glGetUniformLocation(renderingProgramSphere, "modelSphere");
	viewLocSphere = glGetUniformLocation(renderingProgramSphere, "viewSphere");
	projectionLocSphere = glGetUniformLocation(renderingProgramSphere, "projectionSphere");
	glUniformMatrix4fv(projectionLocSphere, 1, GL_FALSE, glm::value_ptr(projectionSphere));
	invTMatrixLocSphere = glGetUniformLocation(renderingProgramSphere, "invTMatrixSphere");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
}

void cleanUpScene() {
	glfwDestroyWindow(window);
	verticesVec.clear();
	verticesSphere.clear();
	glDeleteBuffers(numVBOs, VBO);
	glDeleteBuffers(numVBOs, VBOSphere);
	glDeleteProgram(renderingProgram);
	glDeleteProgram(renderingProgramSphere);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	cameraSpeed = 5.0f * (GLfloat)deltaTime;
	if (keyboard[GLFW_KEY_UP]) {
		cameraPos.z += 0.1f;
	}

	if (keyboard[GLFW_KEY_DOWN]) {
		cameraPos.z -= 0.1f;
	}

	if (keyboard[GLFW_KEY_LEFT]) {
		cameraLookAngle -= cameraSpeed;
		cameraPos.x = radian * cos(cameraLookAngle);
		cameraPos.y = radian * sin(cameraLookAngle);
	}

	if (keyboard[GLFW_KEY_RIGHT]) {
		cameraLookAngle += cameraSpeed;
		cameraPos.x = radian * cos(cameraLookAngle);
		cameraPos.y = radian * sin(cameraLookAngle);
	}

	lightLookAngle += cameraSpeed/2;
	lightPos.x = 2.0 * radian * cos(lightLookAngle);
	lightPos.y = 2.0 * radian * sin(lightLookAngle);
	glUseProgram(renderingProgram);

	computeModelMatrix();
	computeCameraMatrix();

	invTmatrix = glm::inverseTranspose(view * model);
	glUniformMatrix4fv(invTMatrixLoc, 1, GL_FALSE, glm::value_ptr(invTmatrix));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, verticesVec.size()-1);
	glBindVertexArray(0);


	glUseProgram(renderingProgramSphere);
	computeModelMatrixSphere();
	computeCameraMatrixSphere();

	glProgramUniform1f(renderingProgramSphere, lightXLoc, lightPos.x);
	glProgramUniform1f(renderingProgramSphere, lightYLoc, lightPos.y);
	glProgramUniform1f(renderingProgramSphere, lightZLoc, lightPos.z);

	invTmatrixSphere = glm::inverseTranspose(viewSphere * modelSphere);
	glUniformMatrix4fv(invTMatrixLocSphere, 1, GL_FALSE, glm::value_ptr(invTmatrixSphere));
	glBindVertexArray(VAOSphere[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, verticesSphere.size());
	glBindVertexArray(0);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;

	glViewport(0, 0, width, height);

	projection = glm::perspective(glm::radians(55.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanUpScene();

	return EXIT_SUCCESS;
}