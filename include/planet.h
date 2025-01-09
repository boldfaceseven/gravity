#ifndef PLANET_H_
#define PLANET_H_
#include <cmath>
#include <vector>

extern const float G;
extern float dt;

class vec2{
 public:
  float x;
  float y;

  vec2 operator+(const vec2 & b) const{
    vec2 ret;
    ret.x = x + b.x;
    ret.y = y + b.y;
    return ret;
  }

  vec2 operator-(const vec2 & b) const{
    vec2 ret;
    ret.x = x - b.x;
    ret.y = y - b.y;
    return ret;
  }

  vec2 operator*(const float & b) const{
    vec2 ret;
    ret.x = x * b;
    ret.y = y * b;
    return ret;
  }

  vec2 operator/(const float & b) const{
    vec2 ret;
    ret.x = x / b;
    ret.y = y / b;
    return ret;
  }
};

class Body{
 private:
  float mass;
  float radius;
  vec2 r;
  vec2 rd;
  vec2 rdd;

  
 public:
  void propState();
  void compState(Body *);
  bool exist();
  void gravity(Body *);
  vec2 readState();
  float readRad();
  void printBody();
  Body();
  /*Body(unsigned int, unsigned int);
  Body(unsigned int, unsigned int, vec2);*/
  void cc(float, float, float, float, float, float);
  Body(float, float, float, float, float, float);
};

void fillVec(std::vector<Body> *vec ,float c_mass, float c_radius, float c_r_x, float c_r_y, float c_rd_x, float c_rd_y);
float mag(const vec2 &);
vec2 dir(const vec2 &, const vec2 &);

#endif
