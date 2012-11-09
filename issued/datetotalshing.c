/* this program creates an array, datereptrain, where the 1st column is the date and the 2nd is
   the number of total shingles corresponding to the dates of the 1st column for the training
 documents*/

#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>

/* ******************************************************************************************** */


main(int argc, char *argv[]) {

  FILE *mergerdateuniqinptr;
  FILE *resultoutptr;
  
  char mergerdateuniq[30];
  char resultdateshing[30];
  char currentshingle[400];
  char shingle[400];
  
  int numberoffile=9911;
  int mindate=1053;
  int maxdate=1491;
 
  int datereptrain[500]; /* 1st column is the date and the 2nd is repetition of words in
			      that date for the training data */
  int date, repetition;
  int i,j;
  
  strcpy(mergerdateuniq, argv[1]); /* merger2datesortuniq */
  strcpy(resultdateshing, argv[2]); /* datetotal */

  /* reading and loading into an array the unique dates of the training documents */
  
  mergerdateuniqinptr=fopen(mergerdateuniq, "r");
  resultoutptr=fopen(resultdateshing, "w");

  for(i=0;i<=maxdate-mindate; i++){
    datereptrain[i]=0;
  }
  
  while(!feof(mergerdateuniqinptr)){
    fscanf(mergerdateuniqinptr, "%d %d", &repetition, &date);
    datereptrain[date-mindate]=repetition;
  }
   printf("got here\n");
  for(i=0; i<=maxdate-mindate; i++){
    fprintf(resultoutptr, "%d %d \n", i+mindate, datereptrain[i]);
  }
    
  fclose(mergerdateuniqinptr);
  fclose(resultoutptr);
}

  





