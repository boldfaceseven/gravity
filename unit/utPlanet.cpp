#include <iostream>
#include <cstdio>
#include "planet.h"

float dt;

int main(){
  vec2 temp;
  dt = 1/(3600.0*24.0); //Seconds

  // Mass(Solar Masses), Rad(Au), Pos(x,y)(Au), Vel(x,y)(Au/s)
  Body b1(1,0.1,0,0,0,0), b2(3E-6,.01,1,1,0,0);

  for(int i = 0; i < 10; i++){
    temp = b1.readState();
    printf("%f %f\n", temp.x, temp.y);
    b1.gravity(&b2);
    b1.propState();
  }
}
