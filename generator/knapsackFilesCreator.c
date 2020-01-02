/* ======================================================================
   ====================================================================== */
#include <stdlib.h>
#include <stdio.h>




//=========================================================== main
//===========================================================
int main(int argc, char *argv[]){
  if (argc!=3) {
    printf("==================================\n");
    printf("Please enter the correct syntax\n");
    printf("knapsackFilescreator filename1 filename2\n");
    printf("filename1  is the generator filename\n");
    printf("filename2  is the generator output filename\n");
    printf("==================================\n");
    exit(0);
  }
  int n[]={10,50,100,500};
  int r[]={50,100,500,1000};
  int type[]={1,2,3,4} ;

  char filename[50];
  char generatorFilename[50];
  char generatorOutputFilename[50];

  sprintf(generatorFilename,argv[1]);
  sprintf(generatorOutputFilename,argv[2]);


  printf("generatorFilename: %s\n",generatorFilename);
  printf("generatorOutputFilename: %s\n",generatorOutputFilename);

  printf("Press enter to start\n");
  char entr[20];


  printf("==================================\n");
  printf("=========   Generator ============\n");
  printf("==================================\n");


  int fileCount=0;
  for (int k1=0;k1<4;k1++) {
    for (int k2=0;k2<4;k2++) {
        for (int k3=0;k3<4;k3++) {
            for (int k4=1;k4<=5;k4++) {
               int n_=n[k1];
               int r_=r[k2];
               int type_=type[k3];
               int i=k4;
               int S=5;

               sprintf(filename, "problem_%d_%d_%d_%d_%d.txt", n_,r_,type_,i,S);
               fileCount++;

               char filenameWithArguments[50];
               sprintf(filenameWithArguments,"%s %d %d %d %d %d",generatorFilename,  n_,r_,type_,i,S);
               printf("filenameWithArguments=%s\n",filenameWithArguments);
               int retCode = system(filenameWithArguments);

               int value = rename(generatorOutputFilename, filename);
               if(!value) printf("File name changed successfully\n");
               else {
                    perror("Error");
                    exit(0);
               }
            }
        }
    }
  }
   printf("/nCreated files = %d/n",fileCount);
  return 0;
}

