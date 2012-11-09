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
  char *basefilename="/home/gelila/computation/dictionary/";
  char *suffix=".shingled";
  char *at="@";

  char *filenamearray[9911];
  char filename[12];
  char currentshingle[200];
  char currentshingle1[200];
  char currentshingle2[200];
  char currentfilename[100];
  char currentfilenamenew[100];

  int datemasterarray[9913];
  int traindate; /* date of the training dcouments */

  int singledate, Ldate, Hdate, datediff;
  int RESIDUEtrain;
  int RESIDUEval=8;  
  int RESIDUEtest=9;
  int numberoffile=9911;

  char id[9];
  int i,j, count,date;

  strcpy(dateinfile, argv[1]); /* aaawmaster */
  strcpy(resultfile, argv[2]); /* merger2file */

  for(i=0; i<numberoffile; i++){
    filenamearray[i]= malloc(60*sizeof(char));
  }	


  /* reading and loading into an array the dates of the documents */
  printf("%s\t%s\t\n", dateinfile, resultfile);

  dateinfileptr=fopen(dateinfile,"r");
  i=0;
  while(i<=numberoffile-1){
    fscanf(dateinfileptr, "%s %s %d %d %d %d", id, filename, &singledate, &Ldate, &Hdate, &datediff);
    datemasterarray[i]=singledate;
    if(singledate==0){
      datemasterarray[i]=Ldate;
    }
    printf("id is %s, filename is %s \n", id, filename);
    strcpy(filenamearray[i],filename);
    /* printf("%d %s \n", i,filenamearray[i]); */
    i++;
  }
  fclose(dateinfileptr);
 
  
  printf("i'm here \n");
  /* loading in the date of the training documents */ 
    
  printf("\n\n\n\n\n\n\n\ni'm here\n");
  resultoutfileptr=fopen(resultfile, "w");
  i=1; count=0;
  while(i<=numberoffile){
    if( (i%RESIDUEval !=0)&&(i%RESIDUEtest !=0)){
      traindate= datemasterarray[i-1];
      /* printf("%s \n", filenamearray[i-1]);*/
      strcpy(currentfilenamenew, basefilename);
      strcat(currentfilenamenew, filenamearray[i-1]);
      strcpy(currentfilename, currentfilenamenew);
      strcat(currentfilename, suffix);  /* reads in files such as aaaw00001.shingled */
      /*printf("%d %s \n", i, currentfilename); */
      currentfileptr = fopen(currentfilename,"r");
      while(!feof(currentfileptr)){
	fscanf(currentfileptr, "%s %s", currentshingle1, currentshingle2);
	strcat(currentshingle1, at);
	strcpy(currentshingle, currentshingle1);
	strcat(currentshingle, currentshingle2);
	if( !feof(currentfileptr) ) {
	  fprintf(resultoutfileptr, "%s %d \n", currentshingle, traindate);
	}
      }
      fclose(currentfileptr);
      count++;
    }
    i++;
  }
  fclose( resultoutfileptr );
  printf("count is %d", count);
  
}







