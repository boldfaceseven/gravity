#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "utilities.h"
#include "circle.h"
#include "planet.h"
#include "xml.h"

float randP();
float randV(float,float,bool);
uint64_t timeMill();

float dt;
float pScale;
float cZoom;
float cOffX;
float cOffY;

int main(int argc, char** argv){
  GLFWwindow *window;
  unsigned int shaderProgram;
  GL_Circle Circle = Circle_Struct_Init();
  int failed;
  
  std::vector<Body> p;
  Body vecTemp; //Camera Zoom centered on Origin
  const unsigned int fRate = 30;
  int winW, winH;
  unsigned int n = 0;
  uint64_t propTime = 0;
  uint64_t frameTime = 0;
  unsigned int prftim = 0;
  
  cZoom = 2.5;
  parseXML(&p, argv[1]);

  failed = Init(&window);
  if(failed)
    return failed;

  failed = BuildShader(&shaderProgram);
  if(failed)
    return failed;

  Circle_Init(&Circle);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)){
    // Run Physics 10 times a frame
    if(timeMill()-propTime > 100/fRate){
      propTime = timeMill();
      for(int i = 0; i < p.size(); i++){
	if(p[i].exist()){ 
	  for(int ii = i+1; ii < p.size(); ii++){
	    if(p[ii].exist()){
	      p[i].gravity(&p[ii]);
	    }
	  }
	  p[i].propState();
	}
      }

      for(int i = 0; i < p.size(); i++){
	if(p[i].exist()){
	  for(int ii = i+1; ii < p.size(); ii++){
	    if(p[ii].exist()){
	      p[i].compState(&p[ii]);
	    }
	  }
	}
      }
    }

    // Draw a frame every 1/fRate seconds 
    if(timeMill()-frameTime > 1000/fRate){
      frameTime = timeMill();
      glfwGetWindowSize(window, &winW, &winH);

      fflush(stdout);

      processInput(window);
      
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shaderProgram);

      for(int i = 0; i < p.size(); i++){
	Circle_Draw(&Circle, p[i].readState().x, p[i].readState().y, p[i].readRad()*pScale/cZoom, (float)winH/(float)winW);
      }
    
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  Circle_DeInit(&Circle);
  glfwTerminate();
  return 0;
}

float randP(){
  return (rand()%1000)/1000.0;
}

float randV(float minV, float maxV, bool rNeg){
  if(rNeg){
    return pow(-1,(rand()%2)) * (randP()*(maxV-minV))+minV;
  }
  else{
    return (randP()*(maxV-minV))+minV;
  }
}

uint64_t timeMill(){
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
