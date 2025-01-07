#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "utilities.h"

void mWheelCallback(GLFWwindow *, double, double);

int Init(GLFWwindow **window){
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Orbit", NULL, NULL);
  if (*window == NULL){
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return 1;
    }
  glfwMakeContextCurrent(*window);
  glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

  glfwSetScrollCallback(*window, &mWheelCallback);
  
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout << "Failed to initialize GLAD" << std::endl;
      return 2;
    }

  return 0;
}

int BuildShader(unsigned int *shaderProgram){
  unsigned int vertexShader, fragmentShader;
  int success;
  char infoLog[512];

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";
  
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      return 3;
    }

  
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      return 3;
    }


  *shaderProgram = glCreateProgram();
  glAttachShader(*shaderProgram, vertexShader);
  glAttachShader(*shaderProgram, fragmentShader);
  glLinkProgram(*shaderProgram);
  glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return 4;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return 0;
}

void sScrollHandler(GLFWwindow *window, bool initPress){
  static double startCrsX, startCrsY;
  static double startCamX, startCamY;
  double X, Y;
  int winW, winH;
  float AR;
  glfwGetCursorPos(window, &X, &Y);
  glfwGetWindowSize(window, &winW, &winH);
  AR = (float)winH/(float)winW;
  
  if(initPress){
    startCrsX = X;
    startCrsY = Y;
    startCamX = cOffX;
    startCamY = cOffY;
  }

  //pixels -size-> % of screen -AR-> Au
  if(AR < 1){
    cOffX = ((X - startCrsX)/winW)*(cZoom/AR) + startCamX;// % of screen
    cOffY = (-(Y - startCrsY)/winH)*(cZoom) + startCamY;
  }
  else if(AR > 1){
    cOffX = ((X - startCrsX)/winW)*(cZoom) + startCamX;// % of screen
    cOffY = (-(Y - startCrsY)/winH)*(cZoom/AR) + startCamY;
  }
  else if(AR == 1){
    cOffX = ((X - startCrsX)/winW)*(cZoom) + startCamX;// % of screen
    cOffY = (-(Y - startCrsY)/winH)*(cZoom) + startCamY;
  }
}

void processInput(GLFWwindow *window){
  static int oldLMB;
  int temp;
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  
  temp = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
  if(temp == GLFW_PRESS)
    sScrollHandler(window, oldLMB != temp);
  oldLMB = temp;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mWheelCallback(GLFWwindow *window, double X, double Y){
  if(Y > 0){
    cZoom /= 1.05;
  }
  else if (Y < 0){
    cZoom *= 1.05;
  }
}
