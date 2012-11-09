/* the purpose of this program is to merge all the file of the training documents 
   into one where the 1st column is the word and the 2nd is the date in which the word is
   written */ 


#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<string.h>
#include </usr/include/math.h>



/* ******************************************************************************************** */


main(int argc, char *argv[]) {

  FILE *dateinfileptr;
  FILE *currentfileptr;
  FILE *resultoutfileptr;
  
  char dateinfile[40];
  char resultfile[40];
  char *basefilename="/home/gelila/computation/order/";
  char *suffix=".shingled";
  char *at="@";

  char *filenamearray[9911];
  char filename[25];
  char currentshingle[200];
  char currentshingle1[200];
  char currentshingle2[200];
  char currentfilename[100];
  char currentfilenamenew[100];

  int datemasterarray[9911];
  int traindate; /* date of the training dcouments */

  int numberoffile=9911;
  int singledate, Ldate, Hdate, datediff;

  char id[9];
  int i,j, count,date;


  strcpy(dateinfile, argv[1]); /* aaawmaster */
  strcpy(resultfile, argv[2]); /*doc1 doc2 etc... */

  for(i=0; i<numberoffile; i++){
    filenamearray[i]= malloc(25*sizeof(char));
  }	


  /* reading and loading into an array the dates of the documents */
  printf("%s\t%s\t\n", dateinfile, resultfile);

  dateinfileptr=fopen(dateinfile,"r");
  i=0;
  while(i<=numberoffile-1){
    fscanf(dateinfileptr, "%s %s %d %d %d %d", id, filename, &singledate, &Ldate, &Hdate, &datediff);
    datemasterarray[i]=singledate; 
    if(singledate==0){
      datemasterarray[i]=Ldate;
    }
    printf("id is %s, filename is %s \n", id, filename);
    strcpy(filenamearray[i],filename);
    /* printf("%d %s \n", i,filenamearray[i]); */
    i++;
  }
  fclose(dateinfileptr);
 
  
  printf("i'm here \n");
  /* loading in the date of the training documents */ 
    
  /* printf("\n\n\n\n\n\n\n\ni'm here\n");*/
  resultoutfileptr=fopen(resultfile, "w");
  i=1;
  while(i<=numberoffile){
    traindate=datemasterarray[i-1];
    if( /* (traindate==1151)||(traindate==1152)||(traindate==1153)||(traindate==1154)||
	   (traindate==1155)||(traindate==1156)){*/

       /* (traindate==1194)||(traindate==1195)|| (traindate==1196)){*/

       /* (traindate==1223)){*/

       /* (traindate==1251)){ */

       /* (traindate==1277)){ */

       /* (traindate==1300)||(traindate==1301)){*/

       (traindate==1333)||(traindate==1334)||(traindate==1335)||(traindate==1337)||(traindate==1338)||(traindate==1339)||(traindate==1340)||(traindate==1341)){

      /*printf("%s \n", filenamearray[i-1]);*/
      strcpy(currentfilenamenew, basefilename);
      strcat(currentfilenamenew, filenamearray[i-1]);
      strcpy(currentfilename, currentfilenamenew);
      strcat(currentfilename, suffix);  /* reads in files such as aaaw0001.shingled */
      printf("%d %s \n", i, currentfilename); 
      currentfileptr = fopen(currentfilename,"r");
      while(!feof(currentfileptr)){   
	fscanf(currentfileptr, "%s %s", currentshingle1, currentshingle2);
	strcat(currentshingle1, at);
	strcpy(currentshingle, currentshingle1);
	strcat(currentshingle, currentshingle2);
	if( !feof(currentfileptr) ) {
	  fprintf(resultoutfileptr, "%s %d\n", currentshingle, 7);
	}
      }
      fclose(currentfileptr);
    }
    i++;
  }
  fclose( resultoutfileptr );
 
  
}







