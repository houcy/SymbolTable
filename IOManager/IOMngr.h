#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 1024

//Functions
/*
  Open the source file whose name is given in ASourceName. 
  Returns True on successful.
*/
bool OpenFiles(const char* ASourceName, const char* AListingName);

/* 
  Close the source file and the listing file if one was created 
*/
void CloseFiles();

/*
  Return the next source char.  Echo the lines in the source file
  to the listing file.  Returns EOF when the end of source file is reached.
*/
char GetSourceChar();

/*
  Writes a line containing a single '^' char in the indicated column.  If
  no listing file, it is echoed to stdout.
*/
void WriteIndicator(int AColumn);

/*
  Write the message on a separate line.
*/
void WriteMessage(const char* AMessage);

/*
  Return the current line number.
*/
int GetCurrentLine();

/*
  Return the column number of the most recently returned line.
*/
int GetCurrentColumn();