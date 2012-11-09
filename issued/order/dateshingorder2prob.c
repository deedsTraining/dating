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


/* ******************************************************************************************** 
   array relatedseq */


/* ******************************************************************************************** */
main(int argc, char *argv[]) {

  FILE *dateshingresultinptr;
  FILE *sampleseqfileinptr;
  FILE *docprobfileoutptr;

  char dateshingresult[30];
  char sampleseq[30];
  char docprobfile[30];
  char shingle[300];

  char currentfilename[200];
  char currentstring[10];

  int i,j,l,m,count, flag, seqline, ti, repetition;
  int Nti[30], nsti[30]; int sampleseqdata[10], time[10];
  int mindate; int maxdate;
  float mindatedata, maxdatedata, increment, t;
  float h, df, aveprob, sumnum, sumden, bottomsum;
  float docprob[10][3000];

  strcpy(dateshingresult, argv[1]); /*  dateordershingresultfile */
  strcpy(sampleseq, argv[2]); /* sequences */
  strcpy(docprobfile, argv[3]); /* docprobfile */
  h=atoi(argv[4]); /* bandwidth value h */
  df=atoi(argv[5]); /* degree of freedom */
  

  /* mindatedata be not lower than 0 and maxdatedata bigger than 7 */
  mindatedata=10; maxdatedata=260; mindate=40;  maxdate=230;
  increment=0.5;

  /* ******************************************** */

  Nti[0]=18713; Nti[1]=21904; Nti[2]=19778; Nti[3]=19727;
  Nti[4]=21782; Nti[5]=24763; Nti[6]=14188;
  bottomsum=Nti[0]+Nti[1]+Nti[2]+Nti[3]+Nti[4]+Nti[5]+Nti[6];

  time[0]=60; time[1]=140; time[2]=200; time[3]=260; time[4]=320;
  time[5]=380; time[6]=440;

  /* *************************************************** */

  
  /* sampleseqfileinptr=fopen(sampleseq,"r");
  for(i=0; i<=69; i++){
    fscanf(sampleseqfileinptr, "%d", &l);
  }
  for(i=70; i<=76; i++){
    fscanf(sampleseqfileinptr, "%d", &sampleseqdata[i-70]);
  }
  fclose(sampleseqfileinptr);
  */

  sampleseqfileinptr=fopen(sampleseq,"r");docprobfileoutptr=fopen(docprobfile, "w");
  j=0; seqline=1;
  while(j<=3052){ /*j should be upto 693. There are 100 rows of sequences with each row containing 7 
		   scrambled numbers */
    /* printf("sequence line %d \n", seqline);*/
    for(i=j; i<=j+6; i++){
      fscanf(sampleseqfileinptr, "%d", &sampleseqdata[i-j]);
      /* printf("%d ", sampleseqdata[i-j]);*/
    }
    j=j+7;
    /* printf("j is now %d \n\n", j);*/
        
    /* sampleseqdata[0]=1; sampleseqdata[1]=2; sampleseqdata[2]=3; sampleseqdata[3]=4;
       sampleseqdata[4]=5; sampleseqdata[5]=6; sampleseqdata[6]=7; */
    
    l=0;  /* initializing the docprod to be zero */
    for(i=1; i<=7; i++){
      m=0;
      for(t=mindatedata;  t<=maxdatedata; t=t+increment){
	docprob[l][m]=0;
	m++;
      }
      l++; 
    }
    /* printf("l=%d, m=%d \n", l,m);*/
    
    dateshingresultinptr=fopen(dateshingresult, "r");
    while(!feof(dateshingresultinptr)){
      fscanf(dateshingresultinptr, "%s", shingle);
      /*      printf(" %s \n", shingle); */
      if(!feof(dateshingresultinptr)) {
	/* for(ti=mindate; ti<=maxdate ; ti=ti+20){
	  fscanf(dateshingresultinptr, "%d", &nsti[(ti/20)-1]);
	  }*/
	fscanf(dateshingresultinptr, "%d", &nsti[0]);
	fscanf(dateshingresultinptr, "%d", &nsti[1]);
	fscanf(dateshingresultinptr, "%d", &nsti[2]);
	fscanf(dateshingresultinptr, "%d", &nsti[3]);
	fscanf(dateshingresultinptr, "%d", &nsti[4]);
	fscanf(dateshingresultinptr, "%d", &nsti[5]);
	fscanf(dateshingresultinptr, "%d", &nsti[6]);
	  
	count=0;
	for(t=mindatedata;  t<=maxdatedata; t=t+increment){
	  /*printf("starting t = %d\n", t);*/
	  /* sumnum=0; sumden=0; 
	  for(ti=mindate; ti<=maxdate; ti=ti+20){
	    sumnum=sumnum+nsti[(ti/20)-1]*dt((float)(20*sampleseqdata[(ti/20)-1]-t)/h, df);    
	    sumden=sumden+Nti[(ti/20)-1]*dt((float)(20*sampleseqdata[(ti/20)-1]-t)/h, df);  
	    } */
	  sumnum=nsti[0]*dt((float)(30*(sampleseqdata[0]-1)+50-t)/h, df)
	    +nsti[1]*dt((float)(30*(sampleseqdata[1]-1)+50-t)/h, df)
	    +nsti[2]*dt((float)(30*(sampleseqdata[2]-1)+50-t)/h, df)
	    +nsti[3]*dt((float)(30*(sampleseqdata[3]-1)+50-t)/h, df)
	    +nsti[4]*dt((float)(30*(sampleseqdata[4]-1)+50-t)/h, df)
	    +nsti[5]*dt((float)(30*(sampleseqdata[5]-1)+50-t)/h, df)
	    +nsti[6]*dt((float)(30*(sampleseqdata[6]-1)+50-t)/h, df);

	  sumden=Nti[0]*dt((float)(30*(sampleseqdata[0]-1)+50-t)/h, df)
	    +Nti[1]*dt((float)(30*(sampleseqdata[1]-1)+50-t)/h, df)
	    +Nti[2]*dt((float)(30*(sampleseqdata[2]-1)+50-t)/h, df)
	    +Nti[3]*dt((float)(30*(sampleseqdata[3]-1)+50-t)/h, df)
	    +Nti[4]*dt((float)(30*(sampleseqdata[4]-1)+50-t)/h, df)
	    +Nti[5]*dt((float)(30*(sampleseqdata[5]-1)+50-t)/h, df)
	    +Nti[6]*dt((float)(30*(sampleseqdata[6]-1)+50-t)/h, df);

	  for(i=0;i<=6;i++){
	    if(sampleseqdata[i]==1){
	      flag=i;
	    }
	  }
	  sumnum=sumnum-nsti[flag]*dt((float)(30*(sampleseqdata[flag]-1)+50-t)/h, df)
	    +nsti[flag]*dt((float)(40*sampleseqdata[flag]-t)/h, df);
	  sumden=sumden-Nti[flag]*dt((float)(30*(sampleseqdata[flag]-1)+50-t)/h, df)
	    +Nti[flag]*dt((float)(40*sampleseqdata[flag]-t)/h, df);
	  /*printf("%.4f\t.12f \n",  t,sumnum/sumden);*/
	  for(i=0;i<=6;i++){
	    if(nsti[i]!=0){ /* to see if the shingle had occurred in time ti */
	      docprob[i][count]=docprob[i][count]+ log(sumnum/sumden);
	    }
	  }
	  count++;
	}
      }
    }

    fclose(dateshingresultinptr);
    /* writing in a file the results of docprod, */ 
    /* the probability of the documents' occurrence as a function of time*/
    /* fprintf(docprobfileoutptr,"%d\t", seqline); */
    aveprob=0; 
    for(i=0; i<=6; i++){
      aveprob=aveprob+docprob[sampleseqdata[i]-1][time[i]];
      /* aveprob=aveprob+((Nti[sampleseqdata[i]-1])/bottomsum)*docprob[sampleseqdata[i]-1][time[i]];*/
      /* printf("%d \%d \n", sampleseqdata[i], time[i]); */
      /* printf("%.12f \n", aveprob/7); */
      printf("%.5f  ", docprob[sampleseqdata[i]-1][time[i]]);
    }
    printf("\n");
    fprintf(docprobfileoutptr, "%.12f\t", aveprob); 
    fprintf(docprobfileoutptr,"\n"); 
    
    seqline++;
  }
  fclose(sampleseqfileinptr);
  fclose(docprobfileoutptr);
}

















































