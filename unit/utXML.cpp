#include <vector>
#include "planet.h"
#include "xml.h"

float dt;

int main(){
  std::vector<Body> test;
  parseXML(&test,"./inputs/test.xml");

  std::cout << "UT\nTime Step: " << dt << "\n\n";
  for(int i = 0; i < test.size(); i++){
    test[i].printBody();
    std::cout << '\n';
  }
}

