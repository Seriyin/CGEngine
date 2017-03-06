#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include "generator.h"

int main(int argc, char* argv[]){

  if(argc > 7){
    cerr << "Invalid number of arguments provided.";
    exit(EXIT_FAILURE);
  }

  else {
    ofstream fp;

    if(!strcmp(argv[1],"plane")){
      if((argc == 5)){
        try{
          float length = stof(argv[2]), width = stof(argv[3]);
          fp.open(argv[4], ios::trunc);
          generatePlane(fp, length, width);
          fp.close();
        }
        catch(invalid_argument e){
          cerr << "Invalid type of arguments provided.";
          exit(EXIT_FAILURE);
        }
      }
      else {
	cerr << "Invalid number of arguments provided.";
        exit(EXIT_FAILURE);
      }
    }

    else if(!strcmp(argv[1],"box")){
		if((argc == 6)){
        try{
          float length = stof(argv[2]), height = stof(argv[3]);
          float width = stof(argv[4]);
          fp.open(argv[5], ios::trunc);
          generateBox(fp, length, height, width);
          fp.close();
        }
        catch(invalid_argument e){
          cerr << "Invalid type of arguments provided.";
          exit(EXIT_FAILURE);
        }
      }
      else if((argc == 7)){
        try{
          float length = stof(argv[2]), height = stof(argv[3]);
		  float width = stof(argv[4]);
		  int divisions = stoi(argv[5]);
          fp.open(argv[6], ios::trunc);
		  if (divisions==0)
		  {
			  generateBox(fp, length, height, width);
		  }
		  else 
		  {
			  generateBox(fp, length, height, width, divisions);
		  }
		  fp.close();
        }
        catch(invalid_argument e){
          cerr << "Invalid type of arguments provided.";
          exit(EXIT_FAILURE);
        }
      }
      else {
        cerr << ("Invalid number of arguments provided.");
        exit(EXIT_FAILURE);
      }
    }

    else if(!strcmp(argv[1],"sphere")){
      if((argc == 6)){
        try{
          float radius = stof(argv[2]), slices = stof(argv[3]);
          int stacks = stoi(argv[4]);
          fp.open(argv[5], ios::trunc);
          generateSphere(fp, radius, slices, stacks);
          fp.close();
        }
        catch(invalid_argument e){
          cerr << "Invalid type of arguments provided.";
          exit(EXIT_FAILURE);
        } 
      }
      else {
        cerr << ("Invalid number of arguments provided.");
        exit(EXIT_FAILURE);
      }
    }

    else if(!strcmp(argv[1],"cone")){
      if((argc == 7)){
	try{
          float radius = stof(argv[2]), height = stof(argv[3]);
          int slices = stoi(argv[4]), stacks = stoi(argv[5]);
          fp.open(argv[6], ios::trunc);
          generateCone(fp, radius, height, slices, stacks);
          fp.close();
        }
        catch(invalid_argument e){
          cerr << "Invalid type of arguments provided.";
          exit(EXIT_FAILURE);
        }
      }
      else {
        cerr << ("Invalid number of arguments provided.");
        exit(EXIT_FAILURE);
      }
    }
    
	else if (!strcmp(argv[1], "cylinder")) {
		if ((argc == 7)) {
			try {
				float radius = stof(argv[2]), height = stof(argv[3]);
				int slices = stoi(argv[4]), stacks = stoi(argv[5]);
				fp.open(argv[6], ios::trunc);
				generateCylinder(fp, radius, height, slices, stacks);
				fp.close();
			}
			catch (invalid_argument e) {
				cerr << "Invalid type of arguments provided.";
				exit(EXIT_FAILURE);
			}
		}
		else {
			cerr << ("Invalid number of arguments provided.");
			exit(EXIT_FAILURE);
		}
	}
    else {
      cerr << ("Available graphical primitives: plane box sphere cone");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
