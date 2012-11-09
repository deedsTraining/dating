/* This program sums up the one minus the log of the probabilities along the column 
(the columns are the "t"'s which range from 1089 to 1466) from directory 
"/u/gelila/computation/dictionary /dateshingfreqdir/" and puts the result into 
a file "datetrainshing1totalprob". NOTE, THE NUMBER OF FREQUENCY FILES IN DIRECTORY
/u/gelila/computation/dictionary/dateshingfreqdir IS CURRENTLY 222547. THIS MEANS YOU HAVE
TO ADJUST THE VALUE OF THIS NUMBER EVERYTIME THE NUMBER OF TRAINING VALUES ARE CHANGED OR THE
BREAK-UP OF THE VALIDATION-TRAINING-TEST SETS*/
   
#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>

/* ****************************************************************************** */

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
    
    /*    printf("%d\n", array); */
    
    *array = currentchar;
    array += sizeof(char);
 
  }
  
  *array = 0;
  
}



/* ******************************************************************************************** */

main(int argc, char *argv[]) {

  FILE *freqfileinptr;
  FILE *resultproboutptr;
  FILE *filecountptr;

  char resultprob[30];

  char shingle[300];

  char *basedirectory="/u/gelila/computation/dictionary/dateshingfreqdir/"; 
  
  char *freqfile="freq";
  char currentfilename[200], currentfilenamenew[200], currentdirfilename[200];
  char countstring[10];
  char currentstring[100];
  
  int t, i, countfile;
  int mindatedata=1089;
  int maxdatedata=1466;
  int mindate=1089;
  int maxdate=1466;
  
  float prob;
  float Colarray[378];

  strcpy(resultprob, argv[1]);  /* datetrainshing2totalprob */
  

  for(t=0;  t<=maxdatedata-mindatedata ; t++){ /* initializing Colarray to 0 */
    Colarray[t]=0;
  }
 
    
  for(i=1; i<=222547; i++){
    strcpy(currentfilenamenew, basedirectory); 
    strcat(currentfilenamenew, freqfile); 
    inttostring(i, currentstring);
    strcat(currentfilenamenew, currentstring);
    strcpy(currentdirfilename, currentfilenamenew); 
    freqfileinptr=fopen(currentdirfilename, "r");
    for(t=0;  t<=maxdatedata-mindatedata ; t++){
      fscanf(freqfileinptr, "%f", &prob);
      Colarray[t]=Colarray[t] + log(1-prob); /* no chance of prob being equal to 1, so we're ok */
      /* printf("%s %d %.12f %.12f \n", shingle, t, log(1-prob), Colarray[t] ); */
    }
    fclose(freqfileinptr);
  }
  
  resultproboutptr=fopen(resultprob, "w");
  for(t=0;  t<=maxdatedata-mindatedata ; t++){
    fprintf(resultproboutptr, "%.12f\t", Colarray[t]);
  }
  fclose(resultproboutptr);
}


