/* this program creates an array, datereptrain, where the 1st column is the date and the 2nd is
   the number of total shingles corresponding to the dates of the 1st column for the training
 documents*/

#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>

/* ******************************************************************************************** */
void inttostring( int n, char *array ) {
  int i;
  int length;
  int currentdigit;
  char currentchar;
  char nullchar = 0;

  /* the `length' variable determines how many digits the string will be.  so, for example,
     if it is set to five, the number `1' will be rendered as `0001'.  in this case there shouldn't
     be more than 9999 files.  */

  length = 4;

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
    
    /*    printf("%d\n", array); */
    
    *array = currentchar;
    array += sizeof(char);
 
  }
  
  *array = 0;
  
}


/*  ****************************************************************************************** */

main(int argc, char *argv[]) {

  FILE *resultoutptr;
  FILE *dummyfileinptr;

  char resulttotalshing[30];
  char currentfile[400];
  
  char *shingledsuffix;
  char *prefix;
  char *currentstring;
  char *wordcount;
  char *dummyfile;

  int Ndoc[2][2608]; /* the number of training date */
  int trainfilenum;

  int sizeoftrain=2608;
  int numberoffile=3353;
  int RESIDUEval=9;
  int RESIDUEtest=8;

  int i,j, size;
  
  strcpy(resulttotalshing, argv[1]); /* totaldocboot */

  /* loading into an array the number of total words in each of the training documents */
  
  currentstring = malloc(10 * sizeof(char));
  shingledsuffix = ".shingled";
  prefix="aaw";
  wordcount="wc -l ";
  dummyfile=" >dummyfile";

  resultoutptr=fopen(resulttotalshing, "w");

  j=0;
  for(i=1;i<=3353; i++){
    if( (i%8)!=0 && (i%9)!=0 ){
      Ndoc[0][j]=i;
      j=j+1;
    }
  }
  for(j=0; j<sizeoftrain; j++){
    inttostring(Ndoc[0][j], currentstring);
    strcpy(currentfile, wordcount);
    strcat(currentfile, prefix);
    strcat(currentfile, currentstring);
    strcat(currentfile, shingledsuffix);
    strcat(currentfile, dummyfile);
    system(currentfile);
    printf("%s \n", currentfile);
    dummyfileinptr=fopen("dummyfile","r");
    fscanf(dummyfileinptr, "%d", &size);
    fclose(dummyfileinptr);
    Ndoc[1][j]=size;  /* size of document in Ndoc[0][j] */
    fprintf(resultoutptr, "%d\n", Ndoc[1][j]);
  }
  fclose(resultoutptr);
}



