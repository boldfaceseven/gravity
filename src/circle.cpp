#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "circle.h"

GL_Circle Circle_Struct_Init(){
  GL_Circle c;
  float temp1[12*3] = {
    1.000f,  0.000f,  0.000f,
    0.866f,  0.500f,  0.000f,
    0.500f,  0.866f,  0.000f,
    0.000f,  1.000f,  0.000f,
    -0.500f,  0.866f,  0.000f,
    -0.866f,  0.500f,  0.000f,
    -1.000f,  0.000f,  0.000f,
    -0.866f, -0.500f,  0.000f,
    -0.500f, -0.866f,  0.000f,
    0.000f, -1.000f,  0.000f,
    0.500f, -0.866f,  0.000f,
    0.866f, -0.500f,  0.000f
  };
  float temp2[10*3] = {
    0,  1,  2,
    0,  2,  3,
    0,  3,  4,
    0,  4,  5,
    0,  5,  6,
    0,  6,  7,
    0,  7,  8,
    0,  8,  9,
    0,  9, 10,
    0, 10, 11
  };
  
  c.VAO = 0;
  c.VBO = 0;
  c.EBO = 0;
  for(int i = 0; i < (unsigned int)(sizeof(temp1)/sizeof(temp1[0])); i++){
    c.vertices[i] = temp1[i];
  }
  for(int i = 0; i < (unsigned int)(sizeof(temp2)/sizeof(temp2[0])); i++){
    c.indices[i] = temp2[i];
  }
  return c;
}

void Circle_Init(GL_Circle *C){
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  glGenVertexArrays(1, &C->VAO);
  glGenBuffers(1, &C->VBO);
  glGenBuffers(1, &C->EBO);

  glBindVertexArray(C->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, C->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(C->vertices), C->vertices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, C->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(C->indices), C->indices, GL_DYNAMIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

void fs_fs_fxn(float[], float[], float, float, float, float, int);

void Circle_Draw(GL_Circle *C, float x, float y, float r, float AR){
  float res[12*3];
  float xScale, yScale;
  glBindVertexArray(C->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, C->VBO);

  x += cOffX;
  y += cOffY;
  
  if(AR < 1){
    x /= cZoom/AR;
    y /= cZoom;
    xScale = r*AR;
    yScale = r;
  }
  else if(AR > 1){
    x /= cZoom;
    y /= cZoom*AR;
    xScale = r*AR;
    yScale = r;
  }
  else{
    x /= cZoom;
    y /= cZoom;
    xScale = r;
    yScale = r;
  }
  
  fs_fs_fxn(C->vertices,res,x,y,xScale,yScale,12);
  // Do matrix math on c->vertices
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(res), res, GL_DYNAMIC_DRAW);

  glDrawElements(GL_TRIANGLES, (unsigned int)(sizeof(C->indices)/sizeof(C->indices[0])), GL_UNSIGNED_INT, 0);
}

void fs_fs_fxn(float v[], float res[], float x, float y, float xs, float ys, int lenvr){

  float sm[4][4] = {
    {xs, 0,  0,  0},
    {0,  ys, 0,  0},
    {0,  0,  1,  0},
    {0,  0,  0,  1}};
  float tm[4][4] = {
    {1, 0, 0, x},
    {0, 1, 0, y},
    {0, 0, 1, 0},
    {0, 0, 0, 1}};
  
  float vm[4][lenvr];
  float temp[4][lenvr];
  int n = 0;
  for(int i = 0; i < lenvr; i++){
    for(int ii = 0; ii < 4; ii++){
      if(ii != 3){
	vm[ii][i] = v[n];
	n++;
      }
      else{
	vm[ii][i] = 1;
      }
    }
  }

  for(int i = 0; i < 4; i++){
    for(int ii = 0; ii < lenvr; ii++){
      temp[i][ii] = 0;
      for(int iii = 0; iii < 4; iii++){
	temp[i][ii] += sm[i][iii] * vm[iii][ii];
      }
    }
  }

  for(int i = 0; i < lenvr; i++){
    for(int ii = 0; ii < 4; ii++){
      vm[ii][i] = temp[ii][i];
    }
  }
  
  for(int i = 0; i < 4; i++){
    for(int ii = 0; ii < lenvr; ii++){
      temp[i][ii] = 0;
      for(int iii = 0; iii < 4; iii++){
	temp[i][ii] += tm[i][iii] * vm[iii][ii];
      }
    }
  }

  n = 0;
  for(int i = 0; i < lenvr; i++){
    for(int ii = 0; ii < 3; ii++){
      res[n] = temp[ii][i];
      n++;
    }
  }
}

void Circle_DeInit(GL_Circle *C){
  glDeleteVertexArrays(1, &C->VAO);
  glDeleteBuffers(1, &C->VBO);
  glDeleteBuffers(1, &C->EBO); 
}
