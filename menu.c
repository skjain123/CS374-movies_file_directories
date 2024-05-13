#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "menu.h"
#include "searchFile.h"
#include "movies.h"

/**
 * Name: mainMenuInput
 * Input:  none
 * Output: an integer 
 * PreConditions: user enters an int
 * PostConditions: the iteger returned is either 1 or 2
 * Comment: 
*/
int mainMenuInput()
{
    int userInput;
    int c;

    do
    {
        /* get user input */
        printf("\n");
        printf("1. Select file to process\n2. Exit the program\n\nEnter a choice 1 or 2 : ");
        scanf("%d", &userInput);

        /* reset scanf */
        while ((c = getchar()) != '\n' && c != EOF);
    } while (userInput != 1 && userInput != 2); /* repeatedly ask for an input until they give a valid input */

    return userInput;

}

/**
 * Name: mainMenu
 * Input: mone
 * Output: none
 * PreConditions: srand is seeded
 * PostConditions: none
 * Comment: 
*/
void mainMenu()
{
    int userInput;
    
    userInput = mainMenuInput();

    /* repeat until user enters 2 */
    while (userInput != 2) {
        processFileMenu(); //process file
        userInput = mainMenuInput();
    }
}

/**
 * Name: processFileMenuInput
 * Input: none
 * Output: an integer 
 * PreConditions: none
 * PostConditions: can either be a 1, 2, or 3
 * Comment: 
*/
int processFileMenuInput()
{
    int userInput;
    int c;

    do
    {
        /* get user input */
        printf("\n");
        printf("Which file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3 : ");
        scanf("%d", &userInput);

        /* reset scanf */
        while ((c = getchar()) != '\n' && c != EOF);
    } while (userInput != 1 && userInput != 2 && userInput != 3); /* repeatedly ask for an input until they give a valid input */

    return userInput;

}

/**
 * Name: processFileMenu
 * Input: none
 * Output: none
 * PreConditions: none
 * PostConditions: loops until it opens and processes a file
 * Comment: 
*/
void processFileMenu() {
    /* allocate memory */
    char* fileName = (char*) malloc(sizeof(char) * 1000000);
    int userInput;

    do {
        /* get the user input */
        userInput = processFileMenuInput();
    
        /* get the filename */
        if (userInput == 1) {
            fileName = searchLargestFile();
        } else if (userInput == 2) {
            fileName = searchLowestFile();
        } else if (userInput == 3) {
            fileName = inputSearchFile();
        }
        /* until the filename can correctly open a file, loop. */
    } while (startProcessing(fileName) == 0);
}

/**
 * Name: startProcessing
 * Input: a string
 * Output: an integer 
 * PreConditions: the string leads to a file
 * PostConditions: the integer indicates either 0 or 1: that a file has been opened and processed.
 * Comment: 
*/
int startProcessing(char* fileName) {

    /* processing the CSV into Movie structures */

    printf("\nNow processing the chosen file named %s\n", fileName);


    FILE *movieFile = fopen(fileName, "r");
    
    /* check if the movie file is open */
    if (movieFile == NULL) {
        fprintf(stderr, "The file %s was not found. Try again\n", fileName);
        return 0;
    }
    struct Movie *list = processFile(movieFile);

    /* getting valid user input until the answer is 4 (termination) */
    searchMoviesHighestRating(list, createNewDir());

    /* Freeing all the memory */
    freeList(list);

    return 1;

}

/**
 * Name: createNewDir
 * Input: none
 * Output: a string containing the path to the new directory
 * PreConditions: none
 * PostConditions: string returned containing the path to the new directory
 * Comment: 
*/
char* createNewDir() {

    /* Goal: rwxr-x---
     | First 3: User Permissions
     | Second 3: Group Permissions
     | Third 3: Other Permissions
      
     | S_IRWXU - WRX for U(ser)
     | S_IRGRP - R   for GRP (group)
     | S_IWGRP - W   for GRP (group)
     | S_IXGRP - X   for GRP (group)
    */
    
    /* allocate memory */
    char* newDir = (char*) malloc(sizeof(char) * 100); 
    
    /* add the file structure to the newDir */
    strcpy(newDir, "jainsu.movies.");

    sprintf(newDir + strlen(newDir), "%d", rand() % (99999 + 1));

    /* make the file and configure the folder */
    mkdir(newDir, S_IRWXU);
    chmod(newDir, S_IRWXU | S_IRGRP | S_IXGRP);

    return newDir;
}