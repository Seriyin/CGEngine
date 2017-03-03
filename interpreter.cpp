#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "generator.h"
using namespace generator;

int main(int argc, char* argv[]){

  if(argc > 7){
    cerr << "Invalid number of arguments provided.";
    exit(EXIT_FAILURE);
  }

  else{
    ofstream fp;

    if(!strcmp(argv[1],"plane")){
      if((argc == 5)){
		// need to convert the arguments explicitly with stof() or the like !!
        float length = argv[3], width = argv[4];
        fp.open(argv[2], ios::trunc);
        generatePlane(fp, length, width);
        fp.close();
      }
      else
      {
	cerr << "Invalid number of arguments provided.";
        exit(EXIT_FAILURE);
      }
    }

    else if(!strcmp(argv[1],"box")){
      if((argc == 6)){
		 // need to convert the arguments explicitly with stof() or the like !!
        float length = argv[3], height = argv[4], width = argv[5];
        fp.open(argv[5], ios::trunc);
        generateBox(fp, length, height, width, 0);
        fp.close();
      }
      else if((argc == 7)){
		// need to convert the arguments explicitly with stof() or the like !!
        float length = argv[3], height = argv[4];
        float width = argv[5], divisions = argv[6];
        fp.open(argv[6], ios::trunc);
        generateBox(fp, length, height, width, divisions)
        fp.close();
      }
      else cerr << ("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }

    else if(!strcmp(argv[1],"sphere")){
      if((argc == 6)){
		// need to convert the arguments explicitly with stof() or the like !!
        float radius = argv[3], slices = argv[4], stacks = argv[5];
        fp.open(argv[5], ios::trunc);
        generateSphere(fp, radius, slices, stacks);
        fp.close();
      }
      else cerr << ("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }

    else if(!strcmp(argv[1],"cone")){
      if((argc == 7)){
		// need to convert the arguments explicitly with stof() or the like !!
        float radius = argv[3], height = argv[4];
        float slices = argv[5], stacks = argv[6];
        fp.open(argv[6], ios::trunc);
        generateCone(fp, radius, height, slices, stacks);
        fp.close();
      }
      else cerr << ("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }
    else cerr << ("Available graphical primitives: plane box sphere cone");
    exit(EXIT_FAILURE);
  }
  return 0;
}
