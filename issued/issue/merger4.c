/* the purpose of this program is to merge all the file of the training documents 
   into one where the 1st column is the word and the 2nd is the date in which the word is
   written */ 


#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>



/* ******************************************************************************************** */


main(int argc, char *argv[]) {

  FILE *dateinfileptr;
  FILE *currentfileptr;
  FILE *resultoutfileptr;
  
  char dateinfile[20];
  char resultfile[20];
  char *basefilename="/u/gelila/computation/dictionary/";
  char *suffix=".shingled";
  char *at="@";

  char *filenamearray[3353];
  char filename[25];
  char currentshingle[200];
  char currentshingletemp[200];
  char currentshingle1[200];
  char currentshingle2[200];
  char currentshingle3[300];
  char currentshingle4[400];
  char currentfilename[100];
  char currentfilenamenew[100];

  int datemasterarray[3353];
  int traindate; /* date of the training dcouments */

  int RESIDUEtrain;
  int RESIDUEval=8;  
  int RESIDUEtest=9;
  int numberoffile=3353;

  int dummy;
  int i,j, date;

  strcpy(dateinfile, argv[1]); /* aawmaster */
  strcpy(resultfile, argv[2]); /* mergerfile */

  for(i=0; i<numberoffile; i++){
    filenamearray[i]= malloc(25*sizeof(char));
  }	


  /* reading and loading into an array the dates of the documents */
  printf("%s\t%s\t\n", dateinfile, resultfile);

  dateinfileptr=fopen(dateinfile,"r");
  i=0;
  while(i<numberoffile){
    fscanf(dateinfileptr, "%d %d %s", &dummy, &date ,filename);
    datemasterarray[i]=date;
    strcpy(filenamearray[i],filename);
    /*printf("%d %s \n", i,filenamearray[i]); */
    i++;
  }
  fclose(dateinfileptr);
 
  
  printf("i'm here \n");
  /* loading in the date of the training documents */ 
    
  printf("\n\n\n\n\n\n\n\ni'm here\n");
  resultoutfileptr=fopen(resultfile, "w");
  i=1;
  while(i<=numberoffile){
    if( (i%RESIDUEval !=0)&&(i%RESIDUEtest !=0)){
      traindate= datemasterarray[i-1];
      /*printf("%s \n", filenamearray[i-1]);*/
      strcpy(currentfilenamenew, basefilename);
      strcat(currentfilenamenew, filenamearray[i-1]);
      strcpy(currentfilename, currentfilenamenew);
      strcat(currentfilename, suffix);  /* reads in files such as aaw0001.shingled */
      /*printf("%d %s \n", i, currentfilename); */
      currentfileptr = fopen(currentfilename,"r");
      while(!feof(currentfileptr)){
	fscanf(currentfileptr, "%s %s %s %s", currentshingle1, currentshingle2, currentshingle3,
	       currentshingle4);
	strcat(currentshingle1, at);
	strcpy(currentshingle, currentshingle1);
	strcat(currentshingle, currentshingle2);
	strcpy(currentshingletemp, at);
	strcat(currentshingle,currentshingletemp);
	strcpy(currentshingletemp, currentshingle);
	strcat(currentshingletemp, currentshingle3);
	strcpy(currentshingle, currentshingletemp);
	strcat(currentshingle, at);
	strcpy(currentshingletemp, currentshingle4);
	strcat(currentshingle, currentshingle4);
	if( !feof(currentfileptr) ) {
	  fprintf(resultoutfileptr, "%s %d \n", currentshingle, traindate);
	}
      }
      fclose(currentfileptr);
    }
    i++;
  }
  fclose( resultoutfileptr );
 
  
}







