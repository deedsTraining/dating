/* This program is designed to compute the estimated probability of a shingle and store it into
   a directory called "/u/gelila/computation/dictionary/dateshingfreqdir/". Each shingle coming
from "dateshingresultfile" is sequentially assigned a file name freq00001, freq00002, etc... and each
of these files contain the N-W estimated probability for each date ranging from 1089 to 1466, i.e each 
file contains a single line of length 378. */

#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>

/* ******************************************************************************************** */

float dt(float x, float nu) {
  float modiftdensity;
    
  modiftdensity = pow ( (  (float)pow(x,2) / (float)nu )+ 1 , ( -(float)(nu+1) / (float)2 ) );
  
  return modiftdensity;
}


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

  FILE *dateshingresultinptr;
  FILE *datetotalinptr;
  FILE *currentfileoutptr;

  char dateshingresult[30];
  char datetotal[30];

  char shingle[400];

  char *basedirectory="/u/gelila/computation/dictionary/dateshingfreqdir/";
  char *freqfile="freq";
  char currentfilename[200];
  char currentstring[10];

  int i, j, t, ti, date;
  int Nti[400], nsti[400];
  int /* mindatedata=1050;*/ mindatedata=1089;
  int /* maxdatedata=1500; */ maxdatedata=1466;
  int mindate=1089;
  int maxdate=1466;
  float h, df, sumnum, sumden;
 

  strcpy(dateshingresult, argv[1]); /* dateshingresultfile */
  strcpy(datetotal, argv[2]); /*  datetotal4 */
  /* strcpy(resultout, argv[3]);  dateshingresultprob (nolonger we need this as results will be written 
   in .....dictionary/dateshingfreqdir  directory with each file corresponding to the N-W estimator
   of a shingle for each date ranging fron 1089 to 1466 */
  h=atoi(argv[3]); /* bandwidth value h */
  df=atoi(argv[4]); /* degree of freedom */

  datetotalinptr=fopen(datetotal, "r");
  for(ti=0; ti<=maxdate-mindate; ti++){
    fscanf(datetotalinptr, "%d %d", &date, &Nti[ti]);
  }
  fclose(datetotalinptr);

  dateshingresultinptr=fopen(dateshingresult, "r");
  printf("i'm here 1\n");
  
  /* ADDING THESE HERE BECAUSE THE PROGRAM CRASHED AFTER freq117462 */
  /* ************************************************************** */
  /*for(i=1; i<= 117462; i++){
    fscanf(dateshingresultinptr, "%s", shingle);
    for(ti=0; ti<=maxdate-mindate ; ti++){
      fscanf(dateshingresultinptr, "%d", &nsti[ti]);
    }
    }*/

  /* ************************************************************** */
  
  i=1;
  while(!feof(dateshingresultinptr)){
    fscanf(dateshingresultinptr, "%s", shingle);
    /*printf("%d %s \n", i, shingle);*/
    if(!feof(dateshingresultinptr)) {
      for(ti=0; ti<=maxdate-mindate ; ti++){
	fscanf(dateshingresultinptr, "%d", &nsti[ti]);
      }
      
      inttostring(i, currentstring);
      /*printf("%s\n", currentstring);*/
      strcpy(currentfilename, basedirectory);
      strcat(currentfilename, freqfile);
      strcat(currentfilename, currentstring);
      currentfileoutptr=fopen(currentfilename, "w");

      for(t=0;  t<=maxdatedata-mindatedata ; t++){
	/*printf("starting t = %d\n", t);*/
	sumnum=0; sumden=0;
	for(ti=0; ti<=maxdate-mindate; ti++){
	  sumnum=sumnum+nsti[ti]*dt((float)(ti-t)/h,df);    /* *exp(-pow((float)((ti-t)/h),2)/2) ;*/
	  sumden=sumden+Nti[ti]*dt((float)(ti-t)/h,df);  /*exp(-pow((float)((ti-t)/h),2)/2)  ;*/
	}

	/* to print in the directory .../dictionary/dateshingfreqdir */
	/*printf("%d\t%d\t%s\t%.12f \n", i, t,currentfilename, sumnum/sumden);*/
	fprintf(currentfileoutptr, "%.12f\t", sumnum/sumden);
      }
      fprintf(currentfileoutptr,"\n");
      fclose(currentfileoutptr);
      i++;
    }
  }
  fclose(dateshingresultinptr);
}

















































