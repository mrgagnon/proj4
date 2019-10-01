// Maylee Gagnon 
// 9.30.2019
// Project4

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>

#define  MAXBUFF 1024

/* Checks to see if a regular file contains all text  
   returns  true if file contains all text
	    false otherwise 
*/
bool isAllText(char* fileName){ // ??? add param for file  
	int maxbuff = 1024; 
	char buf[maxbuff];
	int fdIn, cnt, i;
	
	if((fdIn = open(fileName, O_RDONLY)) == -1){ 
		fprintf(stderr, "Error: file could not be open\n");
		return false;
	}

	while((cnt = read(fdIn,buf, maxbuff)) > 0){	 
		for(i = 0; i < cnt; i++) {					 
			if(!isprint(buf[i]) && !isspace(buf[i])){
				fprintf(stderr, "Non-printable character\n");
				return false; 
			}
		}
	}
	return true; // end of file reached without problems so file contains all text 
	close(fdIn);
}//endisAllText

main(){
	fprintf(stderr, "In main\n");
	char* inputLine;
	char line[MAXBUFF];

	int numBadFiles = 0;
	int numDir = 0;
	int numRegFiles	 = 0;
	int numSpecFiles = 0;
	long long numBytesReg = 0;
	int numAllText = 0;
	long long numBytesText = 0;

	while(1) {
		struct stat statBuff;
		inputLine = fgets(line,MAXBUFF, stdin);
	
		// Trimming new line char and replacing with terminator
		size_t len = strlen(line) -1;
		if(*line && line[len] == '\n') {
			line[len] = '\0';
		}
	
		if(inputLine == NULL) {
			fprintf(stderr, "End of file reached\n");
			break;
		}
		if (line[0] == '\n'){
			fprintf(stderr, "Empty line, re-enter\n");
			continue;
		}
	
		if(stat(inputLine, &statBuff) == -1) {
			numBadFiles++;
			continue;
		} 
	
		if(S_ISDIR(statBuff.st_mode)) {
			numDir++;
			continue;
		}
		if (S_ISREG(statBuff.st_mode)) {
			numRegFiles++;
			long long size = statBuff.st_size;
			numBytesReg += size; 
			if(isAllText(inputLine)) { 
				numAllText++;
				numBytesText += size;
			} 	
		}
		if(!S_ISDIR(statBuff.st_mode) && !S_ISREG(statBuff.st_mode)){
			numSpecFiles++;
		}
	}// end while(1)

	fprintf(stderr, "Bad Files: %d\n", numBadFiles);
	fprintf(stderr, "Directories: %d\n", numDir);
	fprintf(stderr, "Regular Files: %d\n", numRegFiles);
	fprintf(stderr, "Special Files: %d\n", numSpecFiles);
	fprintf(stderr, "Regular Files Bytes: %lld\n", numBytesReg);
	fprintf(stderr, "Text Files: %d\n", numAllText);
	fprintf(stderr, "Text File Bytes: %lld\n", numBytesText);

	return 0;
} // end main()
