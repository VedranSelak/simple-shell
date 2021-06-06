#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>


int main(int argc, char *argv[]){
	printf("Welcome to my shell!\n\n");
	printf("This shell contains the follwing commands: \n\n");
	printf("count c <filename> This command prints the count of characters in the file! \n\n");
	printf("count w <filename> This command prints the count of words in a file! \n\n");
	printf("count l <filename> This command prints the count of lines in a file! \n\n");
	printf("count <word/character> <filename> This command prints the count of occurences of a given character/word in a file (characters are case insensitive, while words are case sensitive) !\n\n");
	printf("format <filename> This command prints the formated text in a camel case manner!\n\n");
	printf("lower <filename> This command prints text from a file in lowercase!\n\n");
	printf("upper <filename> This command prints text form a file in uppercase!\n\n");
	printf("ctrl + C This commanf exits the program!\n\n");

	while(1){
		// Taking command from the user
		char input[300];
		printf("<prompt>");
		fgets(input, sizeof(input), stdin);
		// Initializing the array of strings to hold the separate words of the command
		char command[10][60];
		int i,j,ctr;
		j = 0;
		ctr = 0;
		// Separating words from the input and adding them into the array of strings
		for(i = 0; i<= strlen(input); i++){
			if(input[i] == ' ' || input[i] == '\0'){
				command[ctr][j] = '\0';
				ctr++;	
				j = 0;
			} else {
				command[ctr][j] = input[i];
				j++;
			}
		}


		// Making a child process to run the commands
		int rc = fork(); 
		
		// If fork returns negative int than display error
		if(rc < 0) {
			fprintf(stderr, "fork failed\n");
			exit(0);
		} else if (rc == 0) {
			char *args[5];
			if(strcmp(command[0], "count") == 0){
				char str[255];
				FILE *r;
				// Switch case that covers all possibilities of the count command
				switch(command[1][0]){
					case 'c':
						// Adding the proper command to args
						args[0] = strdup("wc");
						args[1] = strdup("-m");
						int len = strlen(command[2]);
						// Removing the \n from the command
						if(command[2][len-1] == '\n'){
							command[2][len-1] = 0;
						}
						args[2] = strdup(command[2]);
						args[3] = NULL;
						// Executing the command
						execvp(args[0], args);
						break;
					case 'w':
						args[0] = strdup("wc");
						args[1] = strdup("-w");
						len = strlen(command[2]);
						if(command[2][len-1] == '\n'){
							command[2][len-1] = 0;
						}
						args[2] = strdup(command[2]);
						args[3] = NULL;
						execvp(args[0], args);
						break;
					case 'l':
						args[0] = strdup("wc");
						args[1] = strdup("-l");
						len = strlen(command[2]);
						if(command[2][len-1] == '\n'){
							command[2][len-1] = 0;
						}
						args[2] = strdup(command[2]);
						args[3] = NULL;
						execvp(args[0], args);
						break;
					default:
						// If no cases are satisfied this means we are dealing with a count command
						// that counts the number of occurences of a character/word in a given document
						len = strlen(command[2]);
						if(command[2][len-1] == '\n'){
							command[2][len-1] = 0;
						}
					
						r = fopen(command[2], "r");
		
						if(r == NULL){
							puts("Input file cannot be oppend");
						}
						// Counter for counting the occurence of a letter
						int count = 0;
						char **lines;
						while(1){
							if(fgets(str, 255, r) == NULL)
								break;
							else{
								if(strlen(command[1]) == 1){
									// For loop for counting the number of occurences of a letter in a given file
									for(int j=0; j<strlen(str); j++){
										if(tolower(str[j]) == tolower(command[1][0])){
											count++;
										}
									}
								} else {
									// If the user is counting the number of occurences of a word 
									
									lines = malloc(100 * sizeof(char*));
									
									i = 0;
									int wi = 0;
									char *word = malloc(25);
									// Seperating the words of a line in the file
									for(int j=0; j<strlen(str); j++){
										if(str[j] == ' ' || str[j] == '\n'){
											lines[i] = malloc(25);
											strcpy(lines[i], word);
											wi = 0;
											i++;
											memset(word, 0, 25);
										} else {
											word[wi] = str[j];
											wi++;
										}
									}
									i = 0;
									// Counting the occurrence of the word
									while(&lines[i][0] != NULL){
										if(strstr(&lines[i][0], command[1])){
											count++;
										}
										i++;
									}

								}
							
							}	
						}
						printf("%d\n", count);
						fclose(r);
						break;
				}
			// Execution of lower
			}else if(strcmp(command[0], "lower") == 0){
				FILE *fr;
				char str[255];
				char **lines;
				int len = strlen(command[1]);
				if(command[1][len-1] == '\n'){
					command[1][len-1] = 0;
				}
				
				// Open file for reading
				fr = fopen(command[1], "r");
		
				if(fr == NULL){
					puts("Input file cannot be oppend");
				}

				int i = 0;
				// Initializing an array to hold the lines of the file
				lines = malloc(100 * sizeof(char*));
				while(1){
					if(fgets(str, 255, fr) == NULL)
						break;
					else{
						// Converting every character to lowercase
						for(int j=0; j<strlen(str); j++){
							str[j] = tolower(str[j]);
						}
						
						// Adding the lowercase string to the array
						lines[i] = malloc(255);

						strcpy(lines[i], str);
					}
					i++;		
				}
				fclose(fr);
				i = 0;
				// Printing the result to the user
				while(&lines[i][0] != NULL){
					printf("%s", &lines[i][0]);
					i++;
				}
			// Execution of upper
			} else if(strcmp(command[0], "upper") == 0){
				char str[255];
				FILE *fr;
				char **lines;
				int len = strlen(command[1]);
				if(command[1][len-1] == '\n'){
					command[1][len-1] = 0;
				}
				fr = fopen(command[1], "r");
				if(fr == NULL){
					puts("Input file cannot be oppend");
				}
				int i = 0;
				lines = malloc(100 * sizeof(char*));
				while(1){
					if(fgets(str, 255, fr) == NULL)
						break;
					else{
						for(int j=0; j<strlen(str); j++){
							str[j] = toupper(str[j]);
						}
					
						lines[i] = malloc(255);

						strcpy(lines[i], str);
					}
					i++;		
				}
				fclose(fr);
				i = 0;
				while(&lines[i][0] != NULL){
					printf("%s", &lines[i][0]);
					i++;
				}	
			// Execution of format		
			} else if (strcmp(command[0], "format") == 0){
				char str[255];
				FILE *fr;
				char **lines;
				int len = strlen(command[1]);
				if(command[1][len-1] == '\n'){
					command[1][len-1] = 0;
				}
				fr = fopen(command[1], "r");
				if(fr == NULL){
					puts("Input file cannot be oppend");
				}
				int i = 0;
				lines = malloc(100 * sizeof(char*));
				while(1){
					if(fgets(str, 255, fr) == NULL)
						break;
					else{
						for(int j=0; j<strlen(str); j++){
							if(str[j] == ' ' || str[j] == '-'){
								// Removing the whitespace character
								str[j+1] = toupper(str[j+1]);
							       	memmove(&str[j], &str[j+1], strlen(str) - j);	
							} else if(j == 0){
								// Making sure that first letter is in uppercase
								str[j] = toupper(str[j]);
							} else {
								str[j] = tolower(str[j]);
							}
						}
						
						// Storing the formated string
						lines[i] = malloc(255);

						strcpy(lines[i], str);
					}
					i++;		
				}
				fclose(fr);
				i = 0;
				// Printing the result to the user
				while(&lines[i][0] != NULL){
					printf("%s", &lines[i][0]);
					i++;
				}

			} else {
				printf("Wrong command!\n");
			}
		} else {
			wait(NULL);
		}
	}
	return 0;

}

