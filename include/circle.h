#ifndef CIRCLE_H_
#define CIRCLE_H_

extern float cZoom;
extern float cOffX;
extern float cOffY;

typedef struct{
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;  
  float vertices[12*3];
  unsigned int indices[10*3];
} GL_Circle;

GL_Circle Circle_Struct_Init();
void Circle_Init(GL_Circle *);
void Circle_Draw(GL_Circle *, float, float, float, float);
void Circle_DeInit(GL_Circle *);

#endif

