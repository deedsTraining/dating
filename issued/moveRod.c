

/* this program moves a bunch of files to a sequential titles, like aaaaw0001, aaaaw0002, ..., aaaaw1234. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include </usr/include/math.h>


void inttostring( int n, char *);



/* the syntax of the program is as follows.  if the executable file is called `shingler' then the command
           shingler basename N1 N2 n
   will take files basenameN1, basename(N1+1), ..., basenameN2 and shingle them into shingles of n words,
   placing the results into files basenameN1shingled, basename(N1+1)shingled, ..., basenameN2shingled. */
   
main( int argc, char *argv[] )

{

  
  char *basefilenewname;
  char *currentstring;
  char *aaawmasterstring;
  char *aaawmasterfilename="aaawmaster";

  char currentfilename[60];
  char systemcommand[40];
  char copyfilename[50];
  char id[8]; 
  char filename[8];

  int i, firstfilenumber, lastfilenumber;
  int currentfilenumber, datediff, singledate, lowdate, highdate;
 
  
  FILE *copyfileinptr;
  FILE *aaawmasterfileoutptr;

  /* now we take the variables controlling the run of the program from the input of the user. */

  basefilenewname = argv[1]; /* aaaw */
  firstfilenumber = atoi(argv[2]); /*1 */
  lastfilenumber = atoi(argv[3]); /*10837 */
  strcpy(copyfilename, argv[4]); /* aawmasterRodnewsort */
  /* initialize variables */

  currentstring = malloc(12 * sizeof(char));
   
  /* the outer for loop just goes through the shingling procedure once for each of the files in the list. 
     we are, of course, assuming that if N is entered as the number of files then the files
     basename0001, ..., basenameNshingler.c actually exist.  if not, the file error message gets printed. */


  copyfileinptr= fopen(copyfilename, "r");
  aaawmasterfileoutptr=fopen(aaawmasterfilename, "w");

  for (currentfilenumber = firstfilenumber; currentfilenumber <= lastfilenumber; currentfilenumber++) {
         /* we begin by putting together the name of the current file, created from the file name base
	 given by the user by appending the string corresponding to the number of the currentfile. */
      
    fscanf(copyfileinptr, "%s %d %d %d %d", id, &singledate, &lowdate, &highdate, &datediff);
    inttostring(currentfilenumber, currentstring);
    
    strcpy(currentfilename, basefilenewname);
    strcat(currentfilename, currentstring);
   
    printf("currentfilename is %s \n", currentfilename); 
      
    /* now we actually open the file. */
    
    fprintf(aaawmasterfileoutptr, "%s  %s  %d  %d  %d  %d \n", id, currentfilename,  singledate, lowdate, highdate, datediff);

  } 
	
  fclose(copyfileinptr);
  fclose(aaawmasterfileoutptr);
	
}

/* this function converts an integer to a string which gives its decimal notation. */
   
void inttostring( int n, char *array ) {
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

