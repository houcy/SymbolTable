#include "IOMngr.h"

int main() {
	bool test1 = OpenFiles("sourcefile.txt", NULL);

	
	if( test1 == true ) {
		char testChar = GetSourceChar();
		while( testChar != '\0') {
			if( testChar == '{' ) {
				WriteIndicator( GetCurrentColumn() );
				WriteMessage("Invalid Character");
			}
			testChar = GetSourceChar();
		}
	}

	CloseFiles();

	return 0;
}