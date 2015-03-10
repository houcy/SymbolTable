#include "IOMngr.h"
FILE* SourceFile = NULL;
FILE* ListingFile = NULL;
bool IsListingFile = false;
bool FirstRead = true;
int currCol = 0;
int currRow = 0;
char buffer[MAXLINE];

bool OpenFiles(const char* ASourceName, const char* AListingName) {
    SourceFile = fopen(ASourceName, "r");

    //open ListingFile if argument was given.
    if( AListingName != NULL ) {
        ListingFile = fopen(AListingName, "w");
        if( ListingFile != NULL )
            IsListingFile = true;
    }

    //If no SourceFile, return false.
    if( SourceFile == NULL )
        return false;

    return true;
}

void CloseFiles() {
    fclose(SourceFile);

    if( IsListingFile ) {
        fclose(ListingFile);
        IsListingFile = false;
    }
}

char GetSourceChar() {
    if( FirstRead == true ) {
        if( 0 != feof(SourceFile) ) 
            return EOF;

        char* FgetsReturn = NULL;
        FgetsReturn = fgets(buffer, MAXLINE, SourceFile);
        if( FgetsReturn == NULL )
            return EOF;

        if( IsListingFile == true ) {
            fprintf(ListingFile, "%d. %s", ++currRow, buffer);
        } else {
            currRow++;
        }

        FirstRead = false;
    }
    else if( strlen(buffer) == currCol) {
        if( 0 != feof(SourceFile) )
            return EOF;

        char* FgetsReturn = NULL;
        FgetsReturn = fgets(buffer, MAXLINE, SourceFile);
        if( FgetsReturn == NULL )
	return EOF;

        if( IsListingFile == true ) {
            fprintf(ListingFile, "%d. %s", ++currRow, buffer);
        } else {
            currRow++;
        }

        currCol = 0;
    }
    return buffer[currCol++];
}

void WriteIndicator(int AColumn) {
    int i;
    if( !(IsListingFile) )
        printf("%d. %s\n", currRow, buffer);
    else
        fprintf(ListingFile, "\n");

    for( i = 0; i < AColumn + 3; ++i ) {
        if( IsListingFile ) 
            fprintf(ListingFile, " ");
        else
            printf(" ");
    }
    if( IsListingFile )
        fprintf(ListingFile, "^\n");
    else {
        printf("^\n");
    }


}

void WriteMessage(const char* AMessage) {
    if( IsListingFile == true ) {
        fprintf(ListingFile, "%s\n", AMessage);
    }
    else {
        printf("%s\n", AMessage);
    }
}

int GetCurrentLine() {
    return currRow;
}

int GetCurrentColumn() {
    return currCol;
}