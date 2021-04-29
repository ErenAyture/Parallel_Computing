#include <stdlib.h>
#include <stdio.h>
#include<time.h>
int main(int argc, char *argv[]){

    int size = atoi(argv[1]);

    FILE *fr;
      fr = fopen("filename.txt", "a");
      if (fr == NULL){
        printf("Error Reading File\n");
        exit (0);
      }
     int ch=0;
     fprintf(fr,"%d\n",size);
     int i = 0;

    srand(time(0));

     while( i < size ){
        while ((ch = fgetc(fr)) != EOF){}
        fprintf(fr,"%d\n",rand()%100 -50);
        i++;
     }
      fclose(fr);

    return 0;
}