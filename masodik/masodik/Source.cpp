#include <array>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

int		window_width = 600;
int		window_height = 600;
char	window_title[] = "Bezier curve";

std::vector<glm::vec3> pointToDraw;

std::vector<glm::vec3> myControlPoints = {
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f,  0.5f, 0.0f),
};

#define numVBOs 1
#define numVAOs 1
GLuint VBO[numVBOs];
GLuint VAO[numVAOs];
GLboolean		isPoint;
GLboolean		isPolygon;

GLuint renderingProgram;

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

GLuint createShaderProgram()
{
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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;

	glViewport(0, 0, width, height);
}

int NCR(int n, int r) {
	if (r == 0) return 1;
	if (r > n / 2) return NCR(n, n - r);

	long res = 1;

	for (int k = 1; k <= r; ++k) {
		res *= n - k + 1;
		res /= k;
	}

	return res;
}

GLfloat blending(GLint n, GLint i, GLfloat t) {
	return NCR(n, i) * pow(t, i) * pow(1.0f - t, n - i);
}

void generateBezierCurve(std::vector<glm::vec3> controlPoints) {
	glm::vec3	nextPoint;
	GLfloat		t = 0.0f, B;
	GLfloat		increment = 1.0f / 100.0f;

	while (t <= 1.0f) {
		nextPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < controlPoints.size(); i++) {
			B = blending(controlPoints.size() - 1, i, t);
			nextPoint += B * controlPoints.at(i);
		}

		pointToDraw.push_back(nextPoint);
		t += increment;
	}
	pointToDraw.push_back(controlPoints.at(controlPoints.size() - 1));
	for (int i = 0; i < controlPoints.size(); i++)
	{
		pointToDraw.push_back(controlPoints.at(i));
	}
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	generateBezierCurve(myControlPoints);
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pointToDraw.size() * sizeof(glm::vec3), pointToDraw.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	isPoint = glGetUniformLocation(renderingProgram, "isPoint");
	isPolygon = glGetUniformLocation(renderingProgram, "isPolygon");
	glUseProgram(renderingProgram);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void cleanUpScene() {
	glDeleteVertexArrays(numVAOs, VAO);
	glDeleteBuffers(numVBOs, VBO);
	glDeleteProgram(renderingProgram);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO[0]);

	glProgramUniform1f(renderingProgram, isPoint, false);
	glProgramUniform1f(renderingProgram, isPolygon, false);
	glLineWidth(2.0f);
	glDrawArrays(GL_LINE_STRIP, 0, pointToDraw.size()-myControlPoints.size());

	glProgramUniform1f(renderingProgram, isPoint, true);
	glProgramUniform1f(renderingProgram, isPolygon, false);
	glPointSize(8.0f);
	glEnable(GL_POINT_SMOOTH);
	glDrawArrays(GL_POINTS, pointToDraw.size() - myControlPoints.size(), myControlPoints.size());

	glProgramUniform1f(renderingProgram, isPoint, false);
	glProgramUniform1f(renderingProgram, isPolygon, true);
	glDrawArrays(GL_LINE_LOOP, pointToDraw.size() - myControlPoints.size(), myControlPoints.size());


	glBindVertexArray(0);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);

	cleanUpScene();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}