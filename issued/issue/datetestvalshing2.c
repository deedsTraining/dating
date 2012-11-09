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
    
    /*printf("%d\n", array); */
    
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

    /* printf("pointer = %d\t string = %s\t offset = %d\n", pointer, stringvector[pointer], offset); */

    /* printf("arrived in binarysearch %d %d \n",prevoffset,found   );*/
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
  char dateshingprob[40];
  char shinglefile[30];
  char resultvalprob[40];
  char dateinfile[20];

  char *basefilename="/home/gelila/computation/dictionary/";
  char *basedirectory="/home/gelila/computation/dictionary/dateshingfreqdir/";
  char *suffix=".shingled";
  char *at="@";
  char *freqfile="freq";
  char currentstring[10];
  char currentshingle[200];
  char currentfilename[100];
  char currentname[100];
  char currentfilenamenew[200];
  char currentnamenew[100];
  char currentdirfilename[100];

  char shingle[300];
  char shinglefirst[300];
  char shinglesecond[300];
  char shingletemp[300];
  char id[9];

  char *filenamearray[9911];
  char filename[25];
  char *shinglearray[600000];

  int datemasterarray[9911];
  int t, i, j, dummy, date, dateest, location, lengthshingarray;
  int singledate, Ldate, Hdate, datediff;
  int numberoffile=9911;
  int mindatedata=1053;
  int maxdatedata=1491;
  int RESIDUEval=8;
  int RESIDUEtest=9;
  float bottomline, logoneminusprob, prob;
  float Colarray[500];
  float Totallogoneminusprob[500];
    
  strcpy(dateshingtotalprob, argv[1]); /* datetrainshing2totalprob */ 
  strcpy(shinglefile, argv[2]); /* mergershing2onlysortuniq */
  strcpy(dateinfile, argv[3]); /* aaawmaster */
  strcpy(resultvalprob, argv[4]);  /* dateshingtestprob */

 
  /*bottomline=exp(-pow(378/5,2))/(450*378);*/

  bottomline=exp(-50);
  for(i=0; i<numberoffile; i++){
    filenamearray[i]= malloc(25*sizeof(char));
  }	
  
  for(i=0; i<=600000; i++){
    shinglearray[i]=malloc(100*sizeof(char));
  }

  dateinfileptr=fopen(dateinfile,"r");
  i=0;
  while(i<=numberoffile-1){
    fscanf(dateinfileptr, "%s %s %d %d %d %d", id, filename, &singledate, &Ldate, &Hdate, &datediff);
    if(singledate==0){
      datemasterarray[i]=Ldate;
    }else{
      datemasterarray[i]=singledate;
    }
    /* printf("id is %s, filename is %s \n", id, filename);*/
    strcpy(filenamearray[i],filename);
    /* printf("%d %s \n", i,filenamearray[i]); */
    i++;
  }
  fclose(dateinfileptr);
  
 

  /* dateshingtotalprobinptr=fopen(dateshingtotalprob, "r");
  for(t=0;  t<=maxdatedata-mindatedata ; t++){
    fscanf(dateshingtotalprobinptr, "%f", &logoneminusprob);
    Totallogoneminusprob[t]=logoneminusprob;
    printf("%d  %.12f \n", t,Totallogoneminusprob[t]);  
  }
  fclose(dateshingtotalprobinptr);
  */
  /* printf("i'm here \n");*/

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
  /* printf("i=%d, shinglearray[2]=%s\n", i, shinglearray[2]);*/

  /* ******Compute the probability vectors (from 1053 to 1491) for each document in the validation***** */ 
  
  resultvalproboutptr=fopen(resultvalprob, "w");
  for(i=1; i<=numberoffile; i++){ /* Testing for for(i=90; i<=90; i++) */
    /* unlike other times i starts from one b/c we're initialyzing validation docs from aawmaster */
    if( (i%RESIDUEval==0)||(i%RESIDUEtest==0)){
      strcpy(currentfilenamenew, basefilename);
      strcat(currentfilenamenew, filenamearray[i-1]);
      strcpy(currentfilename, currentfilenamenew);
      strcat(currentfilename, suffix);  /* reads in files such as aaaw0008.shingled */
      /*printf("\n\n");
      printf("%s \n", currentfilename);
      printf("\n\n");*/
      for(t=0;  t<=maxdatedata-mindatedata ; t++){ /* initializing Colarray to zero */
	Colarray[t]=0;
      }
      currentfileptr=fopen(currentfilename, "r"); /* opening aaaw0008.shingled */
      /* printf("currentfilename=%s\n", currentfilename); */
      while(!feof(currentfileptr)){ 
	fscanf(currentfileptr, "%s", shinglefirst); fscanf(currentfileptr, "%s", shinglesecond);
	strcpy(shingle, shinglefirst);
	strcat(shingle, at);
	strcpy(shingletemp, shinglesecond);
	strcat(shingle, shingletemp); 
	if(!feof(currentfileptr)){
	  /*printf("shingle=%s\n", shingle);*/
	  location=binarystringsearch(shinglearray, shingle, lengthshingarray); 
	  /* returns the position of the location of the shingle, strating from
	     1.....lengthshingarray. If it returns 0, it means  the shingle was not found */
	  
	  /* printf("shingle=%s, location=%d currentfilename=%s \n", shingle, location, currentfilename);*/
	  /* printf("location=%d\n",location);*/
	  if(location!=0){
	    inttostring(location, currentstring);
	    strcpy(currentfilenamenew, basedirectory);
	    strcat(currentfilenamenew, freqfile);
	    strcat(currentfilenamenew, currentstring);
	    strcpy(currentdirfilename, currentfilenamenew);  
	    /* printf("shingle=%s, location=%d currentdirfilename=%s \n", shingle, location, currentdirfilename);*/
	    /*  printf("currentdirfilename=%s\n", currentdirfilename);*/
	    /*  reads in files e.g "/home/gelila/computation/dictionary /dateshingfreqdir/freq000001" */
	    currentdirfileptr=fopen(currentdirfilename, "r");
	    for(t=0;  t<=maxdatedata-mindatedata ; t++){
	      fscanf(currentdirfileptr, "%f", &prob);
	      if(prob==0){
		Colarray[t]=Colarray[t]/* + Totallogoneminusprob[t]-log(1-prob)*/+log(bottomline);
	      }
	      else{
		Colarray[t]=Colarray[t]/*+ Totallogoneminusprob[t]-log(1-prob) */+log(prob);
	      }
	    }
	    fclose(currentdirfileptr);
	  } /* if(location!=0){ */
	}
      } /* while(!feof(currentfileptr)){ */
      fclose(currentfileptr); 
      /* print the probability of a val document 378 vectors long */
      dateest=0;
      for(t=0;  t<=maxdatedata-mindatedata ; t++){ 
	if(Colarray[t]>=Colarray[dateest]){
	  dateest=t; /* printf("dateest %d \n", t); */
	}
      }
      /* printf("\n%s\n\n\n\n",currentfilename); */
      fprintf(resultvalproboutptr, "%d\t %d\n", dateest+mindatedata,datemasterarray[i-1]);  /* This line should be kept in ****** */
    }
  }
  fclose(resultvalproboutptr);
}
























































