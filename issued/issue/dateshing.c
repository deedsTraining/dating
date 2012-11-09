/* this program constructs a file "dateshingresultfile" such that for each date ranging from 
1089 to 1466, the number of time a shingle is found is written to the corresponding date. 
"dateshingresultfile" consists of a shingle followed on the same line 
(columns of length 378 (1466-1089)) which indicates the number of time the shingle occurred 
for a given date.*/ 

#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>


/* ******************************************************************************************** */

main(int argc, char *argv[]) {

  FILE *mergershinginptr;
  FILE *resultoutptr;

  char mergershinguniq[30];
  char resultdateshing[30];
  char currentshingle[300];
  char shingle[400];

  int dateshingarray[439];

  int numberoffile=9911;
  int mindate=1053;
  int maxdate=1491;

  int date, repetition;
  int i,j;

  strcpy(mergershinguniq, argv[1]); /* mergershing2sortuniq */
  strcpy(resultdateshing, argv[2]); /* dateshingresultfile (a file to print out result to) */
  
  for(i=0;i<=maxdate-mindate; i++){
    dateshingarray[i]=0;
    /*printf("%d %d\n", dateshingarray[i][0],dateshingarray[i][1]);*/
  }
  
  mergershinginptr=fopen(mergershinguniq,"r");
  fscanf(mergershinginptr, "%d %s %d", &repetition, shingle, &date);
  strcpy(currentshingle, shingle);
  rewind(mergershinginptr);
  
  resultoutptr=fopen(resultdateshing, "w");
 
  while(!feof(mergershinginptr)){
    while(strcmp(shingle,currentshingle)==0 &&!feof(mergershinginptr)){
      dateshingarray[date-mindate]=repetition;
      fscanf(mergershinginptr, "%d %s %d", &repetition, shingle, &date);
    }
    fprintf(resultoutptr, "%s\t", currentshingle);
    for(i=0;i<=maxdate-mindate; i++){
      fprintf(resultoutptr, "%d\t", dateshingarray[i]);
    }
    fprintf(resultoutptr, "\n");
    strcpy(currentshingle, shingle);

    /* reset datearray to zero again for the new shingle */
    for(i=0;i<=maxdate-mindate; i++){
      dateshingarray[i]=0;
    }
  }
  fclose(mergershinginptr);
  fclose(resultoutptr);

}


