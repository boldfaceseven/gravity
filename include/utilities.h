#ifndef UTILITIES_H_
#define UTILITIES_H_

extern float cZoom;
extern float cOffX;
extern float cOffY;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int Init(GLFWwindow **window);
int BuildShader(unsigned int *shader);

#endif
