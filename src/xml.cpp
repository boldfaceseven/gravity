#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include "rapidXML/rapidxml.hpp"
#include "planet.h"
#include "xml.h"

std::string convChar(char[]);
float randAB(float, float);
void parseBody(std::vector<Body>*, rapidxml::xml_node<>*);
extern uint64_t timeMill();
extern float pScale;

void parseXML(std::vector<Body> *sys, std::string filename){  
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> *root; // Root node
  rapidxml::xml_node<> *node; // Node currently being processed
  std::string nodeStr; // Hold current node value being processed
  std::ifstream file;
  
  // Read the xml file, parse the tree, and set the root node
  file.open(filename.c_str());
  std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  for(int i = 0; i < buffer.size(); i++){buffer[i] = tolower(buffer[i]);} // Convert all file contents to lowercase
  buffer.push_back('\0');
  doc.parse<0>(&buffer[0]);
  root = doc.first_node("masterconfig");
  
  // Find the RNG seed option, if available
  if(node = root->first_node("rng")){
    nodeStr = convChar(node->first_attribute("seed")->value());
    if(nodeStr == "null"){
      srand(timeMill());
    }
    else{
      srand(std::stoi(nodeStr));
    }
  }
  else{
    srand(timeMill());
  }
  
  // Find the time step option, if available
  if(node = root->first_node("timestep")){
    nodeStr = convChar(node->first_attribute("value")->value());
    dt = std::stof(nodeStr);
  }
  else{
    dt = 60.0;
  }
  
  // Find the size multiplier option, if available
  if(node = root->first_node("scale")){
    nodeStr = convChar(node->first_attribute("value")->value());
    pScale = std::stoi(nodeStr);
  }
  else{
    pScale = 200;
  }
  
  // Search through the first gen nodes to populate the body list.
  for(node = root->first_node("body"); node; node = node->next_sibling()){
    nodeStr = convChar(node->first_attribute("instance")->value());
    if(nodeStr.find('-') != std::string::npos){
      for(int i = std::stoi(nodeStr.substr(0,nodeStr.find('-'))); i <= std::stoi(nodeStr.substr(nodeStr.find('-')+1)); i++){
	parseBody(sys, node);
      }      
    }
    else{
      parseBody(sys, node);
    }
  }
}

void parseBody(std::vector<Body> *sys, rapidxml::xml_node<> *node){
  float tempF1;
  float tempF2;
  std::string tempStr;
  float out[6];
  
  tempStr = convChar(node->first_node("mass")->first_attribute("value")->value());
  if(tempStr == "rand"){
    out[0] = randAB(atof(node->first_node("mass")->first_attribute("min")->value()), atof(node->first_node("mass")->first_attribute("max")->value()));
  }
  else{
    out[0] = std::stof(tempStr);
  }

  tempStr = convChar(node->first_node("radius")->first_attribute("value")->value());
  if(tempStr == "rand"){
    out[1] = randAB(atof(node->first_node("radius")->first_attribute("min")->value()), atof(node->first_node("radius")->first_attribute("max")->value()));
  }
  else{
    out[1] = std::stof(tempStr);
  }

  tempStr = convChar(node->first_node("ipos")->first_attribute("value")->value());
  if(tempStr == "rand"){
    tempF1 = randAB(atof(node->first_node("ipos")->first_attribute("min")->value()), atof(node->first_node("ipos")->first_attribute("max")->value()));
    tempF2 = randAB(0,359);
    out[2] = tempF1*cos(tempF2);
    out[3] = tempF1*sin(tempF2);
  }
  else{
    sscanf(tempStr.c_str(), "(%f,%f)", &out[2], &out[3]);
  }

  tempStr = convChar(node->first_node("ivel")->first_attribute("value")->value());
  if(tempStr == "rand"){
    tempF1 = randAB(atof(node->first_node("ivel")->first_attribute("min")->value()), atof(node->first_node("ivel")->first_attribute("max")->value()));
    tempF2 = randAB(0,359);
    out[4] = tempF1*cos(tempF2);
    out[5] = tempF1*sin(tempF2);
  }
  else{
    sscanf(tempStr.c_str(), "(%f,%f)", &out[4], &out[5]);
  }
  fillVec(sys,out[0],out[1],out[2],out[3],out[4],out[5]);
}

std::string convChar(char str[]){
  std::string a(str);
  return a;
}

float randAB(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
