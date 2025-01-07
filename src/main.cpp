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

uint64_t timeMill();
float randP();
float randV(float,float,bool);

float dt;
float cZoom;
float cOffX;
float cOffY;

int main(){
  GLFWwindow *window;
  unsigned int shaderProgram;
  GL_Circle Circle = Circle_Struct_Init();
  int failed;
  
  std::vector<Body> p;
  Body vecTemp; //Camera Zoom centered on Origin
  const float pScale = 200;
  const unsigned int fRate = 60;
  int winW, winH;
  unsigned int n = 0;
  unsigned int simtim = 0;
  uint64_t comtim = 0;
  unsigned int prftim = 0;

  srand(timeMill());
  
  cZoom = 2.5;
  dt = 60.0;

  //Until I have a functioning XML or JSON parser, just #include the input files and compile them in here
  #include "solar.inp"
  //  #include "rand.inp"
  
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
    //  for(int i = 0; i < 1000000; i++){
    simtim = ++n*dt;

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
    
    if(timeMill()-comtim > 1000/fRate){
      comtim = timeMill();
      glfwGetWindowSize(window, &winW, &winH);

      //      printf("\r%d %d %d:%3d %2d:%2d:%2d",cOffX, cOffY, simtim/31536000,(simtim/86400)%365,(simtim/3600)%24,(simtim/60)%60 ,simtim%60);
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

uint64_t timeMill(){
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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

