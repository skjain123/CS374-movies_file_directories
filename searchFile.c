#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "searchFile.h"

/**
 * Name: searchLargestFile
 * Input: none
 * Output: a string 
 * PreConditions: no
 * PostConditions: string contains the file name of largest file
 * Comment: 
*/
char* searchLargestFile() {
    DIR* currDirectory;
    struct dirent* entry;
    struct dirent* current;
    FILE* file;

    /* keep track of the highest amount of bytes */
    long currHighest = 0;

    /* open the current directory */
    currDirectory = opendir("."); 

    /* make sure its open */
    if (currDirectory == NULL) {
        return NULL;
    }

    /* get the files and directories inside the current directories */
    entry = readdir(currDirectory);
    current = NULL; /* getting the largests file */

    /* until there is no more files in the directory to go through, loop */
    while ((entry = readdir(currDirectory)) != NULL) {

        /* open the file */
        file = fopen(entry->d_name, "rb");

        if (file == NULL) {
            continue;
        }
        
        /* go through the file and see how many bytes are in it */
        fseek(file, 0, SEEK_END);
        
        long size = ftell(file);

        /* close the file */
        fclose(file);

        /* filter the files by name -- must have prefix 'movies' and postfix '.csv' */
        if (strncmp(entry->d_name, "movies_", strlen("movies_")) != 0 || strcmp(entry->d_name + strlen(entry->d_name) - strlen(".csv"), ".csv") != 0) {
            continue;
        }

        /* sort for the highest file */
        if (size > currHighest) {
            currHighest = size;
            current = entry;
        }
    }

    /* make sure there is at least one file in dir */
    if (current == NULL) {
        return NULL;
    }

    /* close directory */
    closedir(currDirectory);

    return current->d_name;

}

/**
 * Name: searchLowestFile
 * Input: none
 * Output: string
 * PreConditions: none
 * PostConditions: string contains the file name of the smallest file
 * Comment: 
*/
char* searchLowestFile() {
    DIR* currDirectory;
    struct dirent* entry;
    struct dirent* current;
    FILE* file;

    /* keep track of the lowest amount of bytes */
    long currLowest = LONG_MAX;

    /* open the current directory */
    currDirectory = opendir("."); 
    
    /* make sure its open */
    if (currDirectory == NULL) {
        return NULL;
    }

    /* get the files and directories inside the current directories */
    entry = readdir(currDirectory);
    current = NULL; /* getting the largests file */

    /* until there is no more files in the directory to go through, loop */
    while ((entry = readdir(currDirectory)) != NULL) {

        /* open the file */
        file = fopen(entry->d_name, "rb");

        if (file == NULL) {
            continue;
        }
        
        /* go through the file and see how many bytes are in it */
        fseek(file, 0, SEEK_END);
        
        long size = ftell(file);

        /* close the file */
        fclose(file);

        /* filter the files by name -- must have prefix 'movies' and postfix '.csv' */
        if (strncmp(entry->d_name, "movies_", strlen("movies_")) != 0 || strcmp(entry->d_name + strlen(entry->d_name) - strlen(".csv"), ".csv") != 0) {
            continue;
        }

        /* sort for the highest file */
        if (size < currLowest) {
            currLowest = size;
            current = entry;
        }
    }

    /* make sure there is at least one file in dir */
    if (current == NULL) {
        return NULL;
    }

    /* close directory */
    closedir(currDirectory);

    return current->d_name;
}

/**
 * Name: inputSearchFile
 * Input: none
 * Output: string
 * PreConditions: 
 * PostConditions: returns the users string provided the user inputted correctly
 * Comment: 
*/
char* inputSearchFile() {
    DIR* currDirectory;
    struct dirent* entry;

    char* userFile;

    /* found the file and opened it */
    int foundFlag = 0;

    /* get the user input */
    userFile = promptUserFile();

    /* open the current directory */
    currDirectory = opendir("."); 
    
    /* is the current directory open */
    if (currDirectory == NULL) {
        return NULL;
    }

    /* get the first file */
    entry = readdir(currDirectory);

    /* go through all the files and check if any file name matches the user input */
    while ((entry = readdir(currDirectory)) != NULL) {
        if (strcmp(entry->d_name, userFile)) {
            foundFlag = 1; //if so then mark it as found
        }   
    }

    /* close directory */
    closedir(currDirectory);

    /* if the file name matches the user input, return it */
    if (foundFlag == 1) {
        return userFile;
    } else {
        printf("The file %s was not found. Try again\n", userFile);
        return NULL;
    }
}

/**
 * Name: promptUserFile
 * Input: none
 * Output: the user's input -- string
 * PreConditions: none
 * PostConditions: user input contains an actual file name
 * Comment: 
*/
char* promptUserFile() {
    /* allocate mem */
    char* userInput = (char*) malloc (sizeof(char) * 1000);
    char c;

    /* get user input */
    printf("\n");
    printf("Enter the complete file name : ");
    scanf("%s", userInput);

    /* reset scanf */
    while ((c = getchar()) != '\n' && c != EOF);

    return userInput;
}