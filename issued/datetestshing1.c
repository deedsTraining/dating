/* This program takes as input "datetrainshing1totalprob" and uses the file "dateshingresultprob" 
to extract probabilities of words from the validation documents, and subtracts log(1-p) from 
the values of "datetrainshing1totalprob"  but then adds log(p) from"dateshingresultprob".
Implementing R.Neal's suggestion of incorporating uninformative words */

/* NOTE WE NEED THE EXACT NAMING "dateshingresultprob" AS IT IS USED IN THE DECLARATION */

   
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

  length = 5;

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



/* ********************************************************************************* */



int binarystringsearch( char **stringvector, char *target, int veclen ) {

  int position = -1;
  int pointer, offset, applyoffset, prevoffset;
  int found = 0;
 


  pointer = veclen / 2;
  offset = veclen;
  
  prevoffset = offset;
  while( prevoffset > 0 && found == 0 ) {
    
    prevoffset = offset;

    if( offset == 1 ) {
      offset = 0;
    } else {
      offset = (offset + 1) / 2;
    }

    applyoffset = offset;

    if( offset == 0 ) {
      applyoffset = 1;
    }

    /*printf("pointer = %d\t string = %s\t offset = %d\n", pointer, stringvector[pointer], offset); */

    /*printf("arrived in binarysearch %d %d \n",prevoffset,found   );*/
    if( strcmp( stringvector[pointer], target ) > 0 ) {
      pointer = pointer - applyoffset;
      if( pointer < 0 ) pointer = 0;
    } else {
      if( strcmp( stringvector[pointer], target ) < 0 ) {
	pointer = pointer + applyoffset;
	if( pointer > veclen - 1 ) pointer = veclen - 1;
      } else {
	found = 1;
	position = pointer;
      }
    }
    
  }
  return position+1; /* returns the position of the location of the shingle, strating from 1....veclen.
			If it returns 0, it means the shingle was not found */
}


/* ******************************************************************************************** */

main(int argc, char *argv[]) {

  FILE *dateshingtotalprobinptr;
  FILE *dateshingresultprobinptr;
  FILE *shinglefileinptr;
  FILE *resultvalproboutptr;
  FILE *currentfileptr;
  FILE *currentdirfileptr;
  FILE *dateinfileptr;
  

  char dateshingtotalprob[30];
  char dateshingprob[30];
  char shinglefile[30];
  char resultvalprob[50];
  char dateinfile[20];

  char *basefilename="/u/gelila/computation/dictionary/";
  char *basedirectory="/u/gelila/computation/dictionary/dateshingfreqdir/";
  char *suffix=".shingled";
  char *freqfile="freq";
  char *currentstring;
  char currentshingle[200];
  char currentfilename[100];
  char currentname[100];
  char currentfilenamenew[200];
  char currentnamenew[100];
  char currentdirfilename[100];

  char shingle[300];

  char *filenamearray[3353];
  char filename[25];
  char *shinglearray[100000];

  int datemasterarray[3353];
  int t, i, j, dummy, date, dateest, location, lengthshingarray;
  int numberoffile=3353;
  int mindatedata=1089;
  int maxdatedata=1466;
  int RESIDUEval=8;
  int RESIDUEtest=9;
  
  float bottomline, logoneminusprob, prob;
  float Colarray[378];
  float Totallogoneminusprob[378];
    
  strcpy(dateshingtotalprob, argv[1]); /* datetrainshing1totalprob */ 
  strcpy(shinglefile, argv[2]); /* mergershing1onlysortuniq */
  strcpy(dateinfile, argv[3]); /* aawmaster */
  strcpy(resultvalprob, argv[4]);  /* dateshingtesdateshingprob */

 
  /* bottomline=exp(-pow(378/5,2))/(450*378); */
  bottomline=exp(-50);
  printf("%f\n", bottomline); 
  for(i=0; i<numberoffile; i++){
    filenamearray[i]= malloc(25*sizeof(char));
  }	
  
  for(i=0; i<=100000; i++){
    shinglearray[i]=malloc(100*sizeof(char));
  }

  dateinfileptr=fopen(dateinfile,"r");
  i=0;
  while(i<numberoffile){        /* read in dates into filenamearray and store them into aawmaster */
    fscanf(dateinfileptr, "%d %d %s", &dummy, &date ,filename);
    datemasterarray[i]=date;
    strcpy(filenamearray[i],filename);
    /* printf("%d %s \n", i,filenamearray[i]); */
    i++;
  }
  fclose(dateinfileptr);
    
  dateshingtotalprobinptr=fopen(dateshingtotalprob, "r");
  for(t=0;  t<=maxdatedata-mindatedata ; t++){
    fscanf(dateshingtotalprobinptr, "%f", &logoneminusprob);
    Totallogoneminusprob[t]=logoneminusprob;
    /* printf("%d  %.12f \n", t,Totallogoneminusprob[t]);  */
  }
  fclose(dateshingtotalprobinptr);
  printf("i'm here \n");

  i=0;
  shinglefileinptr=fopen(shinglefile, "r");
  fscanf(shinglefileinptr, "%s", shingle);
  while(!feof(shinglefileinptr)){
    strcpy(shinglearray[i],shingle);
    i++;
    fscanf(shinglefileinptr, "%s", shingle);
  }
  fclose(shinglefileinptr);
  lengthshingarray=i;
  printf("i=%d, shinglearray[2]=%s\n", i, shinglearray[2]);

  /* ******Compute the probability vectors (from 1089 to 1466) for each document in the validation***** */ 
  
  resultvalproboutptr=fopen(resultvalprob, "w");
  for(i=1; i<=numberoffile; i++){ /* SHOULD BE  for(i=1; i<=numberoffile; i++) */
    /* unlike other times i starts from one b/c we're initialyzing validation docs from aawmaster */
    if( (i%RESIDUEtest==0)&&(i%RESIDUEval!=0)){
      printf("i=%d \n", i);
      strcpy(currentfilenamenew, basefilename);
      strcat(currentfilenamenew, filenamearray[i-1]);
      strcpy(currentfilename, currentfilenamenew);
      strcat(currentfilename, suffix);  /* reads in files such as aaw0009.shingled */
      /* printf("%s \n", currentfilename); */
      for(t=0;  t<=maxdatedata-mindatedata ; t++){ /* initializing Colarray to zero */
	Colarray[t]=0;
      }
      currentfileptr=fopen(currentfilename, "r"); /* opening aaw0009.shingled */
      /* printf("currentfilename=%s\n", currentfilename); */
      while(!feof(currentfileptr)){ 
	fscanf(currentfileptr, "%s", shingle);
	if(!feof(currentfileptr)){
	  location=binarystringsearch(shinglearray, shingle, lengthshingarray); 
	  /* returns the position of the location of the shingle, strating from
	     1.....lengthshingarray. If it returns 0, it means  the shingle was not found */
	  
	  /* printf("shingle=%s, location=%d currentfilename=%s \n", shingle, location, currentfilename);*/
	  if(location!=0){
	    /* printf("location=%d\n", location);*/
	    inttostring(location, currentstring);
	    /*printf("currentstring=%s \n", currentstring);*/
	    strcpy(currentfilenamenew, basedirectory);
	    strcat(currentfilenamenew, freqfile);
	    strcat(currentfilenamenew, currentstring);
	    strcpy(currentdirfilename, currentfilenamenew);  
	    /* printf("currentdirfilename=%s\t",currentstring );*/
	    /*  reads in files e.g "/u/gelila/computation/dictionary /dateshingfreqdir/freq00001" */
	    currentdirfileptr=fopen(currentdirfilename, "r");
	    for(t=0;  t<=maxdatedata-mindatedata ; t++){
	      fscanf(currentdirfileptr, "%f", &prob);
	      if(prob==0){
		Colarray[t]=Colarray[t]/*+Totallogoneminusprob[t]-log(1-prob)*/+log(bottomline);
	      }
	      else{
		Colarray[t]=Colarray[t]/*+Totallogoneminusprob[t]-log(1-prob)*/+log(prob);
	      }
	    }
	    fclose(currentdirfileptr);
	  }
	}
      }
      fclose(currentfileptr); 
      /* print the probability of a val document 378 vectors long */
      dateest=0;
      for(t=0;  t<=maxdatedata-mindatedata ; t++){ 
	if(Colarray[t]>=Colarray[dateest]){
	  dateest=t; 
	}
	/* fprintf(resultvalproboutptr, "%f\n", Colarray[t]); */  /* ****THIS LINE SHOULD BE TAKEN OUT ******  */
      }
      fprintf(resultvalproboutptr, "%d\t%d\n", i, dateest+1089);  /* This line should be kept in ****** */
    }
  }
  fclose(resultvalproboutptr);
}
























































