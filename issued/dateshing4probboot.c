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

  FILE *totalwordcountinptr;
  FILE *dateinptr;
  FILE *currentshinglefileinptr;
  FILE *shinglefileinptr;
  FILE *wordcountfileinptr;
  FILE *dateestfileoutptr;
  FILE *testoutputptr;
  FILE *randomnumfileinptr;


  char shinglefile[30];
  char totalwordcount[30];
  char randomnumfile[30];
  char aawmaster[30];
  char dateest[30];

  char *basedirectory="/u/gelila/computation/dictionary/dateshingprobdir/";
  char *wordcountfile="count";
  char *shingled=".shingled";
  char *basename="aaw";
  char *at="@";
  char currentfilename[200];
  char *currentstring;
  char tempcurrentstring[100];

  /*char currentstring[10];*/
  char dummystring1[20];
  char dummystring2[20];
  char dummystring3[20];
  char dummystring4[20];

  char shingle[300];
  char *shinglearray[500000];

  int i, j, l,k, t, count,wordcount, ti,freq, date, remainder, lengthshingarray,location, optdate,randomnum ;

  int randomnumvec[1500000];
  int traindate[3000];
  int bootvector[3000];
  int Occvector[3000];
  int Occvectortotal[3000];
  int ns[3000];
  int Nt[3000];
  int indicator[3000];

  int sizeoftrain=2608;
  int /* mindatedata=1050;*/ mindatedata=1089;
  int /* maxdatedata=1500; */ maxdatedata=1466;
  int mindate=1089;
  int maxdate=1466;
  int RESIDUEval=8;
  int RESIDUEtest=9;
  int bootstrapsample=500;

  float h, sum, Num[400], Den[400], probs[400], probdoc[400], kernelvalue[378];
  float bottomline;
 
  strcpy(shinglefile, argv[1]); /* mergershing4onlysortuniq */
  strcpy(randomnumfile, argv[2]); /* randomnumberfile */
  strcpy(totalwordcount, argv[3]); /*  totaldocboot */
  strcpy(aawmaster, argv[4]); /* aawmaster */
  h=atoi(argv[5]); /* bandwidth value h */
  strcpy(dateest, argv[6]); /* dateestboot */ 
 

  currentstring = malloc(11*sizeof(char));

  totalwordcountinptr=fopen(totalwordcount, "r");
  for(i=0; i<sizeoftrain; i++){
    fscanf(totalwordcountinptr, "%d",  &wordcount);
    Occvectortotal[i]=wordcount;
  }
  fclose(totalwordcountinptr);
  
  for(i=0; i<=500000; i++){
    shinglearray[i]=malloc(200*sizeof(char));
  }
  
  i=0; 
  shinglefileinptr=fopen(shinglefile, "r");
  fscanf(shinglefileinptr, "%s", shingle);
  while(!feof(shinglefileinptr)){
    /*printf("%s \n", shingle);*/
    strcpy(shinglearray[i],shingle);
    fscanf(shinglefileinptr, "%s", shingle);
    i++;
  }
  fclose(shinglefileinptr);
  lengthshingarray=i;
  printf("%d %s \n", i, shinglearray[i]);

  randomnumfileinptr=fopen(randomnumfile, "r");
  j=0;
  while(!feof(randomnumfileinptr)){
    fscanf(randomnumfileinptr, "%d", &randomnum);
    randomnumvec[j]=randomnum;
    j++;
  }
  printf("%d \n", j);

 
  dateinptr=fopen(aawmaster, "r");
  j=0;
  for(i=1; i<=3353; i++){
    fscanf(dateinptr, "%s %d %s", dummystring1, &date, dummystring2); 
    if( (i%RESIDUEval)!=0 && (i%RESIDUEtest)!=0 ){
      traindate[j]=date;
      j++;
    }
  }
  fclose(dateinptr);

  /* PUT THE KERNEL VALUES HERE */
  for(i=0; i<=maxdate-mindate;i++){
    kernelvalue[i]= dt((float)(i/h),10.5);
    /*printf("%.12f \n",  kernelvalue[i]);*/
  }
  bottomline=/*exp(-pow(378/5,2))/(450*378);*/ log(pow(5,-30));
  dateestfileoutptr=fopen(dateest, "w");
  /* this comand just wipes out the previous contents of the file.   later it will
     be re-opened in append mode */
  fclose(dateestfileoutptr);


  for(k=0; k<bootstrapsample; k++){ /* AN OUTER LOOP HERE FOR REPEATING BOOTSTRAP SAMPLING */
    for(i=0; i<sizeoftrain; i++){
      bootvector[i]=0;
    }
    for(i=0; i<sizeoftrain; i++){  /* THE END OF THE LOOP DOESN'T HAVE TO BE sizeoftrain....SHOULD
				      PERHAPSE BE MUCH LARGER */
       bootvector[i]=randomnumvec[i + sizeoftrain * k];
      /* printf("%d ",  bootvector[i]); */
    }
   
    if(k>=0){  /* for debugging purposes ....since it crashes after k>=72 */
    
      for(i=1; i<=1026/*3353*/; i++){
	if( (i%RESIDUEtest)==0 && (i%RESIDUEval)!=0 ){
	  inttostring(i, currentstring);  /* TAKING A DOC FROM THE TEST SET */
	  currentstring = currentstring + 2*sizeof(char);
	  strcpy(currentfilename, basename);
	  strcat(currentfilename, currentstring);
	  strcat(currentfilename, shingled);
	  currentstring = currentstring - 2*sizeof(char);  /* reset the pointer for currentstring */
	  currentshinglefileinptr=fopen(currentfilename, "r"); /* SUCH AS AAW0009.SHINGLED */ 
	  for(t=0; t<=maxdate-mindate; t++){
	    probdoc[t]=0;
	  }
	  while(!feof(currentshinglefileinptr)){
	    fscanf(currentshinglefileinptr, "%s %s %s %s", dummystring1, dummystring2, dummystring3, 
		   dummystring4);
	    strcpy(shingle, dummystring1);
	    strcat(shingle, at);
	    strcat(shingle, dummystring2);
	    strcat(shingle, at);
	    strcat(shingle, dummystring3);
	    strcat(shingle, at);
	    strcat(shingle, dummystring4);
	    
	    location=binarystringsearch(shinglearray, shingle, lengthshingarray); 
	    /* printf("%s %d \n", shingle, location); */
	    if(location!=0){
	      inttostring(location, currentstring);
	      strcpy(tempcurrentstring, basedirectory);
	      strcat(tempcurrentstring, wordcountfile);
	      strcat(tempcurrentstring, currentstring);
	      /*printf("%s %d %s \n", shingle, location, tempcurrentstring);*/
	      wordcountfileinptr=fopen(tempcurrentstring, "r");
	      count=0;
	      fscanf(wordcountfileinptr, "%d", &freq); 
	      while(!feof(wordcountfileinptr)){
		Occvector[count]=freq;
		fscanf(wordcountfileinptr, "%d", &freq);
		count++;
	      }
	      fclose(wordcountfileinptr);
	      /*printf("count=%d \n", count);*/
	      /* NEED TO SET UP dateshingresultfileboot FROM THE PREVIOUS PROGRAM INTO FILE
		 AND THEN USE mergershing2onlysortuniq AS AN INPUT IN ORDER TO USE THE BINARY SEARCH
		 ALGORITHM */
	      /* ASSUMING THE LOCATION OF THE SHINGLE HAS BEEN FOUND, (NEED TO PUT STATEMENT IF NOT FOUND) */
	      
	      /* YOU THEN PUT ALL THE 2608 COLUMNS INTO AN ARRAY CALLED Occvector */
	      
	      for(l=0; l<sizeoftrain; l++){
		ns[l]=Occvector[l]*bootvector[l];
	      }
	      
	      for(t=0; t<=maxdate-mindate; t++){
		sum=0;
		for(j=0; j<sizeoftrain; j++){
		  sum=sum+((float) ns[j])*kernelvalue[abs( (traindate[j]-mindate)-t )]; 
		  /*printf("kernelvalue[traindate[j]-t]=%.20f \n", 
		    kernelvalue[abs( (traindate[j]-mindate)-t )]);*/
		}
		Num[t]=sum;
	      }
	      
	      
	      for(l=0; l<sizeoftrain; l++){
		Nt[l]=Occvectortotal[l]*bootvector[l];
		/*printf("occvectortotal[%d] = %d \t bootvector[%d] = %d \n", l, Occvectortotal[l], l, 
		  bootvector[l] );*/
	      }
	      
	      for(t=0; t<=maxdate-mindate; t++){
		sum=0;
		for(j=0; j<sizeoftrain; j++){
		  sum=sum+((float) Nt[j])*kernelvalue[abs( (traindate[j]-mindate)-t )]; 
		}
		Den[t]=sum;
	      }
	      for(t=0; t<=maxdate-mindate; t++){
		/*printf("%.20f %.20f \n", Num[t], Den[t]);*/
	      }
	      
	      for(t=0; t<=maxdate-mindate; t++){
		if(log((float)(Num[t]/Den[t])) <bottomline){
		  probs[t]=bottomline;
		}
		else{
		  probs[t]=log((float)(Num[t]/Den[t]));
		}
	      }
	      /*printf("%f %f %f \n", Num[t], Den[t],probs[t]);*/
	      for(t=0; t<=maxdate-mindate; t++){
		probdoc[t]=probdoc[t]+probs[t];
	      }
	    } /* if(location!=0) */
	  } /*while(!feof(currentshinglefileinptr)) */
	  fclose(currentshinglefileinptr);
	  optdate=0;
	  for(t=0;  t<=maxdatedata-mindatedata ; t++){ 
	    /*printf("%.12f \n", probdoc[t]);*/
	    if(probdoc[t]>=probdoc[optdate]){
	      optdate=t; 
	    }
	  }
	  
	  dateestfileoutptr=fopen(dateest, "a");
	  fprintf(dateestfileoutptr, "%d\t%d\n", optdate+1089, i);
	  fclose(dateestfileoutptr);
	  
	  printf("%d %d\n",optdate+1089, i);
	}
      } /* for(i=1; i<=3353; i++) */
    }
    /*fclose(dateestfileoutptr);*/
  } /* for(k=1; k<=bootstrapsample; k++) */
}






















