/* NOTE, ON LINE 76 "sort" USE TO BE "sort -u" WHICH WAS NEEDED, APPARENTLY FOR 
   "comparer" PROGRAM. WE NOW HAVE IT CHANGED TO JUST "sort" */

/* this program shingles a bunch of files with sequential titles, like base0001, base0002, ..., base1234. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include </usr/include/math.h>

struct node *lookaheadlist(struct node *, int);
void inttostring( int n, char *);
void InitializeList ( struct node * );
void FreeList ( struct node * );

/* the basic structure is a linked list, whose nodes are just called `node'.  a node consists of a word
   (pointer to character), plus a pointer to the next node.  as the name suggests, the `word's are intended
   to contain one word each, although they could hold strings with many words. */

struct node {
  char *word;
  struct node *nextnode;
};

/* the syntax of the program is as follows.  if the executable file is called `shingler' then the command
           shingler basename N1 N2 n
   will take files basenameN1, basename(N1+1), ..., basenameN2 and shingle them into shingles of n words,
   placing the results into files basenameN1shingled, basename(N1+1)shingled, ..., basenameN2shingled. */
   
main( int argc, char *argv[] )

{
  struct node *listofwords;
  struct node *currentnode;
  struct node *thenextnode;

  char *filechar; 
  char *basefilename; 
  char *currentstring;
  char *currentword;
  char *shingledsuffix;
  char *loopptr;
  char *systemcommandbase;
  char *systemcommandoutputdirect;

  char currentfilename[30];
  char currentshingledfilename[30];
  char systemcommand[40];

  int i, SN, firstfilenumber, lastfilenumber;
  int currentfilenumber;
 
  FILE *readptr;
  FILE *writeptr;

  /* we begin by defining the variable `listofwords', which is a pointer to the first node in the
     linked list which will be set up later.  after defining the variable, we use malloc to
     initialize the node it points to, that is clearing space for the things which will eventually
     be put there. */

  listofwords = malloc(sizeof(struct node));

  InitializeList ( listofwords );

  /* now we take the variables controlling the run of the program from the input of the user. */

  basefilename = argv[1];
  firstfilenumber = atoi(argv[2]);
  lastfilenumber = atoi(argv[3]);
  /*SN = atoi(argv[4]);*/

  /* initialize variables */

  currentstring = malloc(10 * sizeof(char));
  shingledsuffix = ".shingled";
  systemcommandbase = "wc -l ";
  systemcommandoutputdirect = " -o ";

  /* the outer for loop just goes through the shingling procedure once for each of the files in the list. 
     we are, of course, assuming that if N is entered as the number of files then the files
     basename0001, ..., basenameNshingler.c actually exist.  if not, the file error message gets printed. */

  for (currentfilenumber = firstfilenumber; currentfilenumber <= lastfilenumber; currentfilenumber++) {
         /* we begin by putting together the name of the current file, created from the file name base
	 given by the user by appending the string corresponding to the number of the currentfile. */
      
      inttostring(currentfilenumber, currentstring);
      
      /*printf("%d\t%s", currentfilenumber, currentstring);*/

      strcpy(currentfilename, basefilename);
      strcat(currentfilename, currentstring);
            
      strcpy(systemcommand, systemcommandbase);
      /* printf("systemcommand1=%s \n", systemcommandbase); */
      
      strcat(systemcommand, currentfilename);
      /* printf("systemcommand2=%s \n", systemcommand); */
 
      system(systemcommand); 
       
  }
}


  /* this function returns a pointer to the node which is n ahead of the current node in the
     linked list, assuming that the list continues for at least n nodes.  it is not clear
     what happens if this is not the case, however the situation should never come up. */
  
  struct node *lookaheadlist(struct node *nodeptr, int n) {
  int i;
  struct node *currentptr;
  
  currentptr = nodeptr;

  /* we just go through the list, one node at a time, until we have gone n steps.  then
     return the relevant pointer. */

  for (i=1; i<=n; i++) {
    currentptr = currentptr->nextnode;
  }

  return currentptr;
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

void InitializeList ( struct node *listptr ) {

  listptr->word = malloc(sizeof(char *));
  listptr->nextnode = malloc(sizeof(struct node *));

  /* now we set the contents of the first node of the list.  since `word' is a pointer
     to character (ie an array of characters or a word) we set the contents of this
     to be 0, representing an empty string.  also we set the pointer to the next node
     to be NULL, indicating that the first node of the list is also the last. */

  *(listptr->word) = 0;
  listptr->nextnode = NULL;

}

void FreeList ( struct node *listptr ) {
  
  struct node *currentptr;
  struct node *previousptr;

  currentptr = listptr->nextnode;

  while ( currentptr != NULL ) {
    previousptr = currentptr;
    currentptr = currentptr->nextnode;
    free(previousptr);
  }

  free(currentptr);
  
}
