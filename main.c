/* 
Author: Sunil Jain
Class: Operating Systems 1 or CS 374
Description: A program that parses a CSV for data about movies (title, year released, 5 or less languages 
             the movie was released in, and the rating value of the movie). This program allows the user to search movies by year, 
             languages, and shows the highest rating movie per year (1900-2021).
Date: 4/16/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "movies.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    /* seed the time */
    srand(time(NULL));

    /* call the menu */
    mainMenu();

    return 0;
}
