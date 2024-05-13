/* 
Author: Sunil Jain
Class: Operating Systems 1 or CS 374
Description: A program that parses a CSV for data about movies (title, year released, 5 or less languages 
             the movie was released in, and the rating value of the movie). This program allows the user to search movies by year, 
             languages, and shows the highest rating movie per year (1900-2021).
Date: 4/16/2024
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "movies.h"

#define MAX_LANG_LENGTH 100

/**
 * Name: createMovie - creates movies and returns it to be added in a linked list.
 * Input: a single line from a CSV that holds movie data
 * Output: a movie filled with data from the CSV line.
 * PreConditions: The line is parsed using commas, and the languages are formatted as [1;2;3].
 * PostConditions: None.
*/
struct Movie *createMovie(char *currLine) {
    /* allocate memory to movie */
    struct Movie *currMovie = malloc(sizeof(struct Movie));

    char *saveptr;
    char *token;

    /* get the title */
	token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = strdup(token);

    /* get the year */
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    /* get the language(s) */
    token = strtok_r(NULL, ",", &saveptr);
    token++; /* get rid of the bracket [ */
    token[strlen(token) - 1] = '\0'; /* get rid of bracket ] */
    char *langToken = strtok(token, ";");
    int i = 0;
    char* blank = " "; /* initialize values */
    while (i < 5) { /* each movie can have up to 5 languages */
        strcpy(currMovie->languages[i], blank); /* initialize */
        if (langToken != NULL) {
            strcpy(currMovie->languages[i], langToken);
            langToken = strtok(NULL, ";");
        }
        i++;
    }

    /* get the rating */
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = atof(token);

    currMovie->next = NULL;

    return currMovie;
}

/**
 * Name: processFile - goes through the entire CSV file, creates movies, and adds them to a linked list.
 * Input: a file path going to a CSV file.
 * Output: a linked list of movies.
 * PreConditions: The filePath HAS to lead to a CSV file with proper formatting.
 * PostConditions: The linkedlist uses the movie structure itself.
*/
struct Movie *processFile(FILE *filePath) {
    /* open file */
    
    if (filePath == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    /* for getline */
    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;

    struct Movie *head = NULL;
    struct Movie *tail = NULL;
	
    /* boolean flag that tells us if the program went through the first line or not. */
	int skipFirstLine = 0;

    while ((nread = getline(&currLine, &len, filePath)) != -1) {
        /* using the flag. */
		if (skipFirstLine) {
            /* initialize movie, parse data, insert data into movie */
			struct Movie *newNode = createMovie(currLine);

            /* covering cases if the list is empty. if not then set as the tail. */
			if (head == NULL) {
				head = newNode;
				tail = newNode;
			} else {
                /* insert normally */
				tail->next = newNode;
				tail = newNode;
			}
		}
        /* set flag */
		skipFirstLine = 1;
        
    }
    /* free allocated memory after we are done and close the file. */
    free(currLine);
    fclose(filePath);

    /* return the linked list */
    return head;
}

/**
 * Name: searchMoviesHighestRating
 * Input: a movie list and a path to make a file and write to it
 * Output: none
 * PreConditions: list is LL and is filled with a movie, dirPath is initialized and has a properly made path in the cwd
 * PostConditions: none
 * Comment: 
*/
void searchMoviesHighestRating(struct Movie *list, char* dirPath) {
    struct Movie* current; /* move through the list */
    double highest = -1.0; /* keep track of the highest score to compare */
    char* filePath = (char*) malloc (sizeof(char) * 100);

    /* if we dont find any movies in the LL then print message. */
    int flag = 0;

    int i;
    /* for all years between 1900-2021, go thru the LL and find the highest corresponding rating for that year. */
    for (i = 1900; i <= 2021; i++) {
        current = list;
        highest = -1.0;
        while (current != NULL) {
            if (current->year == i) {
                flag = 1;

                sprintf(filePath, "%s/%d.txt", dirPath, i);

                FILE* file = fopen(filePath, "a");

                fprintf(file, "%d %.1f %s\n", current->year, current->rating, current->title);

                fclose(file);
            }
            current = current->next;
        }
        if (highest >= 0.0) { /* if we didnt find any movie for that year dont print */
            
        }
    }

    printf("\n");

    if (flag == 0) {
        printf("No Movies!\n");
    }
}


/**
 * Name: freeList - frees all data used
 * Input: none
 * Output: none
 * PreConditions: linked list uses malloc to create data
 * PostConditions: all data from all movies are freed.
*/
void freeList(struct Movie *list) { /* frees all aspects of a movie */
    struct Movie *nextNode;
    while (list != NULL) {
        nextNode = list->next;
        free(list->title);
        free(list);
        list = nextNode;
    }
}