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
  SN = atoi(argv[4]);

  /* initialize variables */

  currentstring = malloc(10 * sizeof(char));
  shingledsuffix = ".shingled";
  systemcommandbase = "sort  ";
  systemcommandoutputdirect = " -o ";

  /* the outer for loop just goes through the shingling procedure once for each of the files in the list. 
     we are, of course, assuming that if N is entered as the number of files then the files
     basename0001, ..., basenameNshingler.c actually exist.  if not, the file error message gets printed. */

  for (currentfilenumber = firstfilenumber; currentfilenumber <= lastfilenumber; currentfilenumber++) {
         /* we begin by putting together the name of the current file, created from the file name base
	 given by the user by appending the string corresponding to the number of the currentfile. */
      
      inttostring(currentfilenumber, currentstring);
    
      strcpy(currentfilename, basefilename);
      strcat(currentfilename, currentstring);
      
      /* printf("currentfilename is %s \n", currentfilename); */
      
      /* now we actually open the file. */
      
      readptr = fopen(currentfilename, "r"); 
      
      /* printf(" the char value is = %c \n", &readptr ); */
      
      if (readptr == NULL) {
      
	printf("error with input file.\n");
	
      } else {
	
	/* assuming that all went well with the file opening, we now will go through the file and place
	   the words of which it consists into our linked list, one word at a time.  so the current
	   node always keeps track of the next node to be written in---which is why it starts at the
	   first node in the list, `listofwords'. */
	
	currentnode = listofwords;
	
	while ( !feof(readptr) ) {
	  /* read a word from the file into the current node, */
	  
	  fscanf(readptr, "%s", currentnode->word);
	  
	  /* now allocate space for the nextnode and set the currentnode to be
	     the new node. */
	  
	  currentnode->nextnode = malloc(sizeof(struct node));
	  currentnode = currentnode->nextnode;
	  
	  /* and now initialize the new node. */
	  
	  currentnode->word = malloc(sizeof(char *));
	  currentnode->nextnode = malloc(sizeof(struct node));
	  
	}

	/* once we have read in all the words, we want the list to terminate.  the indicator that
	   we're on the last node of the list is that the pointer to the next node is NULL, so
	   this is how we define the pointer of the current node. */
	
	currentnode->nextnode = NULL;
	
	fclose(readptr);
	
	/* now we reset the currentnode pointer to point to the beginning of the list of words. */
	
	currentnode = listofwords;
	
	/* this while loop just prints to the standard output the list of words, if anyone is interested. */
	
	/* while( currentnode->nextnode != NULL) {
	   printf("%s\n", currentnode->word);
	   currentnode = currentnode->nextnode;
	   } */
	
	/* now we create the output (shingled) filename, in a similar way to the input file name but
	   with the additional shingled prefix added on. */
	
	strcpy(currentshingledfilename, basefilename);
	strcat(currentshingledfilename, currentstring);
	strcat(currentshingledfilename, shingledsuffix);
	
	/* and open the file. */

	writeptr = fopen(currentshingledfilename, "w");
	
	if (writeptr == NULL) {
	  printf("error opening output file.\n");
	} 
	else {
	
	  /* now we go through the list, one word (node) at a time.  at each node, we print out the next
	     SN (shingle number) of words to the file (this is the inner for loop), creating a shingle. 
	     after completing this we set currentnode to the node on the list which is immediately
	     after the node the current shingle started at, print a carriage return to the file in order to separate
	     the shingles from each other, and continue.  after we have written the entire file we
	     close the data stream.  */
	  
	  while( lookaheadlist(currentnode, SN +1) != NULL) {
	    thenextnode = currentnode->nextnode;
	    for (i=1; i<= SN; i++) {
	      fprintf(writeptr, "%s ", currentnode->word);
	      currentnode = currentnode->nextnode;
	    }
	    fprintf(writeptr, "\n");
	    currentnode = thenextnode;
	  }
	  
	  fclose(writeptr);
	}
      }
      
      /* now all that remains before starting the next file is to free up the memory contained
	 in the linked list, so that it can be used again for the next file. */
      
      FreeList ( listofwords );
      
      /* the following lines perform the unix command `sort -u' on the newly shingled file, which sorts the
	 files alphabetically and guarantees that each shingle occurs only once in the file.  this is important 
	 because of the way the comparer program works. */
      
      strcpy(systemcommand, systemcommandbase);
      /* printf("systemcommand1=%s \n", systemcommandbase); */
      
      strcat(systemcommand, currentshingledfilename);
      /* printf("systemcommand2=%s \n", systemcommand); */
      
      strcat(systemcommand, systemcommandoutputdirect);
      /* printf("systemcommand3=%s \n", systemcommand); */
      
      strcat(systemcommand, currentshingledfilename);
      /* printf("systemcommand4= %s \n", systemcommand); */
      system(systemcommand); 
      
      printf("shingled file number %d\n", currentfilenumber);
      
       
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
