#include <stdio.h>
#include <stdlib.h>
//#include "generator.c"

int main(int argc, char* argv[]){

  if(argc > 7){
    perror ("Invalid number of arguments provided.");
    exit(EXIT_FAILURE);
  }

  else{
    FILE* fp;

    if(!strcmp(argv[1],"plane")){
      if((argc == 3)){
        fp = fopen(argv[2], "w+");
        //generatePlane();
        //redirect the output to fp
        fclose(fp);
      }
      else perror("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }

    else if(!strcmp(argv[1],"box")){
      if((argc == 6)){
        fp = fopen(argv[5], "w+");
        //generateBox(argv[2], argv[3], argv[4]);
        //redirect the output to fp
        fclose(fp);
      }
      else if((argc == 7)){
        fp = fopen(argv[6], "w+");
        //generateBox(argv[2], argv[3], argv[4], argv[5]);
        //redirect the output to fp
        fclose(fp);
      }
      else perror("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }

    else if(!strcmp(argv[1],"sphere")){
      if((argc == 6)){
        fp = fopen(argv[5], "w+");
        //generateSphere(argv[2], argv[3], argv[4]);
        //redirect the output to fp
        fclose(fp);
      }
      else perror("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }

    else if(!strcmp(argv[1],"cone")){
      if((argc == 7)){
        fp = fopen(argv[6], "w+");
        //generateCone(argv[2], argv[3], argv[4], argv[5]);
        //redirect the output to fp
        fclose(fp);
      }
      else perror("Invalid number of arguments provided.");
      exit(EXIT_FAILURE);
    }
    else perror("Available graphical primitives: plane box sphere cone");
    exit(EXIT_FAILURE);
  }
  return 0;
}
