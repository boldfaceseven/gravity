#include <iostream>
#include <cmath>
#include "planet.h"

const float G = 3.9728E-14;

void Body::propState(){
  rd  = rd + rdd * dt;
  r   = r  + rd  * dt;
  rdd.x = 0; rdd.y = 0;
}

void Body::gravity(Body *bTwo){
  vec2 F;
  F = (dir(bTwo->r, this->r)) * (G * this->mass * bTwo->mass / (pow(mag(bTwo->r - this->r),2)));
  this->rdd = this->rdd - F/this->mass;
  bTwo->rdd = bTwo->rdd + F/bTwo->mass;
}

void Body::compState(Body *bTwo){\
  vec2 p1, p2;
  if(mag(bTwo->r - this->r) <= bTwo->radius + this->radius){
    p1 = this->rd * this->mass;
    p2 = bTwo->rd * bTwo->mass;
    this->mass += bTwo->mass;
    this->rd = (p1 + p2) / this->mass;

    bTwo->mass = 0;
  }
}

bool Body::exist(){
  return this->mass != 0;
}

vec2 Body::readState(){
  return r;
}

float Body::readRad(){
  return radius;
}

/*
Body::Body(unsigned int, unsigned int){
  
}

Body::Body(unsigned int, unsigned int, float, float){
  
}
*/

Body::Body(){
  mass = 0;
  radius = 0;
  r.x = 0;
  r.y = 0;
  rd.x = 0;
  rd.y = 0;
  rdd.x = 0;
  rdd.y = 0;
}

void Body::cc(float c_mass, float c_radius, float c_r_x, float c_r_y, float c_rd_x, float c_rd_y){
  mass = c_mass;
  radius = c_radius;
  r.x = c_r_x;
  r.y = c_r_y;
  rd.x = c_rd_x;
  rd.y = c_rd_y;
  rdd.x = 0;
  rdd.y = 0;
}

Body::Body(float c_mass, float c_radius, float c_r_x, float c_r_y, float c_rd_x, float c_rd_y){
  mass = c_mass;
  radius = c_radius;
  r.x = c_r_x;
  r.y = c_r_y;
  rd.x = c_rd_x;
  rd.y = c_rd_y;
  rdd.x = 0;
  rdd.y = 0;
}

void fillVec(std::vector<Body> *vec ,float c_mass, float c_radius, float c_r_x, float c_r_y, float c_rd_x, float c_rd_y){
  Body temp;
  temp.cc(c_mass, c_radius, c_r_x, c_r_y, c_rd_x, c_rd_y);
  vec->push_back(temp);
}

float mag(const vec2 & a){
  return sqrt(pow(a.x,2) + pow(a.y,2));
}

vec2 dir(const vec2 & a, const vec2 & b){
  return (b - a) / mag(b - a);
}
