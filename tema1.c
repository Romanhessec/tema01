#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

Dir* allocDir(char* name, struct Dir* parent, struct File* head_children_files, 
	struct Dir* head_children_dirs, struct Dir* next){

	Dir* thisDir = (Dir*) malloc(sizeof(Dir));
	thisDir->name = malloc(MAX_INPUT_LINE_SIZE);
	thisDir->parent = (Dir*) malloc(sizeof(Dir));
	thisDir->head_children_files = (File*) malloc(sizeof(File));
	thisDir->head_children_dirs = (Dir*) malloc(sizeof(Dir));
	thisDir->next = (Dir*) malloc(sizeof(Dir));

	strcpy(thisDir->name, name);
	thisDir->parent = parent;
	thisDir->head_children_dirs = head_children_dirs;
	thisDir->head_children_files = head_children_files;
	thisDir->next = next;

	return thisDir;

}

File* allocFile(char* name, struct Dir* parent, struct File* next){
	
	File* thisFile = (File*) malloc(sizeof(File));
	thisFile->name = malloc(MAX_INPUT_LINE_SIZE);
	thisFile->parent = (Dir*) malloc(sizeof(Dir));
	thisFile->next = (File*) malloc(sizeof(File));

	strcpy(thisFile->name, name);
	thisFile->parent = parent;
	thisFile->next = next;

	return thisFile;
}

void touch (Dir* parent, char* name) {

	File* thisFile = allocFile(name, parent, NULL);


	if (parent->head_children_files == NULL){
		parent->head_children_files = thisFile;
		return;
	}

	File* toIterate = parent->head_children_files;
		
	while(1){

		if (strcmp(toIterate->name, name) == 0){

			puts("File already exists");
			return;
		}

		if (toIterate->next == NULL)
			break;
		toIterate = toIterate->next;
	}
		
	toIterate->next = thisFile;
	
}

void mkdir (Dir* parent, char* name) {

	Dir* thisDir = allocDir(name, parent, NULL, NULL, NULL);
	
	if (parent->head_children_dirs == NULL){

		parent->head_children_dirs = thisDir;
		return;
	}

	Dir* toIterate = parent->head_children_dirs;

	while (1){

		if (strcmp(toIterate->name, name) == 0){

			puts("Directory already exists");
			return;
		}

		if (toIterate->next == NULL)
			break;
		toIterate = toIterate->next;
	}

	toIterate->next = thisDir;
	
}

void ls (Dir* parent) {

	Dir* toIterateDirs = parent->head_children_dirs;
	
	if (toIterateDirs != NULL){

		while(1){

			puts(toIterateDirs->name);
			toIterateDirs = toIterateDirs->next;
			if (toIterateDirs == NULL)
				break;
		}
	}

	File* toIterateFiles = parent->head_children_files;

	if (toIterateFiles != NULL){
		
		while(1){

			puts(toIterateFiles->name);
			toIterateFiles = toIterateFiles->next;
			if (toIterateFiles == NULL)
				break;
		}
	}

}

void rm (Dir* parent, char* name) {

	if (parent->head_children_files == NULL){

		puts("Could not find the file");
		return;
	}

	File* toIterate = parent->head_children_files;
	File* toIteratePrev = parent->head_children_files;
	
	while (1){

		if (strcmp(toIterate->name, name) == 0){

			if (toIteratePrev == toIterate){ //cazul in care avem primul fisier din lista

				if (toIterate->next == NULL)
					parent->head_children_files = NULL;
				else
					parent->head_children_files = toIterate->next;

				return;
			}

			if (toIterate->next == NULL){

				toIteratePrev->next = NULL;
				return;
			}
			else {

				toIteratePrev->next = toIterate->next;
				//+dealoc;
				return;
			}
			return; //just 2 be safe :D
		}

		if (toIterate->next == NULL){

			puts("Could not find the file");
			return;
		}

		toIteratePrev = toIterate;
		toIterate = toIterate->next;
	}	

}

void rmdir (Dir* parent, char* name) {

	if (parent->head_children_dirs == NULL){

		puts("Could not find the dir");
		return;
	}

	Dir* toIterate = parent->head_children_dirs;
	Dir* toIteratePrev = parent->head_children_dirs;
	
	while (1){

		if (strcmp(toIterate->name, name) == 0){

			if (toIteratePrev == toIterate){ //cazul in care avem primul fisier din lista

				if (toIterate->next == NULL)
					parent->head_children_dirs = NULL;
				else
					parent->head_children_dirs = toIterate->next;

				return;
			}

			if (toIterate->next == NULL){

				toIteratePrev->next = NULL;
				return;
			}
			else {

				toIteratePrev->next = toIterate->next;
				//+dealoc;
				return;
			}
			return; //just 2 be safe :D
		}

		if (toIterate->next == NULL){

			puts("Could not find the dir");
			return;
		}

		toIteratePrev = toIterate;
		toIterate = toIterate->next;
	}

}

void cd(Dir** target, char *name) {

	Dir* toIterateDirs = (*target)->head_children_dirs;
	
	if (strcmp(name, "..") == 0){

		if ((*target)->parent == NULL)
			return;

		*target = (*target)->parent;
		return;
	}

	if (toIterateDirs == NULL){

		puts("No directories found!");
		return;
	}

	while (1){

		if (strcmp(toIterateDirs->name, name) == 0){

			*target = toIterateDirs;
			return;
		}

		if (toIterateDirs->next == NULL){

			puts("No directories found!");
			return;
		}

		toIterateDirs = toIterateDirs->next;
	}
}

char *pwd (Dir* target) {}

void stop (Dir* target) {}

void tree (Dir* target, int level) {}

void mv(Dir* parent, char *oldname, char *newname) {}

int main () {

	char* input = malloc(MAX_INPUT_LINE_SIZE);
	Dir* home = allocDir("home", NULL, NULL, NULL, NULL);
	Dir* currentDir = home;

	do
	{	
		char* token = malloc(MAX_INPUT_LINE_SIZE);

		fgets(input, MAX_INPUT_LINE_SIZE, stdin);
		input[strcspn(input, "\n")] = 0; //delete trailing \n;


		if (strstr(input, "touch") != NULL){
			
			token = strtok(input, " ");
			token = strtok(NULL, " ");

			touch(currentDir, token);
		}
		

		if (strstr(input, "mkdir") != NULL){

			token = strtok(input, " ");
			token = strtok(NULL, " ");

			mkdir(currentDir, token);
		}

		if (strstr(input, "ls") != NULL)
			ls(currentDir);

		if (strstr(input, "rm") != NULL){

			if (strstr(input, "rmdir") != NULL){ //confusion
			
				token = strtok(input, " ");
				token = strtok(NULL, " ");
				
				rmdir(currentDir, token);	
			}
			else {
				token = strtok(input, " ");
				token = strtok(NULL, " ");

				rm(currentDir, token);
			}
		}

		if (strstr(input, "cd") != NULL){

			token = strtok(input, " ");
			token = strtok(NULL, " ");
			cd(&currentDir, token);
		}



		if (strcmp(input, "stop") == 0)
			break;

	} while (1);
		
	free(input);
	//free home;
	//free currentDir;
	return 0;
}
