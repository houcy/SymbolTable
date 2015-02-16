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
			return '\0';

		char* FgetsReturn = NULL;
		FgetsReturn = fgets(buffer, MAXLINE, SourceFile);
		if( FgetsReturn == NULL )
			return '\0';

        if( IsListingFile == true ) {
            fprintf(ListingFile, "%d. %s", ++currRow, buffer);
        } else {
            printf("%d. %s", ++currRow, buffer);
        }

		FirstRead = false;
	}
	else if( strlen(buffer) == currCol) {
		if( 0 != feof(SourceFile) )
			return '\0';

		char* FgetsReturn = NULL;
		FgetsReturn = fgets(buffer, MAXLINE, SourceFile);
		if( FgetsReturn == NULL )
			return '\0';

        if( IsListingFile == true ) {
            fprintf(ListingFile, "%d. %s", ++currRow, buffer);
        } else {
            printf("%d. %s", ++currRow, buffer);
        }

		currCol = 0;
	}
	return buffer[currCol++];
}

void WriteIndicator(int AColumn) {
	int i;
	for( i = 0; i < AColumn + 2; ++i ) {
        if( IsListingFile == true ) 
		  fprintf(ListingFile, " ");
        else
            printf(" ");
	}
    if( IsListingFile )
	   fprintf(ListingFile, "^\n");
    else
        printf("^\n");

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