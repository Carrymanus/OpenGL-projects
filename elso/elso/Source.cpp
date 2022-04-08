#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>


using namespace std;

std::vector<glm::vec3> circlePoints;
std::vector<glm::vec3> circleColors;

void generateCirclePoints(glm::vec2 o, GLfloat r, GLint n) {
	GLfloat x, y;
	GLfloat t = 0.0f;
	GLfloat full_circle = 2.0f * M_PI;
	GLfloat delta = full_circle / n;
	circlePoints.push_back(glm::vec3(0, 0, 0.0f));
	circleColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	for (int i = 0; i < n; i++)
	{
		x = o.x + r * cos(t);
		y = o.y + r * sin(t);

		circlePoints.push_back(glm::vec3(x, y, 0.0f));
		circleColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));


		t += delta;
	}
	circlePoints.push_back(glm::vec3(r * cos(0), r * sin(0), 0.0f));
	circleColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	circlePoints.push_back(glm::vec3(0.33f, 0.0f, 0.0f));
	circlePoints.push_back(glm::vec3(-0.33f, 0.0f, 0.0f));

}

#define		numVBOs			2
#define		numVAOs			2
GLuint		VBO[numVBOs];
GLuint		VAO[numVAOs];

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Elso beadando EY3455";
GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;
GLuint		renderingProgram;
GLuint		XoffsetLocation;
GLuint		YoffsetLocation;
GLuint		lineYoffsetLocation;
GLboolean		isLine;

float		x = 0.00f;
float		y = 0.00f;
float	    lineY = 0.00f;
float		incrementX = 0.01f;
float		incrementY = 0.01f;

bool		xDir = true;
bool		yDir = false;

bool checkOpenGLError() {
	bool	foundError = false;
	int		glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;

		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

void printShaderLog(GLuint shader) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);
		glGetShaderInfoLog(shader, length, &charsWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);
		glGetProgramInfoLog(prog, length, &charsWritten, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

string readShaderSource(const char* filePath) {
	ifstream	fileStream(filePath, ios::in);
	string		content;
	string		line;
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	string		vertShaderStr = readShaderSource("vertexShader.glsl");
	string		fragShaderStr = readShaderSource("fragmentShader.glsl");
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}
	GLuint		vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return vfProgram;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	generateCirclePoints(glm::vec2(-0.0f, -0.0f), 0.2f, 50);
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(glm::vec3), circlePoints.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, circleColors.size() * sizeof(glm::vec3), circleColors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);


	XoffsetLocation = glGetUniformLocation(renderingProgram, "offsetX");
	YoffsetLocation = glGetUniformLocation(renderingProgram, "offsetY");
	lineYoffsetLocation = glGetUniformLocation(renderingProgram, "lineoffsetY");
	isLine = glGetUniformLocation(renderingProgram, "isLine");
	glUseProgram(renderingProgram);
	glClearColor(1.0, 1.0, 0.0, 1.0);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	if (xDir) {
		x += incrementX;
		if (x > 0.8f) incrementX = -0.01f;
		if (x < -0.8f) incrementX = 0.01f;
		glProgramUniform1f(renderingProgram, XoffsetLocation, x);
	}
	if (yDir) {
		y += incrementY;
		if (y > 0.8f) incrementY = -0.01f;
		if (y < -0.8f) incrementY = 0.01f;
		glProgramUniform1f(renderingProgram, YoffsetLocation, y);
	}
	glProgramUniform1f(renderingProgram, isLine, false);
	glDrawArrays(GL_TRIANGLE_FAN, 0, circlePoints.size() - 2);
	glProgramUniform1f(renderingProgram, isLine, true);
	glLineWidth(3);
	glDrawArrays(GL_LINES, circlePoints.size() - 2, circlePoints.size());

}
void cleanUpScene() {
	glDeleteProgram(renderingProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();
	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		lineY += 0.1f;
		glProgramUniform1f(renderingProgram, lineYoffsetLocation, lineY);
		glProgramUniform1f(renderingProgram, isLine, true);
		glLineWidth(3);
		glDrawArrays(GL_LINES, circlePoints.size() - 2, circlePoints.size());
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		lineY -= 0.1f;
		glProgramUniform1f(renderingProgram, lineYoffsetLocation, lineY);
		glProgramUniform1f(renderingProgram, isLine, true);
		glLineWidth(3);
		glDrawArrays(GL_LINES, circlePoints.size() - 2, circlePoints.size());
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		xDir = true;
		yDir = true;
		incrementX = 0.05 + (((float)rand()) / (float)RAND_MAX) * -0.1;
		incrementY = 0.05 + (((float)rand()) / (float)RAND_MAX) * -0.1;
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
}

int main(void) {
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	glfwSwapInterval(1);
	glfwSetWindowSizeLimits(window, 400, 400, 800, 800);
	glfwSetWindowAspectRatio(window, 1, 1);
	init(window);
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cleanUpScene();
	return EXIT_SUCCESS;
}