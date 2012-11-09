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

void inttostring( int n, char *array ) {  /* HERE WE SET LENGTH TO BE 5 SINCE WE HAVE 36677 FILES */
  int i;
  int length;
  int currentdigit;
  char currentchar;
  char nullchar = 0;

  /* the `length' variable determines how many digits the string will be.  so, for example,
     if it is set to five, the number `1' will be rendered as `0001'.  in this case there shouldn't
     be more than 9999 files.  */

  length = 6;

  for(i = length - 1; i >= 0; i--) {

    /* going through the number by columns, reading from left to right.  so, again assuming length = 4,
       we start with the thousands (10^3) digit, then the hundreds, then the tens, and then the ones. 
       at each step, the digit is scraped off using the floor function, then the appropriate digit
       is added to the output string, then the number is decremented by the amount corresponding to
       the value of the digit, so that it will be reading for the next step.  EXAMPLE:  if n = 2345,
       (and length = 4), the first time through (thousands digit), we'll have 
          currentdigit = floor (2345 / 1000) = 2.
       then currentcharptr is set the ASCII for two, which is 48 + 2 = 50, and n is decremented
       by 2000, so that the new value of n for the next step is 345.  at this time through we'll have
          currentdigit = floor (345 / 100) = 3,
       so the digit `3' will be added to the output string, then 300 will be decremented so the
       new value of n will be 45, etc. */

    currentdigit = floor( n / pow(10, i) );

    /* note that 48 is the ASCII for `0'; 49 for `1', etc. */
    /*    printf("%d\n", currentdigit);*/

    currentchar = 48 + currentdigit;
    n = n - (pow(10, i) * currentdigit);
    
    /* printf("%d\n", array);*/ 
    
    *array = currentchar;
    array += sizeof(char);
 
  }
  
  *array = 0;
  
}


/* ******************************************************************************************** */

main(int argc, char *argv[]) {

  FILE *mergershinginptr;
  FILE *resultoutptr;
  FILE *wordcountfileoutptr;
 
  char mergershinguniq[30];
  char resultdateshing[30];
 
  char currentshingle[300];
  char shingle[400];
  char currentstring[10];
  char currentcountfile[200];

  char *basedirectory="/u/gelila/computation/dictionary/dateshingprobdir/";
  char *wordcountfile="count";

  int trainfilearray[2608]; /* the number of training date */
  int trainfilenum;

  int numberoffile=3353;
  int mindate=1089;
  int maxdate=1466;
  int RESIDUEval=9;
  int RESIDUEtest=8;
  int sizeoftrain=2608;

  int date, repetition;
  int i,k, j,a;

  strcpy(mergershinguniq, argv[1]); /* mergershing2sortuniqboot */
  
  for(i=0;i<sizeoftrain; i++){
    trainfilearray[i]=0;
  }
        
  mergershinginptr=fopen(mergershinguniq,"r");
  fscanf(mergershinginptr, "%d %s %d %d", &repetition, shingle, &date, &trainfilenum);
  strcpy(currentshingle, shingle);
  rewind(mergershinginptr);
  
  j=1;

  /* THIS PART IS TEMPORARY BECAUSE THE PROGRAM CRASHED AT COUNT380092 */
  /* ***************************************************************** */
  /* for(k=1; k<=380090; k++){
    fscanf(mergershinginptr, "%d %s %d %d", &repetition, shingle, &date, &trainfilenum);
    }
    printf("%d \n", k);
    strcpy(currentshingle, shingle);
    j=k; */
  /* ******************************************************************* */

  while(!feof(mergershinginptr)){
    while(!feof(mergershinginptr) && strcmp(shingle,currentshingle)==0){
      a=trainfilenum-floor((float)trainfilenum/RESIDUEval)-floor((float)trainfilenum/RESIDUEtest)+
	floor((float)trainfilenum/(RESIDUEtest*RESIDUEval));
      trainfilearray[a-1]=repetition;
      fscanf(mergershinginptr, "%d %s %d %d", &repetition, shingle, &date, &trainfilenum);
    }
        
    inttostring(j, currentstring);
    strcpy(currentcountfile, basedirectory);
    strcat(currentcountfile, wordcountfile);
    strcat(currentcountfile, currentstring);
    /*printf("%s \n", currentcountfile);*/
    
    wordcountfileoutptr=fopen(currentcountfile,"w");
    for(i=0;i<sizeoftrain; i++){
      fprintf(wordcountfileoutptr, "%d\t", trainfilearray[i]);
    }
    fclose(wordcountfileoutptr);
    j++;
    
    strcpy(currentshingle, shingle);

    /* reset datearray to zero again for the new shingle */
    for(i=0;i<sizeoftrain; i++){
     trainfilearray[i]=0;
    }
  }
  fclose(mergershinginptr);
  printf("shingle= %s \n", shingle);
}







