#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "rapidXML/rapidxml.hpp"
#include "rapidXML/rapidxml_utils.hpp"

int main(){

  rapidxml::xml_document<> doc;
  std::ifstream file("~/scratch/cpp/GL/inputs/test.xml", std::ifstream::in);
  std::stringstream buffer;

  buffer << file.rdbuf();
  file.close();

  std::string content(buffer.str());

  doc.parse<0>(&content[0]);

  rapidxml::xml_node<> *pRoot = doc.first_node();

  std::cout << pRoot->name() << '\n';
}
