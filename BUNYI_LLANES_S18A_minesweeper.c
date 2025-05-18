/*
    DESCRIPTION:
        This program is an imitation of the classic Minesweeper game developed by Microsoft. It is
        a single-player game wherein the user navigates through the board and avoids inspecting any
        mines to win. The format of the board can vary. With that being said, the user may modify
        the board's format through their choice with a range of 5 to 10 rows and 5 to 15 columns
        through a custom game. The user may also choose to play a classic game of either easy or
        difficult which already has a set number of rows, columns and mines. However, the number
        and locations of the mines are randomized and these apply to both modes. Furthermore, the
        progam further implements leaderboard, statistics, and game profile features which lets the
        program have its own individuality from the original game.
       
    AUTHORS:
        Christian Joseph Bunyi
        Andre Gabriel Llanes

    SECTION:
        S18A
       
    LAST MODIFIED:
        April 1, 2024

    ACKNOWLEDGMENTS:
        1. Functions from Ups and Downs, a CCPROG1 Machine Project by Christian Joseph Bunyi
        2. ASCII codes from https://theasciicode.com.ar/
        3. Local date and time extraction techniques from
           https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
*/


// preprocessor directives
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_ROWS 10
#define MAX_COLUMNS 15
#define MAX_PROFILES 10
#define MAX_LEVELS 100
#define MAX_RECORDS 10

#define SHORT_SLEEP 500
#define LONG_SLEEP 1000
#define BOARD_REFRESH 100

#define ENTER_VALUE 13
#define ESCAPE_VALUE 27
#define ARROW_VALUE 224
#define UP_VALUE 72
#define DOWN_VALUE 80
#define LEFT_VALUE 75
#define RIGHT_VALUE 77

#define EASY_MODE "Classic->Easy"
#define DIFFICULT_MODE "Classic->Difficult"
#define CUSTOM_MODE "Custom"

#define WON_OUTCOME "Won"
#define LOST_OUTCOME "Lost"
#define QUIT_OUTCOME "Quit"

#define PROFILES_DIRECTORY "profiles\\profiles.txt"
#define LEADERBOARD_DIRECTORY "profiles\\leaderboard.txt"
#define LEVELS_DIRECTORY "levels\\levels.txt"

typedef char string20[21];
typedef char string100[101];

struct Tile {
    int state;
    int isFlagged;
    int isRevealed;
};

struct Game {
    int exists;
    int rows;
    int columns;
    struct Tile Board[MAX_ROWS][MAX_COLUMNS];
    string20 mode;
    string20 outcome;
    int seconds;
};

struct Stats {
    int totalSeconds;
    int won;
    int lost;
};

struct Profile {
    string20 name;
    int creationDate;
    int lifetimeGames;

    struct Stats EasyStats;
    struct Stats DifficultStats;
    struct Stats CustomStats;

    struct Game CurrentGame;
    struct Game RecentGame1;
    struct Game RecentGame2;
    struct Game RecentGame3;
};

struct Records {
    int numRecords;
    string20 names[MAX_RECORDS];
    int times[MAX_RECORDS];
};

struct Leaderboard {
    struct Records EasyRecords;
    struct Records DifficultRecords;
    struct Records CustomRecords;
};


/*
	@brief: prints the title screen ASCII

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: assumes theme is between 1 to 4, inclusive
*/
void printTitle(int theme) {
    printf("\n");

    if (theme == 1) { // cyan
        printf("  \x1b[1m\x1b[36m%c%c     %c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c %c%c  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c   %c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c\x1b[0m\n", 219,219, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 220, 219,219,219,219, 220, 223, 219, 219, 219, 219, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220);
        printf("  \x1b[1m\x1b[33m%c%c%c%c %c%c%c%c      %c%c     %c%c  %c %c%c  %c%c        %c%c       %c%c   %c%c   %c%c         %c%c         %c%c   %c%c  %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c  %c  %c%c      %c%c     %c%c  %c %c%c  %c%c%c%c%c%c     %c%c%c%c%c   %c%c   %c%c   %c%c%c%c%c%c     %c%c%c%c%c%c     %c%c   %c%c  %c%c%c%c%c%c     %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 223, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c             %c%c  %c%c %c %c%c   %c%c         %c%c         %c%c%c%c%c%c   %c%c         %c%c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 223);
        printf("  \x1b[1m\x1b[33m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c        %c    %c%c  %c%c%c%c%c%c%c   %c%c         %c%c         %c%c       %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220);
        printf(" \x1b[1m\x1b[36m%c%c%c%c   %c%c%c%c  %c%c%c%c%c%c%c%c  %c%c  %c%c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c    %c%c %c%c    %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c      %c%c%c%c%c%c%c%c  %c%c%c  %c%c%c%c\x1b[0m\n", 220, 219, 219, 220, 220, 219, 219, 220, 219, 220, 220, 219, 219, 220, 220, 219, 219, 219, 223, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220);
    }
    else if (theme == 2) { // bright red
        printf("  \x1b[1m\x1b[31m%c%c     %c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c %c%c  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c   %c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c\x1b[0m\n", 219,219, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 220, 219,219,219,219, 220, 223, 219, 219, 219, 219, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220);
        printf("  \x1b[1m\x1b[33m%c%c%c%c %c%c%c%c      %c%c     %c%c  %c %c%c  %c%c        %c%c       %c%c   %c%c   %c%c         %c%c         %c%c   %c%c  %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c  %c  %c%c      %c%c     %c%c  %c %c%c  %c%c%c%c%c%c     %c%c%c%c%c   %c%c   %c%c   %c%c%c%c%c%c     %c%c%c%c%c%c     %c%c   %c%c  %c%c%c%c%c%c     %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 223, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c             %c%c  %c%c %c %c%c   %c%c         %c%c         %c%c%c%c%c%c   %c%c         %c%c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 223);
        printf("  \x1b[1m\x1b[33m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c        %c    %c%c  %c%c%c%c%c%c%c   %c%c         %c%c         %c%c       %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220);
        printf(" \x1b[1m\x1b[31m%c%c%c%c   %c%c%c%c  %c%c%c%c%c%c%c%c  %c%c  %c%c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c    %c%c %c%c    %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c      %c%c%c%c%c%c%c%c  %c%c%c  %c%c%c%c\x1b[0m\n", 220, 219, 219, 220, 220, 219, 219, 220, 219, 220, 220, 219, 219, 220, 220, 219, 219, 219, 223, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220);
    }
    else if (theme == 3) { // bright green
        printf("  \x1b[1m\x1b[32m%c%c     %c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c %c%c  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c   %c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c\x1b[0m\n", 219,219, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 220, 219,219,219,219, 220, 223, 219, 219, 219, 219, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220);
        printf("  \x1b[1m\x1b[33m%c%c%c%c %c%c%c%c      %c%c     %c%c  %c %c%c  %c%c        %c%c       %c%c   %c%c   %c%c         %c%c         %c%c   %c%c  %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c  %c  %c%c      %c%c     %c%c  %c %c%c  %c%c%c%c%c%c     %c%c%c%c%c   %c%c   %c%c   %c%c%c%c%c%c     %c%c%c%c%c%c     %c%c   %c%c  %c%c%c%c%c%c     %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 223, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c             %c%c  %c%c %c %c%c   %c%c         %c%c         %c%c%c%c%c%c   %c%c         %c%c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 223);
        printf("  \x1b[1m\x1b[33m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c        %c    %c%c  %c%c%c%c%c%c%c   %c%c         %c%c         %c%c       %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220);
        printf(" \x1b[1m\x1b[32m%c%c%c%c   %c%c%c%c  %c%c%c%c%c%c%c%c  %c%c  %c%c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c    %c%c %c%c    %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c      %c%c%c%c%c%c%c%c  %c%c%c  %c%c%c%c\x1b[0m\n", 220, 219, 219, 220, 220, 219, 219, 220, 219, 220, 220, 219, 219, 220, 220, 219, 219, 219, 223, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220);
    }
    else if (theme == 4) { // dark purple
        printf("  \x1b[1m\x1b[35m%c%c     %c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c %c%c  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c   %c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c\x1b[0m\n", 219,219, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 220, 219,219,219,219, 220, 223, 219, 219, 219, 219, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 219, 219, 223, 219, 219, 219, 219, 219, 220);
        printf("  \x1b[1m\x1b[33m%c%c%c%c %c%c%c%c      %c%c     %c%c  %c %c%c  %c%c        %c%c       %c%c   %c%c   %c%c         %c%c         %c%c   %c%c  %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c  %c  %c%c      %c%c     %c%c  %c %c%c  %c%c%c%c%c%c     %c%c%c%c%c   %c%c   %c%c   %c%c%c%c%c%c     %c%c%c%c%c%c     %c%c   %c%c  %c%c%c%c%c%c     %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 223, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223, 223, 219, 219, 219, 219);
        printf("  \x1b[1m\x1b[37m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c             %c%c  %c%c %c %c%c   %c%c         %c%c         %c%c%c%c%c%c   %c%c         %c%c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 223);
        printf("  \x1b[1m\x1b[33m%c%c     %c%c      %c%c     %c%c  %c %c%c  %c%c        %c    %c%c  %c%c%c%c%c%c%c   %c%c         %c%c         %c%c       %c%c         %c%c   %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220);
        printf(" \x1b[1m\x1b[35m%c%c%c%c   %c%c%c%c  %c%c%c%c%c%c%c%c  %c%c  %c%c%c%c  %c%c%c%c%c%c%c%c   %c%c%c%c%c    %c%c %c%c    %c%c%c%c%c%c%c%c   %c%c%c%c%c%c%c%c  %c%c%c%c      %c%c%c%c%c%c%c%c  %c%c%c  %c%c%c%c\x1b[0m\n", 220, 219, 219, 220, 220, 219, 219, 220, 219, 220, 220, 219, 219, 220, 220, 219, 219, 219, 223, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220, 220, 220, 220, 219, 219, 220, 219, 219, 220, 219, 219, 220);
    }
}


/*
	@brief: prints the main menu ASCII

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: assumes theme is between 1 to 4
*/
void printMenu(int theme) {
	if (theme == 1) { // cyan
        printf ("\x1b[1m\x1b[37m    _.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m .-'---      - ---     --     ---   ---- '-.\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[36m%c%c%c  %c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[37m%c%c%c%c%c%c%c%c %c%c      %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[33m%c%c %c%c %c%c %c%c%c%c%c   %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[36m%c%c    %c%c %c%c%c%c%c%c%c %c%c    %c%c %c%c%c%c%c%c%c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-' ( __ _)  `-._.-'  (__ _ ) `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-'          `-._.-'          `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m                   `-._.-'\x1b[0m\n");
    }
    else if (theme == 2) { // bright red
        printf ("\x1b[1m\x1b[37m    _.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m .-'---      - ---     --     ---   ---- '-.\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[31m%c%c%c  %c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[37m%c%c%c%c%c%c%c%c %c%c      %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[33m%c%c %c%c %c%c %c%c%c%c%c   %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[31m%c%c    %c%c %c%c%c%c%c%c%c %c%c    %c%c %c%c%c%c%c%c%c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-' ( __ _)  `-._.-'  (__ _ ) `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-'          `-._.-'          `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m                   `-._.-'\x1b[0m\n");
    }
    else if (theme == 3) { // bright green
        printf ("\x1b[1m\x1b[37m    _.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m .-'---      - ---     --     ---   ---- '-.\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[32m%c%c%c  %c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[37m%c%c%c%c%c%c%c%c %c%c      %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[33m%c%c %c%c %c%c %c%c%c%c%c   %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[32m%c%c    %c%c %c%c%c%c%c%c%c %c%c    %c%c %c%c%c%c%c%c%c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-' ( __ _)  `-._.-'  (__ _ ) `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-'          `-._.-'          `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m                   `-._.-'\x1b[0m\n");
    }
    else if (theme == 4) { // dark purple
        printf ("\x1b[1m\x1b[37m    _.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m .-'---      - ---     --     ---   ---- '-.\x1b[0m\n");
        printf ("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[35m%c%c%c  %c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[37m%c%c%c%c%c%c%c%c %c%c      %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[33m%c%c %c%c %c%c %c%c%c%c%c   %c%c    %c%c %c%c    %c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (\x1b[0m    \x1b[1m\x1b[35m%c%c    %c%c %c%c%c%c%c%c%c %c%c    %c%c %c%c%c%c%c%c%c%c\x1b[0m   \x1b[1m\x1b[37m)\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf("\x1b[1m\x1b[37m (                                         )\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-' ( __ _)  `-._.-'  (__ _ ) `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m  `-._.-'          `-._.-'          `-._.-'\x1b[0m\n");
        printf("\x1b[1m\x1b[37m                   `-._.-'\x1b[0m\n");
    }
}


/*
	@brief: prints the leaderboard ASCII

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: assumes theme is between 1 to 4, inclusive
*/
void printLeaderboard(int theme) {
	if (theme == 1) { // cyan
        printf ("\x1b[1m\x1b[36m%c%c   %c%c %c%c%c%c%c%c%c	%c%c    %c%c      %c%c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219 ,223, 223, 223, 219, 219, 223, 223, 223, 220, 219, 219, 219, 219, 219, 220, 220, 219, 219, 219, 220, 220, 219, 219, 219, 220, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c  %c%c  %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m%c%c%c%c%c%c%c	%c%c%c%c%c%c%c %c%c    %c%c      %c%c  %c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c  %c%c  %c%c %c%c%c%c\x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219 ,219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[36m%c%c   %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 219, 219, 220, 220, 220, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    }
    else if (theme == 2) { // bright red
        printf ("\x1b[1m\x1b[31m%c%c   %c%c %c%c%c%c%c%c%c	%c%c    %c%c      %c%c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219 ,223, 223, 223, 219, 219, 223, 223, 223, 220, 219, 219, 219, 219, 219, 220, 220, 219, 219, 219, 220, 220, 219, 219, 219, 220, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c  %c%c  %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m%c%c%c%c%c%c%c	%c%c%c%c%c%c%c %c%c    %c%c      %c%c  %c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c  %c%c  %c%c %c%c%c%c\x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219 ,219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[31m%c%c   %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 219, 219, 220, 220, 220, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    }
    else if (theme == 3) { // bright green
        printf ("\x1b[1m\x1b[32m%c%c   %c%c %c%c%c%c%c%c%c	%c%c    %c%c      %c%c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219 ,223, 223, 223, 219, 219, 223, 223, 223, 220, 219, 219, 219, 219, 219, 220, 220, 219, 219, 219, 220, 220, 219, 219, 219, 220, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c  %c%c  %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m%c%c%c%c%c%c%c	%c%c%c%c%c%c%c %c%c    %c%c      %c%c  %c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c  %c%c  %c%c %c%c%c%c\x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219 ,219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[32m%c%c   %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 219, 219, 220, 220, 220, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    }
    else if (theme == 4) { // dark purple
        printf ("\x1b[1m\x1b[35m%c%c   %c%c %c%c%c%c%c%c%c	%c%c    %c%c      %c%c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219 ,223, 223, 223, 219, 219, 223, 223, 223, 220, 219, 219, 219, 219, 219, 220, 220, 219, 219, 219, 220, 220, 219, 219, 219, 220, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c  %c%c  %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m%c%c%c%c%c%c%c	%c%c%c%c%c%c%c %c%c    %c%c      %c%c  %c%c %c%c%c%c%c   %c%c%c%c%c %c%c%c%c%c%c%c %c%c  %c%c  %c%c %c%c%c%c\x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 223, 223, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219, 219, 223, 223, 223, 219 ,219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m%c%c   %c%c %c%c   %c%c %c%c    %c%c      %c%c  %c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[35m%c%c   %c%c %c%c   %c%c %c%c%c%c%c %c%c%c%c%c   %c%c%c%c%c%c %c%c      %c%c    %c%c   %c%c %c%c      %c%c %c%c%c%c%c\x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 220, 220, 219, 219, 220, 220, 220, 219, 219, 220, 220, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
    }
}


/*
	@brief: prints the termination sequence ASCII

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: assumes theme is between 1 to 4, inclusive
*/
void printEnd(int theme) {
	if (theme == 1) { // cyan
        printf ("                                              \x1b[1m\x1b[36m%c%c   %c%c\n", 220, 220, 220, 220);
        printf ("\x1b[1m\x1b[37m  %c%c%c %c  %c %c%c%c%c %c%c%c%c %c %c%c   %c   %c %c%c%c %c %c\x1b[0m\x1b[1m\x1b[36m    %c%c%c%c%c%c%c%c%c\n", 223, 219, 223, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 220, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c  %c%c%c%c %c%c%c%c %c  %c %c%c%c     %c%c%c  %c %c %c %c\x1b[0m\x1b[1m\x1b[36m     %c%c%c%c%c%c%c   \n", 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 223, 220, 223, 220, 223, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 223);
        printf ("\x1b[1m\x1b[37m   %c  %c  %c %c  %c %c  %c %c  %c     %c   %c%c%c %c%c%c\x1b[0m\x1b[1m\x1b[36m       %c%c%c       \n\x1b[0m", 219, 219 ,219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 220, 219, 223, 219, 223);
    }
    else if (theme == 2) { // bright red
        printf ("                                              \x1b[1m\x1b[31m%c%c   %c%c\n", 220, 220, 220, 220);
        printf ("\x1b[1m\x1b[37m  %c%c%c %c  %c %c%c%c%c %c%c%c%c %c %c%c   %c   %c %c%c%c %c %c\x1b[0m\x1b[1m\x1b[31m    %c%c%c%c%c%c%c%c%c\n", 223, 219, 223, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 220, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c  %c%c%c%c %c%c%c%c %c  %c %c%c%c     %c%c%c  %c %c %c %c\x1b[0m\x1b[1m\x1b[31m     %c%c%c%c%c%c%c   \n", 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 223, 220, 223, 220, 223, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 223);
        printf ("\x1b[1m\x1b[37m   %c  %c  %c %c  %c %c  %c %c  %c     %c   %c%c%c %c%c%c\x1b[0m\x1b[1m\x1b[31m       %c%c%c       \n\x1b[0m", 219, 219 ,219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 220, 219, 223, 219, 223);
    }
    else if (theme == 3) { // bright green
        printf ("                                              \x1b[1m\x1b[32m%c%c   %c%c\n", 220, 220, 220, 220);
        printf ("\x1b[1m\x1b[37m  %c%c%c %c  %c %c%c%c%c %c%c%c%c %c %c%c   %c   %c %c%c%c %c %c\x1b[0m\x1b[1m\x1b[32m    %c%c%c%c%c%c%c%c%c\n", 223, 219, 223, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 220, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c  %c%c%c%c %c%c%c%c %c  %c %c%c%c     %c%c%c  %c %c %c %c\x1b[0m\x1b[1m\x1b[32m     %c%c%c%c%c%c%c   \n", 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 223, 220, 223, 220, 223, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 223);
        printf ("\x1b[1m\x1b[37m   %c  %c  %c %c  %c %c  %c %c  %c     %c   %c%c%c %c%c%c\x1b[0m\x1b[1m\x1b[32m       %c%c%c       \n\x1b[0m", 219, 219 ,219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 220, 219, 223, 219, 223);
    }
    else if (theme == 4) { // dark purple
        printf ("                                              \x1b[1m\x1b[35m%c%c   %c%c\n", 220, 220, 220, 220);
        printf ("\x1b[1m\x1b[37m  %c%c%c %c  %c %c%c%c%c %c%c%c%c %c %c%c   %c   %c %c%c%c %c %c\x1b[0m\x1b[1m\x1b[35m    %c%c%c%c%c%c%c%c%c\n", 223, 219, 223, 219, 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 220, 223, 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c  %c%c%c%c %c%c%c%c %c  %c %c%c%c     %c%c%c  %c %c %c %c\x1b[0m\x1b[1m\x1b[35m     %c%c%c%c%c%c%c   \n", 219, 219, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 223, 220, 223, 220, 223, 219, 219, 219, 219, 223, 219, 219, 219, 219, 219, 223);
        printf ("\x1b[1m\x1b[37m   %c  %c  %c %c  %c %c  %c %c  %c     %c   %c%c%c %c%c%c\x1b[0m\x1b[1m\x1b[35m       %c%c%c       \n\x1b[0m", 219, 219 ,219, 219, 219, 219, 219, 219, 219, 219, 219, 220, 219, 219, 220, 219, 223, 219, 223);
    }
}


/*
	@brief: prints the "evade the mine" ASCII

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: assumes theme is between 1 to 4, inclusive
*/
void printEvade(int theme) {
    printf("\n");

    if (theme == 1) { // cyan
        printf ("                                                                            \x1b[1m\x1b[31m_------*\x1b[0m\n");
        printf ("\x1b[1m\x1b[36m   %c%c%c%c%c %c%c   %c%c %c%c%c%c%c%c%c%c %c%c%c%c%c  %c%c%c%c%c%c     %c%c%c%c%c%c %c%c  %c%c %c%c%c%c%c\x1b[0m           \x1b[1m\x1b[31m_/ \x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223);
        printf ("\x1b[1m\x1b[37m   %c%c    %c%c   %c%c %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m             \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c%c%c%c  %c%c%c %c%c%c %c%c%c%c%c%c%c%c %c%c   %c %c%c%c%c         %c%c   %c%c%c%c%c%c %c%c%c%c\x1b[0m          \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 223, 223, 219, 220, 220, 219, 223, 219, 219, 223, 223, 223, 223, 219 ,219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m   %c%c     %c%c %c%c  %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m           \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[36m   %c%c%c%c%c   %c%c%c   %c%c    %c%c %c%c%c%c%c  %c%c%c%c%c%c       %c%c   %c%c  %c%c %c%c%c%c%c\x1b[0m       \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 220, 220, 220, 223, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 219 ,219, 219, 219, 220, 220, 220);
        printf ("                                                                     \x1b[1m\x1b[31m(_)\x1b[0m\n");
    }
    else if (theme == 2) { // bright red
        printf ("                                                                            \x1b[1m\x1b[31m_------*\x1b[0m\n");
        printf ("\x1b[1m\x1b[31m   %c%c%c%c%c %c%c   %c%c %c%c%c%c%c%c%c%c %c%c%c%c%c  %c%c%c%c%c%c     %c%c%c%c%c%c %c%c  %c%c %c%c%c%c%c\x1b[0m           \x1b[1m\x1b[31m_/ \x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223);
        printf ("\x1b[1m\x1b[37m   %c%c    %c%c   %c%c %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m             \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c%c%c%c  %c%c%c %c%c%c %c%c%c%c%c%c%c%c %c%c   %c %c%c%c%c         %c%c   %c%c%c%c%c%c %c%c%c%c\x1b[0m          \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 223, 223, 219, 220, 220, 219, 223, 219, 219, 223, 223, 223, 223, 219 ,219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m   %c%c     %c%c %c%c  %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m           \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[31m   %c%c%c%c%c   %c%c%c   %c%c    %c%c %c%c%c%c%c  %c%c%c%c%c%c       %c%c   %c%c  %c%c %c%c%c%c%c\x1b[0m       \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 220, 220, 220, 223, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 219 ,219, 219, 219, 220, 220, 220);
        printf ("                                                                     \x1b[1m\x1b[31m(_)\x1b[0m\n");
    }
    else if (theme == 3) { // bright green
        printf ("                                                                            \x1b[1m\x1b[31m_------*\x1b[0m\n");
        printf ("\x1b[1m\x1b[32m   %c%c%c%c%c %c%c   %c%c %c%c%c%c%c%c%c%c %c%c%c%c%c  %c%c%c%c%c%c     %c%c%c%c%c%c %c%c  %c%c %c%c%c%c%c\x1b[0m           \x1b[1m\x1b[31m_/ \x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223);
        printf ("\x1b[1m\x1b[37m   %c%c    %c%c   %c%c %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m             \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c%c%c%c  %c%c%c %c%c%c %c%c%c%c%c%c%c%c %c%c   %c %c%c%c%c         %c%c   %c%c%c%c%c%c %c%c%c%c\x1b[0m          \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 223, 223, 219, 220, 220, 219, 223, 219, 219, 223, 223, 223, 223, 219 ,219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m   %c%c     %c%c %c%c  %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m           \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[32m   %c%c%c%c%c   %c%c%c   %c%c    %c%c %c%c%c%c%c  %c%c%c%c%c%c       %c%c   %c%c  %c%c %c%c%c%c%c\x1b[0m       \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 220, 220, 220, 223, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 219 ,219, 219, 219, 220, 220, 220);
        printf ("                                                                     \x1b[1m\x1b[31m(_)\x1b[0m\n");
    }
    else if (theme == 4) { // dark purple
        printf ("                                                                            \x1b[1m\x1b[31m_------*\x1b[0m\n");
        printf ("\x1b[1m\x1b[35m   %c%c%c%c%c %c%c   %c%c %c%c%c%c%c%c%c%c %c%c%c%c%c  %c%c%c%c%c%c     %c%c%c%c%c%c %c%c  %c%c %c%c%c%c%c\x1b[0m           \x1b[1m\x1b[31m_/ \x1b[0m\n", 219, 219, 223, 223, 223, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 220, 219, 219, 223, 223, 223, 223, 223, 223, 219, 219, 223, 223, 219, 219, 219, 219, 219, 219, 223, 223, 223);
        printf ("\x1b[1m\x1b[37m   %c%c    %c%c   %c%c %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m             \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[37m   %c%c%c%c  %c%c%c %c%c%c %c%c%c%c%c%c%c%c %c%c   %c %c%c%c%c         %c%c   %c%c%c%c%c%c %c%c%c%c\x1b[0m          \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 223, 223, 219, 220, 220, 219, 223, 219, 219, 223, 223, 223, 223, 219 ,219, 219, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223, 219, 219, 219, 219, 223, 223);
        printf ("\x1b[1m\x1b[33m   %c%c     %c%c %c%c  %c%c    %c%c %c%c  %c%c %c%c           %c%c   %c%c  %c%c %c%c\x1b[0m           \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 223, 219, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
        printf ("\x1b[1m\x1b[35m   %c%c%c%c%c   %c%c%c   %c%c    %c%c %c%c%c%c%c  %c%c%c%c%c%c       %c%c   %c%c  %c%c %c%c%c%c%c\x1b[0m       \x1b[1m\x1b[31m/ / \x1b[0m\n", 219, 219, 220, 220, 220, 223, 219, 223, 219, 219, 219, 219, 219, 219, 219, 219, 223, 219, 219, 220, 220, 220, 220, 219, 219, 219, 219, 219 ,219, 219, 219, 220, 220, 220);
        printf ("                                                                     \x1b[1m\x1b[31m(_)\x1b[0m\n");
    }
}


/*
	@brief: extracts the current date and formats it as an integer code
	
	@return: integer of the format MMDDYYYY (MM - month, DD - day, YYYY - year)
*/
int getDateCode() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return (tm.tm_mon + 1) * 1000000 + tm.tm_mday * 10000 + tm.tm_year + 1900;
}


/*
	@brief: updates the current profile struct's statistics based on the current game outcome
	
	@param: CurrentProfile - pointer to the current profile structure holding profile information
	
	Precondition: assumes a game has recently concluded
*/
void updateStatistics(struct Profile *CurrentProfile) {
    string20 mode;
    strcpy(mode, CurrentProfile->CurrentGame.mode);
    int seconds = CurrentProfile->CurrentGame.seconds;

    if (strcmp(CurrentProfile->CurrentGame.outcome, WON_OUTCOME) == 0) { // won
        if (strcmp(mode, EASY_MODE) == 0) {
            CurrentProfile->EasyStats.won++;
            CurrentProfile->EasyStats.totalSeconds += seconds;
        }
        else if (strcmp(mode, DIFFICULT_MODE) == 0) {
            CurrentProfile->DifficultStats.won++;
            CurrentProfile->DifficultStats.totalSeconds += seconds;
        }
        else if (strcmp(mode, CUSTOM_MODE) == 0) {
            CurrentProfile->CustomStats.won++;
            CurrentProfile->CustomStats.totalSeconds += seconds;
        }
    }
    else { // lost/quit
        if (strcmp(mode, EASY_MODE) == 0) {
            CurrentProfile->EasyStats.lost++;
        }
        else if (strcmp(mode, DIFFICULT_MODE) == 0) {
            CurrentProfile->DifficultStats.lost++;
        }
        else if (strcmp(mode, CUSTOM_MODE) == 0) {
            CurrentProfile->CustomStats.lost++;
        }
    }
}


/*
	@brief: updates the current profile's struct's recent games
	
	@param: CurrentProfile - pointer to the current profile structure holding profile information
	
	Precondition: assumes a game has recently concluded
*/
void updateRecentGames(struct Profile *CurrentProfile) {
    int i, j;

    // set recent game 3's details to that of recent game 2's
    if (CurrentProfile->RecentGame2.exists) {
        CurrentProfile->RecentGame3 = CurrentProfile->RecentGame2;
    }

    // set recent game 2's details to that of recent game 1's
    if (CurrentProfile->RecentGame1.exists) {
        CurrentProfile->RecentGame2 = CurrentProfile->RecentGame1;
    }

    // set recent game 1's details to that of the current game's
    if (CurrentProfile->CurrentGame.exists) {
        CurrentProfile->RecentGame1 = CurrentProfile->CurrentGame;

        // reset current board's details
        CurrentProfile->CurrentGame.rows = 0;
        CurrentProfile->CurrentGame.columns = 0;
        for (i = 0; i < MAX_ROWS; i++) {
            for (j = 0; j < MAX_COLUMNS; j++) {
                CurrentProfile->CurrentGame.Board[i][j].state = 0;
                CurrentProfile->CurrentGame.Board[i][j].isFlagged = 0;
                CurrentProfile->CurrentGame.Board[i][j].isRevealed = 0;
            }
        }
        strcpy(CurrentProfile->CurrentGame.mode, "");
        strcpy(CurrentProfile->CurrentGame.outcome, "");
        CurrentProfile->CurrentGame.seconds = 0;
    }
}


/*
	@brief: converts a tile into a 3-4 digit code for storing information
	
	@param: row - the row of the tile
	@param: column - the column of the tile
	@param: Board - a 2-dimensional array of tiles representing the board where the tile belongs
	
	@return: 3-4 digit integer representing the tile's code

    Precondition: assumes row and column are not out of bounds
*/
int getTileCode(int row, int column, struct Tile Board[][MAX_COLUMNS]) {
    int code = 0;
    code += Board[row][column].state * 100;
    code += Board[row][column].isFlagged * 10;
    code += Board[row][column].isRevealed;
    return code;
}


/*
	@brief: access the current profile's text file and update it in accordance with the struct's
        contents
	
	@param: CurrentProfile - pointer to the current profile struct that we will be using to update
        their text file

    Precondition: the current profile name corresponds with the actual profile being updated
*/	
void updateProfile(struct Profile *CurrentProfile) {
    FILE *fp;
    int i, j;

    string100 directory = "profiles\\";
    strcat(directory, CurrentProfile->name);
    strcat(directory, ".txt");

    updateStatistics(CurrentProfile);
    updateRecentGames(CurrentProfile);

    fp = fopen(directory, "w");
    if (fp == NULL) return;

    struct Game *RecentGame1 = &CurrentProfile->RecentGame1;
    struct Game *RecentGame2 = &CurrentProfile->RecentGame2;
    struct Game *RecentGame3 = &CurrentProfile->RecentGame3;

    // player information and statistics
    fprintf(fp, "%s\n%d\n%d\n\n", CurrentProfile->name, CurrentProfile->creationDate, CurrentProfile->lifetimeGames);
    fprintf(fp, "%d\n%d\n%d\n\n", CurrentProfile->EasyStats.totalSeconds, CurrentProfile->EasyStats.won,
        CurrentProfile->EasyStats.lost);
    fprintf(fp, "%d\n%d\n%d\n\n", CurrentProfile->DifficultStats.totalSeconds, CurrentProfile->DifficultStats.won,
        CurrentProfile->DifficultStats.lost);
    fprintf(fp, "%d\n%d\n%d\n\n", CurrentProfile->CustomStats.totalSeconds, CurrentProfile->CustomStats.won,
        CurrentProfile->CustomStats.lost);

    // recent game 1
    fprintf(fp, "%d\n", RecentGame1->exists);
    if (RecentGame1->exists) {
        fprintf(fp, "%d %d\n", RecentGame1->rows, RecentGame1->columns);
        for (i = 0; i < RecentGame1->rows; i++) {
            for (j = 0; j < RecentGame1->columns; j++) {
                fprintf(fp, "%d ", getTileCode(i, j, RecentGame1->Board));
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "%s\n%s\n", RecentGame1->mode, RecentGame1->outcome);
        fprintf(fp, "%d\n", RecentGame1->seconds);
    }

    // recent game 2
    fprintf(fp, "\n%d\n", RecentGame2->exists);
    if (RecentGame2->exists) {
        fprintf(fp, "%d %d\n", RecentGame2->rows, RecentGame2->columns);
        for (i = 0; i < RecentGame2->rows; i++) {
            for (j = 0; j < RecentGame2->columns; j++) {
                fprintf(fp, "%d ", getTileCode(i, j, RecentGame2->Board));
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "%s\n%s\n", RecentGame2->mode, RecentGame2->outcome);
        fprintf(fp, "%d\n", RecentGame2->seconds);
    }

    // recent game 3
    fprintf(fp, "\n%d\n", RecentGame3->exists);
    if (RecentGame3->exists) {
        fprintf(fp, "%d %d\n", RecentGame3->rows, RecentGame3->columns);
        for (i = 0; i < RecentGame3->rows; i++) {
            for (j = 0; j < RecentGame3->columns; j++) {
                fprintf(fp, "%d ", getTileCode(i, j, RecentGame3->Board));
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "%s\n%s\n", RecentGame3->mode, RecentGame3->outcome);
        fprintf(fp, "%d\n", RecentGame3->seconds);
    }

    fclose(fp);
}


/*
	@brief: initializes the values of the current profile's information and statistics to default
        values, except for the creation date
	
	@param: CurrentProfile - pointer to the current profile's struct that we want to initialize
	@param: name - the profile's name

    Precondition: The profile is being created or reset. MAX_ROWS and MAX_COLUMNS are accurate.
*/
void initializeProfile(struct Profile *CurrentProfile, char name[]) {
    int i, j;
    struct Game *CurrentGame = &CurrentProfile->CurrentGame;

    strcpy(CurrentProfile->name, name);
    CurrentProfile->lifetimeGames = 0;

    CurrentProfile->EasyStats.totalSeconds = 0;
    CurrentProfile->EasyStats.won = 0;
    CurrentProfile->EasyStats.lost = 0;

    CurrentProfile->DifficultStats.totalSeconds = 0;
    CurrentProfile->DifficultStats.won = 0;
    CurrentProfile->DifficultStats.lost = 0;

    CurrentProfile->CustomStats.totalSeconds = 0;
    CurrentProfile->CustomStats.won = 0;
    CurrentProfile->CustomStats.lost = 0;

    CurrentGame->exists = 0;
    CurrentGame->rows = 0;
    CurrentGame->columns = 0;
    for (i = 0; i < MAX_ROWS; i++) {
        for (j = 0; j < MAX_COLUMNS; j++) {
            CurrentGame->Board[i][j].state = 0;
            CurrentGame->Board[i][j].isFlagged = 0;
            CurrentGame->Board[i][j].isRevealed = 0;
        }
    }
    strcpy(CurrentGame->mode, "");
    strcpy(CurrentGame->outcome, "");
    CurrentGame->seconds = 0;

    CurrentProfile->RecentGame1.exists = 0;
    CurrentProfile->RecentGame2.exists = 0;
    CurrentProfile->RecentGame3.exists = 0;

    updateProfile(CurrentProfile);
}


/*
    @brief: initializes the values of the current leaderboard's statistics to default values

    @param: CurrentLeaderboard - pointer to the leaderboard struct containing information regarding
        the top records for each game mode
    		
    Precondition: MAX_RECORDS is accurate.
*/
void initializeLeaderboard(struct Leaderboard *CurrentLeaderboard) {
    int i;

    struct Records *EasyRecords = &CurrentLeaderboard->EasyRecords;
    struct Records *DifficultRecords = &CurrentLeaderboard->DifficultRecords;
    struct Records *CustomRecords = &CurrentLeaderboard->CustomRecords;

    EasyRecords->numRecords = 0;
    DifficultRecords->numRecords = 0;
    CustomRecords->numRecords = 0;

    for (i = 0; i < MAX_RECORDS; i++) {
        strcpy(EasyRecords->names[i], "");
        EasyRecords->times[i] = 0;

        strcpy(DifficultRecords->names[i], "");
        DifficultRecords->times[i] = 0;

        strcpy(CustomRecords->names[i], "");
        CustomRecords->times[i] = 0;
    }
}


/*
	@brief: extracts information from leaderboard.txt and stores it into leaderboard struct
	
	@param: CurrentLeaderboard - pointer to the current leaderboard struct that we want to store
        information into
    
    Precondition: The leaderboard text file is formatted correctly. LEADERBOARD_DIRECTORY is
        accurate.
*/
void loadLeaderboard(struct Leaderboard *CurrentLeaderboard) {
    FILE *fp;
    int i;

    fp = fopen(LEADERBOARD_DIRECTORY, "r");
    if (fp == NULL) {
        fp = fopen(LEADERBOARD_DIRECTORY, "w");
        fprintf(fp, "0\n\n0\n\n0\n");
        fclose(fp);

        loadLeaderboard(CurrentLeaderboard);
        return;
    }

    // easy leaderboard
    fscanf(fp, "%d", &CurrentLeaderboard->EasyRecords.numRecords);
    for (i = 0; i < CurrentLeaderboard->EasyRecords.numRecords; i++) {
        fscanf(fp, "%s", CurrentLeaderboard->EasyRecords.names[i]);
        fscanf(fp, "%d", &CurrentLeaderboard->EasyRecords.times[i]);
    }

    // difficult leaderboard
    fscanf(fp, "%d", &CurrentLeaderboard->DifficultRecords.numRecords);
    for (i = 0; i < CurrentLeaderboard->DifficultRecords.numRecords; i++) {
        fscanf(fp, "%s", CurrentLeaderboard->DifficultRecords.names[i]);
        fscanf(fp, "%d", &CurrentLeaderboard->DifficultRecords.times[i]);
    }
    
    // custom leaderboard
    fscanf(fp, "%d", &CurrentLeaderboard->CustomRecords.numRecords);
    for (i = 0; i < CurrentLeaderboard->CustomRecords.numRecords; i++) {
        fscanf(fp, "%s", CurrentLeaderboard->CustomRecords.names[i]);
        fscanf(fp, "%d", &CurrentLeaderboard->CustomRecords.times[i]);
    }

    fclose(fp);
}


/*
    @brief: clears the input buffer after a user response
*/
void clearInputBuffer() {
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


/*
	@brief: sorts the profile names in alphabetical order using the selection sort algorithm
	
	@param: ProfileNames - array containing the list of existing profiles
	@param: n - indicates how many profiles are in the array
*/
void sortProfileNames(string20 ProfileNames[], int n) {
    int i, j;
    int index;
    string20 temp;

    for (i = 0; i < n - 1; i++) {
        index = i;

        for (j = i + 1; j < n; j++) {
            if (strcmp(ProfileNames[j], ProfileNames[index]) < 0) {
                index = j;
            }
        }

        if (i != index) {
            strcpy(temp, ProfileNames[i]);
            strcpy(ProfileNames[i], ProfileNames[index]);
            strcpy(ProfileNames[index], temp);
        }
    }
}


/*
	@brief: stores the profiles in profiles directory into the ProfileNames[] array
	
	@param: ProfileNames - array of strings containing the existing profile names
	
    Precondition: PROFILES_DIRECTORY is accurate.

	@return: the number of profiles successfully read from the profiles text file
*/
int getProfileNames(string20 ProfileNames[]) {
    FILE *fp;
    string20 name;
    int numProfiles = 0;

    fp = fopen(PROFILES_DIRECTORY, "r");

    if (fp == NULL) {
        fp = fopen(PROFILES_DIRECTORY, "w");
        fclose(fp);
        return getProfileNames(ProfileNames);
    }

    while (fscanf(fp, "%s", name) == 1) {
        if (strcmp(name, "") != 0) {
            strcpy(ProfileNames[numProfiles++], name);
        }
    }

    sortProfileNames(ProfileNames, numProfiles);

    fclose(fp);
    return numProfiles;
}


/*
    @brief: prints a divider consisting of several dashes to aid in display formatting
*/
void printDivider() {
    printf(" \x1b[1m\x1b[37m----------------------------------------------------------------------------\x1b[0m");
}


/*
	@brief: prints a list of existing profiles
	
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
*/
void printProfiles(int theme) {
    int i;
    string20 profileNames[MAX_PROFILES];
    int numProfiles = getProfileNames(profileNames);
	
	printEvade(theme);

    printf("\n Here are the existing profiles:\n\n");
    for (i = 0; i < numProfiles; i++) {
        printf(" %d.) %s\n", i + 1, profileNames[i]);
    }
}


/*
	@brief: capitalizes all the letters of a string
	
	@param: string - the string to be capitalized

    Precondition: The string only consists of letters from the English alphabet.
*/
void toUpperCaseString(string20 string) {
    for (int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
}


/*	
	@brief: checks if a particular profile exists
	
	@param: name - the name being checked
	
	@return: 1 - profile exists
			 0 - profile does not exist
	
*/
int profileExists(string20 name) {
    int i;
    string20 ProfileNames[10];
    int numProfiles = getProfileNames(ProfileNames);

    toUpperCaseString(name);

    for (i = 0; i < numProfiles; i++) {
        if (strcmp(name, ProfileNames[i]) == 0) {
            return 1;
        }
    }

    return 0;
}


/*
	@brief: extracts information from the current profile's text file and stores it into the current
        profile structure
	
	@param: CurrentProfile - pointer to the structure holding the current profile information
	@param: name - name of the profile
	
	Precondition: The profile text file being loaded stores the profile information in a correct,
        accurate format.
	
*/
void loadProfile(struct Profile *CurrentProfile, string20 name) {
    FILE *fp;
    int i, j;
    int code;

    struct Game *RecentGame1 = &CurrentProfile->RecentGame1;
    struct Game *RecentGame2 = &CurrentProfile->RecentGame2;
    struct Game *RecentGame3 = &CurrentProfile->RecentGame3;

    string100 directory = "profiles\\";
    strcat(directory, name);
    strcat(directory, ".txt");

    fp = fopen(directory, "r");
    if (fp == NULL) {
        CurrentProfile->creationDate = getDateCode();
        initializeProfile(CurrentProfile, name);
        loadProfile(CurrentProfile, name);
        return;
    }

    // current profile information
    fscanf(fp, "%s", CurrentProfile->name);
    fscanf(fp, "%d", &CurrentProfile->creationDate);
    fscanf(fp, "%d", &CurrentProfile->lifetimeGames);

    // easy statistics
    fscanf(fp, "%d", &CurrentProfile->EasyStats.totalSeconds);
    fscanf(fp, "%d", &CurrentProfile->EasyStats.won);
    fscanf(fp, "%d", &CurrentProfile->EasyStats.lost);

    // difficult statistics
    fscanf(fp, "%d", &CurrentProfile->DifficultStats.totalSeconds);
    fscanf(fp, "%d", &CurrentProfile->DifficultStats.won);
    fscanf(fp, "%d", &CurrentProfile->DifficultStats.lost);

    // custom statistics
    fscanf(fp, "%d", &CurrentProfile->CustomStats.totalSeconds);
    fscanf(fp, "%d", &CurrentProfile->CustomStats.won);
    fscanf(fp, "%d", &CurrentProfile->CustomStats.lost);

    // recent game 1
    fscanf(fp, "%d", &RecentGame1->exists);
    if (RecentGame1->exists) { 
        fscanf(fp, "%d", &RecentGame1->rows);
        fscanf(fp, "%d", &RecentGame1->columns);

        // recent game 1 board
        for (i = 0; i < RecentGame1->rows; i++) {
            for (j = 0; j < RecentGame1->columns; j++) {
                fscanf(fp, "%d", &code);
                RecentGame1->Board[i][j].state = code / 100;
                RecentGame1->Board[i][j].isFlagged = code / 10 % 10;
                RecentGame1->Board[i][j].isRevealed = code % 10;
            }
        }

        fscanf(fp, "%s", RecentGame1->mode);
        fscanf(fp, "%s", RecentGame1->outcome);
        fscanf(fp, "%d", &RecentGame1->seconds);
    }

    // recent game 2
    fscanf(fp, "%d", &RecentGame2->exists);
    if (RecentGame2->exists) { 
        fscanf(fp, "%d", &RecentGame2->rows);
        fscanf(fp, "%d", &RecentGame2->columns);

        // recent game 2 board
        for (i = 0; i < RecentGame2->rows; i++) {
            for (j = 0; j < RecentGame2->columns; j++) {
                fscanf(fp, "%d", &code);
                RecentGame2->Board[i][j].state = code / 100;
                RecentGame2->Board[i][j].isFlagged = code / 10 % 10;
                RecentGame2->Board[i][j].isRevealed = code % 10;
            }
        }

        fscanf(fp, "%s", RecentGame2->mode);
        fscanf(fp, "%s", RecentGame2->outcome);
        fscanf(fp, "%d", &RecentGame2->seconds);
    }

    // recent game 3
    fscanf(fp, "%d", &RecentGame3->exists);
    if (RecentGame3->exists) { 
        fscanf(fp, "%d", &RecentGame3->rows);
        fscanf(fp, "%d", &RecentGame3->columns);

        // recent game 3 board
        for (i = 0; i < RecentGame3->rows; i++) {
            for (j = 0; j < RecentGame3->columns; j++) {
                fscanf(fp, "%d", &code);
                RecentGame3->Board[i][j].state = code / 100;
                RecentGame3->Board[i][j].isFlagged = code / 10 % 10;
                RecentGame3->Board[i][j].isRevealed = code % 10;
            }
        }

        fscanf(fp, "%s", RecentGame3->mode);
        fscanf(fp, "%s", RecentGame3->outcome);
        fscanf(fp, "%d", &RecentGame3->seconds);
    }
}


/*
    @brief: prompts the user to press enter to return to the main menu
*/
void pressEnter() {
    printf(" Press 'Enter' to return to the Main Menu.");
    clearInputBuffer();
}


/*
	@brief: prompts the user to select an existing profile from the profile directory
	
	@param: CurrentProfile - pointer to the structure holding the current profile information
    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: The list of profiles is accurate.
*/
void selectProfile(struct Profile *CurrentProfile, int theme) {
    string20 profile;
    int exists;

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();
        printf("\n");

        printProfiles(theme);

        printf("\n");
        printDivider();

        printf("\n\n Enter an existing profile to load (or '0' to return to the main menu): ");
        scanf("%s", profile);
        clearInputBuffer();

        if (strcmp(profile, "0") == 0) {
            return;
        }

        exists = profileExists(profile);
        if (!exists) {
            printf("\n The profile '%s' does not exist.", profile);
        }
        
    } while (!exists);

    // account change processing
    remove("profiles\\GUEST.txt");

    Sleep(SHORT_SLEEP);
    printf("\n Successful.");
    Sleep(SHORT_SLEEP);

    printf(" The current profile has been changed from '%s' to '%s'.", CurrentProfile->name, profile);
    loadProfile(CurrentProfile, profile); // load the selected profile's information
    Sleep(LONG_SLEEP);
    
    printf("\n\n");
    pressEnter();
}


/*
    @brief: checks if a profile name is valid
	
	@param: name - name of the profile being created
	
	@return: 1 - valid profile name
			 0 - invalid profile name
*/
int isValidProfileName(string20 name) {
    int i;
    int strLen = strlen(name);
    int hasGoodLength, isAlphabetic, isNotTaken, isNotReserved;

    toUpperCaseString(name);

    // the name has between 3 to 20 characters
    hasGoodLength = strLen >= 3 && strLen <= 20;

    // the name only contains letters of the English alphabet
    isAlphabetic = 1;
    for (i = 0; i < strLen; i++) {
        if (!isalpha(name[i])) {
            isAlphabetic = 0;
        }
    }

    // the name has not yet been taken
    isNotTaken = !profileExists(name);

    // the name has not yet been reserved for the program
    isNotReserved = strcmp(name, "GUEST") != 0;

    // inform the user of the naming requirements
    if (!hasGoodLength) {
        printf("\tThe name must contain between 3 to 20 characters!\n");
        Sleep(SHORT_SLEEP);
    }
    if (!isAlphabetic) {
        printf("\tThe name must only contain letters from the English alphabet!\n");
        Sleep(SHORT_SLEEP);
    }
    if (!isNotTaken) {
        printf("\tThe name '%s' has already been taken. Please provide another name!\n", name);
        Sleep(SHORT_SLEEP);
    }
    if (!isNotReserved) {
        printf("\tThe name '%s' has already been reserved for the program. Please provide another name!\n", name);
        Sleep(SHORT_SLEEP);
    }

    return hasGoodLength && isAlphabetic && isNotTaken && isNotReserved;
}


/*
    @brief: guides the user in creating a new profile, then creates the corresponding text file and
        updates the list of profiles
	
	@param: CurrentProfile - pointer to the current profile struct holding the current profile's
        information
	
	@return: 1 - successfully created a profile
			 0 - a profile was not created

    Precondition: MAX_PROFILES is accurate.
*/
int createProfile(struct Profile *CurrentProfile, int theme) {
    FILE *fp;
    string20 name;
    string20 ProfileNames[10];

    Sleep(SHORT_SLEEP);

    // restrict the user from creating a profile if there are at least 10 existing ones
    if (getProfileNames(ProfileNames) >= MAX_PROFILES) {
        printf("\n Sorry, you cannot create a new profile as there can only be a maximum of ");
        printf("%d profiles.", MAX_PROFILES);
        Sleep(LONG_SLEEP);

        printf("\n\n");
        pressEnter();
        return 0;
    }

    system("cls");

    printf("\n");
    printDivider();
    printf("\n");

    printProfiles(theme);

    printf("\n");
    printDivider();
    printf("\n\n");

    printf(" Requirements:\n\n");
    printf(" 1.) The name must contain between 3 to 20 characters.\n");
    printf(" 2.) The name must only contain letters from the English alphabet.\n");
    printf(" 3.) The name must not have already been taken by an existing profile.\n\n");

    printDivider();
    printf("\n");

    do {
        printf("\n Enter a valid name for your profile (or '0' to return to the main menu): ");
        scanf("%s", name);
        clearInputBuffer();

        if (strcmp(name, "0") == 0) return 0;
    } while (!isValidProfileName(name));
    
    // initialize the account information, as well as their creation date
    CurrentProfile->creationDate = getDateCode();
    initializeProfile(CurrentProfile, name);

    fp = fopen(PROFILES_DIRECTORY, "a");
    fprintf(fp, "%s\n", CurrentProfile->name);
    fclose(fp);

    Sleep(SHORT_SLEEP);
    printf("\n Successful.");
    Sleep(SHORT_SLEEP);

    printf(" The profile '%s' has been created.", name);
    Sleep(LONG_SLEEP);
    
    printf("\n\n");
    pressEnter();

    return 1;
}


/*
    @brief: prompts the user to confirm their action
	
	@return: 1 - user confirms
			 0 - user does not confirm
*/
int confirmAction() {
    char c;

    do {
        printf("\n Yes [Y] or No [N]: ");
        scanf("%c", &c);
        clearInputBuffer();
    } while (!(c == 'Y' || c == 'N'));

    return c == 'Y';
}


/*
    @brief: guides the user in deleting an existing profile; restricts them from deleting the
        current profile

    @param: name - name of the current profile
    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: The list of profile is accurate. PROFILES_DIRECTORY is accurate.
*/
void deleteProfile(string20 name, int theme) {
    FILE *fp;
    int i;
    int exists, isCurrent;

    string100 directory = "profiles\\";
    string20 profile;
    string20 ProfileNames[10];
    int numProfiles = getProfileNames(ProfileNames);

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();
        printf("\n");

        printProfiles(theme);

        printf("\n");
        printDivider();

        printf("\n\n Enter an existing profile to delete (or '0' to return to the main menu): ");
        scanf("%s", profile);
        toUpperCaseString(profile);
        clearInputBuffer();
        
        if (strcmp(profile, "0") == 0) return;

        exists = profileExists(profile);
        if (!exists) {
            printf("\n The profile '%s' does not exist.", profile);
        }

        isCurrent = strcmp(profile, name) == 0;
        if (isCurrent) {
            printf("\n You cannot delete '%s' as it is the current profile!", profile);
        }
    } while (!exists || isCurrent);
    
    printf("\n Are you sure you want to delete the profile '%s'?\n", profile);

    if (!confirmAction()) return;

    fp = fopen(PROFILES_DIRECTORY, "w");

    for (i = 0; i < numProfiles; i++) {
        if (strcmp(ProfileNames[i], profile) != 0) {
            fprintf(fp, "%s\n", ProfileNames[i]);
        }
    }

    fclose(fp);

    strcat(directory, profile);
    strcat(directory, ".txt");
    remove(directory);

    Sleep(SHORT_SLEEP);
    printf("\n Successful.");
    Sleep(SHORT_SLEEP);

    printf(" The profile '%s' has been deleted.", profile);
    Sleep(LONG_SLEEP);
    
    printf("\n\n");
    pressEnter();
}


/*
    @brief: guides the user in selecting, creating, or deleting profiles
	
	@param: CurrentProfile - pointer to the current profile structure holding the current profile
        information
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
	
	Precondition: The list of profiles is accurate.
*/
void profileHandler(struct Profile *CurrentProfile, int theme) {
    char userResponse;
    string20 ProfileNames[10];
    
    getProfileNames(ProfileNames);

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();
        printf("\n\n");
		
		printMenu(theme);
		
        printf("\n\n Current profile: %s\n\n", CurrentProfile->name);
        printf(" [a] Select an Existing Profile\n [b] Create a New Profile\n [c] Delete an Existing Profile\n\n");

        printDivider();

        printf("\n\n Please input the letter corresponding to your action (a/b/c): ");
        scanf("%c", &userResponse);
        clearInputBuffer();
    } while (!(userResponse >= 'a' && userResponse <= 'c'));

    if (userResponse == 'a') { // user wants to select an existing profile
        selectProfile(CurrentProfile, theme);
    }
    else if (userResponse == 'b') { // user wants to create a new profile
        if (createProfile(CurrentProfile, theme)) {
            remove("profiles\\GUEST.txt");
        }
    }
    else if (userResponse == 'c') { // user wants to delete an existing profile
        deleteProfile(CurrentProfile->name, theme);
        if (!profileExists(CurrentProfile->name)) {
            initializeProfile(CurrentProfile, "GUEST");
        }
    }
}


/*
    @brief: generates a pseudorandom integer between a range of numbers

    @param: min - minimum number in the range
    @param: max - maximum number in the range

    @return: pseudorandom-generated integer

    Precondition: assumes parameters are nonnegative and min is less than or equal to max
*/
int getRandInt(int min, int max) {
    return min + rand() % (max - min + 1); // randomly generates an integer in the interval [min, max]
}


/*
    @brief: generates a easy/difficult classic game
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
    @param: mineLocations - array of integers representing mine locations
	@param: rows - indicates the number of rows of the board (8 - easy, 10 - difficult)
	@param: columns - indicates the number of columns of the board (8 - easy, 15 - difficult)
	@param: numMines - indicates the number of mines (10 - easy, 35 - difficult)

    Precondition: The user intended to select classic mode.
*/
void generateClassicGame(struct Tile Board[][15], int mineLocations[], int rows, int columns, int numMines) {
    int i;
    int mineCount = 0;
    int isValid;
    int randRow, randColumn;
    int mineRow, mineColumn;

    // randomly generate the mines
    while (mineCount < numMines) {
        isValid = 1;
        randRow = getRandInt(0, rows - 1);
        randColumn = getRandInt(0, columns - 1);

        for (i = 0; i < mineCount; i++) {
            mineRow = mineLocations[i] / 100;
            mineColumn = mineLocations[i] % 100;

            if (randRow == mineRow && randColumn == mineColumn) {
                isValid = 0;
            }
        }

        if (isValid) {
            Board[randRow][randColumn].state = 9;
            mineLocations[mineCount++] = randRow * 100 + randColumn;
        }
    }
}


/*
    @brief: prints a list of the existing custom levels

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
*/
void printLevels(int theme) {
    FILE *fp;
    int i;
    int numLevels;
    string20 level;

    fp = fopen(LEVELS_DIRECTORY, "r");

    if (fp == NULL) {
        fp = fopen(LEVELS_DIRECTORY, "w");
        fclose(fp);

        printLevels(theme);
        return;
    }

    fscanf(fp, "%d", &numLevels);

    printf("\n");
	printEvade(theme);

    printf("\n Here are the existing custom levels:\n\n");
    for (i = 0; i < numLevels; i++) {
        fscanf(fp, "%s", level);
        printf(" %d.) %s\n", i + 1, level);
    }

    fclose(fp);
}


/*
    @brief: guides the user in generating a custom game
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
	@param: rows - pointer to the current profile's current game struct variable storing the current
        game's rows
	@param: columns - pointer to the current profile's current game struct variable storing the current
        game's columns
    @param: mineLocations - array of integers representing mine locations
    @param: numMines - pointer to a variable storing the current game's number of mines
    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
	
	@return: 1 - successfully generated a custom game
			 0 - unsuccessfully generated a custom game

    Precondition: The list of levels is accurate.
*/
int generateCustomGame(struct Tile Board[][15], int *rows, int *columns, int mineLocations[], int *numMines, int theme) {
    FILE *fp;
    int i, j;
    char c;

    string100 directory;
    string100 name;

    *numMines = 0;

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();

        printLevels(theme);

        printf("\n");
        printDivider();
        printf("\n\n");

        printf(" Enter an existing level to load (or '0' to return to the main menu): ");
        scanf("%s", name);
        clearInputBuffer();

        if (strcmp(name, "0") == 0) return 0;

        strcpy(directory, "levels\\");
        strcat(directory, name);
        strcat(directory, ".txt");

        fp = fopen(directory, "r");
        if (fp == NULL) {
            printf("\n The level '%s' does not exist!", name);
        }
    } while (fp == NULL);

    fscanf(fp, "%d %d", rows, columns);

    for (i = 0; i < *rows; i++) {
        for (j = 0; j < *columns; j++) {
            fscanf(fp, "%c", &c);

            if (c == 'X') {
                Board[i][j].state = 9;
                mineLocations[(*numMines)++] = i * 100 + j;
            }
            else if (!(c == 'X' || c == '.')) { // some special character was encountered
                j--;
            }
        }
    }

    Sleep(SHORT_SLEEP);
    fclose(fp);
    return 1;
}


/*
    @brief: increments a tile's state if it is within bounds and is not a mine
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
    @param: row - the row being incremented
	@param: column - the column being incremented
	@param: numRows - the current board's number of rows; indicates if the row value is valid
	@param: numColumns - the current board's number of columns; indicates if the column value is
        valid
	
	Precondition: The current tile's information is accurate. numRows and numColumns are less than
        MAX_ROWS AND MAX_COLUMNS, respectively.

*/
void incrementTileState(struct Tile Board[][15], int row, int column, int numRows, int numColumns) {
    int isValidRow = row >= 0 && row <= numRows - 1;
    int isValidColumn = column >= 0 && column <= numColumns - 1;
    int isNotMine = Board[row][column].state != 9;

    if (isValidRow && isValidColumn && isNotMine) { // valid tile to increment
        Board[row][column].state++;
    }
}


/*
    @brief: initializes the states of the entire board based on the locations of all the mines
		  
	@param: Board - a 2-dimensional array of tiles representing the current game board
    @param: mineLocations - array of integers representing mine locations
	@param: rows - number of rows of the board
	@param: columns - number of columns of the board
	@param: numMines - number of mines on the board
	
	Precondition: mineLocations contains the locations of all the mines.
*/
void initializeTileStates(struct Tile Board[10][15], int mineLocations[], int rows, int columns, int numMines) {
    int i;
    int mineRow, mineColumn;

    for (i = 0; i < numMines; i++) {
        mineRow = mineLocations[i] / 100;
        mineColumn = mineLocations[i] % 100;

        incrementTileState(Board, mineRow - 1, mineColumn - 1, rows, columns); // tile northwest
        incrementTileState(Board, mineRow - 1, mineColumn, rows, columns); // tile north
        incrementTileState(Board, mineRow - 1, mineColumn + 1, rows, columns); // tile northeast
        incrementTileState(Board, mineRow, mineColumn - 1, rows, columns); // tile west
        incrementTileState(Board, mineRow, mineColumn + 1, rows, columns); // tile east
        incrementTileState(Board, mineRow + 1, mineColumn - 1, rows, columns); // tile southwest
        incrementTileState(Board, mineRow + 1, mineColumn, rows, columns); // tile south
        incrementTileState(Board, mineRow + 1, mineColumn + 1, rows, columns); // tile southeast
    }
}


/*
    @brief: prints a game board given information about it
	
	@param: Board - a 2-dimensional array of tiles representing a game board
	@param: rows - number of rows of the board
	@param: columns - number of columns of the board
	@param: currRow - indicates the row of the cursor ( > < )
	@param: currColumn - indicates the column of the cursor ( > < )
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: currRow and currColumn are positive and within range if the user's cursor is to
        be printed.
*/
void printBoard(struct Tile Board[][15], int rows, int columns, int currRow, int currColumn, int theme) {
    int i, j, k;
    int state;

    // print the column numbers
    printf("\n%7d", 1);
    for (i = 2; i <= columns; i++)
        printf("%4d", i);
    printf("\n");

    // print the upper border
    printf(" \x1b[1m\x1b[30m%4c%c%c%c\x1b[0m", 218, 196, 196, 196);
    for (i = 0; i < columns - 1; i++)
        printf("\x1b[1m\x1b[30m%c%c%c%c\x1b[0m", 194, 196, 196, 196);
    printf("\x1b[1m\x1b[30m%c\x1b[0m\n", 191);

    // print the body
    for (i = 0; i < rows; i++) {
        printf(" %-3d", i + 1);

        for (j = 0; j < columns; j++) {
            state = Board[i][j].state;

            printf("\x1b[1m\x1b[30m%c\x1b[0m", 179);

            if (i == currRow && j == currColumn) {
                printf("\x1b[1m\x1b[37m>\x1b[0m");
            }
            else {
                printf(" ");
            }
            
            if (Board[i][j].isRevealed) { // tile is revealed; negate to reveal/hide
                if (state == 0) { // empty; no nearby mines
                    printf(" ");
                }
                else if (state >= 1 && state <= 8) { // 1 - 8 nearby mines
                    printf("\x1b[1m\x1b[37m%d\x1b[0m", state);
                }
                else if (state == 9) { // mine
                    printf("\x1b[1m\x1b[31m%c\x1b[0m", 233);
                }
                else if (state == 10) { // exploded mine
                    printf("\x1b[1m\x1b[31m%c\x1b[0m", 254);
                }
            }
            else if (Board[i][j].isFlagged) { // tile is flagged
                printf("\x1b[1m\x1b[37m%c\x1b[0m", 219);
            }
            else { // tile is not yet inspected
				if (theme == 1) { // cyan
                	printf("\x1b[1m\x1b[36m%c\x1b[0m", 219);
            	}
            	else if (theme == 2) { // bright red
            		printf("\x1b[1m\x1b[31m%c\x1b[0m", 219);
				}
				else if (theme == 3) { // bright green
					printf("\x1b[1m\x1b[32m%c\x1b[0m", 219);
				}
                else if (theme == 4) { // dark purple
                    printf("\x1b[1m\x1b[35m%c\x1b[0m", 219);
                }
            }

            if (i == currRow && j == currColumn) {
                printf("\x1b[1m\x1b[37m<\x1b[0m");
            }
            else {
                printf(" ");
            }
        }

        printf("\x1b[1m\x1b[30m%c\x1b[0m\n", 179);

        if (i < rows - 1) {
            // print the middle border
            printf(" \x1b[1m\x1b[30m%4c%c%c%c\x1b[0m", 195, 196, 196, 196);
            for (k = 0; k < columns - 1; k++)
                printf("\x1b[1m\x1b[30m%c%c%c%c\x1b[0m", 197, 196, 196, 196);
            printf("\x1b[1m\x1b[30m%c\x1b[0m\n", 180);
        }
    }

    // print the lower border
    printf(" \x1b[1m\x1b[30m%4c%c%c%c\x1b[0m", 192, 196, 196, 196);
    for (i = 0; i < columns - 1; i++)
        printf("\x1b[1m\x1b[30m%c%c%c%c\x1b[0m", 193, 196, 196, 196);
    printf("\x1b[1m\x1b[30m%c\x1b[0m\n", 217);
}


/*
    @brief: detects key presses, allowing the user to utilize keys to navigate through the board
	
	@param: currRow - pointer to the current row position of the cursor ( > < )
	@param: currColumn - pointer to the current column position of the cursor ( > < )
	@param: rows - number of rows of the board, also serves as a limit for out of bounds cursor
	@param: columns - number of columns of the board, also serves as a limit for out of bounds cursor
	
	@return: the ASCII value of the pressed key
*/
int detectKeyPress(int *currRow, int *currColumn, int rows, int columns) {
    int key = getch();

    if (key == ARROW_VALUE) { // arrow key press
        key = getch();

        if (key == UP_VALUE) { // up arrow key
            if (*currRow != 0) {
                (*currRow)--;
            }
        }
        else if (key == DOWN_VALUE) { // down arrow key
            if (*currRow != rows - 1) {
                (*currRow)++;
            }
        }
        else if (key == LEFT_VALUE) { // left arrow key
            if (*currColumn != 0) {
                (*currColumn)--;
            }
        }
        else if (key == RIGHT_VALUE) { // right arrow key
            if (*currColumn != columns - 1) {
                (*currColumn)++;
            }
        }
    }

    return key;
}


/*
    @brief: reveals a tile; continues revealing tiles adjacent to it if it is a blank
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
	@param: rows - number of rows of the board; helps determine if tile is out of bounds
	@param: columns - number of columns of the board; helps determine if tile is out of bounds
	@param: row - the row of the tile inspected
	@param: column - the column of the tile inspected
	
	Precondition: The board information is accurate.
*/
void revealTiles(struct Tile Board[][15], int rows, int columns, int row, int column) {
    int validRow = row >= 0 && row <= rows - 1;
    int validColumn = column >= 0 && column <= columns - 1;

    if (!validRow || !validColumn) return;
    if (Board[row][column].isRevealed) return;
    Board[row][column].isRevealed = 1;
    if (Board[row][column].state != 0) return;

    // at this point, the tile is confirmed to be an unrevealed blank, triggering the cascading
    revealTiles(Board, rows, columns, row - 1, column - 1); // tile northwest
    revealTiles(Board, rows, columns, row - 1, column); // tile north
    revealTiles(Board, rows, columns, row - 1, column + 1); // tile northeast
    revealTiles(Board, rows, columns, row, column - 1); // tile west
    revealTiles(Board, rows, columns, row, column + 1); // tile east
    revealTiles(Board, rows, columns, row + 1, column - 1); // tile southwest
    revealTiles(Board, rows, columns, row + 1, column); // tile south
    revealTiles(Board, rows, columns, row + 1, column + 1); // tile southeast
}


/*
	@brief: determines the current state of the game (ongoing/won/lost)
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
	@param: rows - indicates the number of rows
	@param: column - indicates the number of columns
	@param: mineLocations - array of integers representing mine locations
	@param: numMines - indicates the total number of mines
	
	@return: 0 - game is ongoing
			 1 - won
			 2 - lost
*/
int getGameState(struct Tile Board[][15], int rows, int columns, int mineLocations[], int numMines) {
    int i, j;
    int mineRow, mineColumn;

    // check if a mine has been inspected
    for (i = 0; i < numMines; i++) {
        mineRow = mineLocations[i] / 100;
        mineColumn = mineLocations[i] % 100;

        if (Board[mineRow][mineColumn].isRevealed) { // a mine has been revealed
            Board[mineRow][mineColumn].state = 10;
            return 2; // game is lost
        }
    }

    // check if all non-mines have been inspected
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            if (Board[i][j].state != 9 && !Board[i][j].isRevealed) // not all non-mine tiles have been revealed
                return 0;  // game is ongoing
        }
    }

    return 1; // all non-mine tiles have been revealed; game is won
}


/*
    @brief: sets the isRevealed value of all mines to a particular value; effectively hides/reveals
        all mines
	
	@param: visibility - new value that all mines' isRevealed members will be set to
	@param: Board - a 2-dimensional array of tiles representing the current game board
	@param: mineLocations - array of integers representing mine locations
	@param: numMines - total number of mines

    Precondition: Assumes visibility is 1 or 0.
*/
void setMineVisibility(int visibility, struct Tile Board[][15], int mineLocations[], int numMines) {
    int i;
    int mineRow, mineColumn;

    for (i = 0; i < numMines; i++) {
        mineRow = mineLocations[i] / 100;
        mineColumn = mineLocations[i] % 100;

        Board[mineRow][mineColumn].isRevealed = visibility; // reveal the mine
    }
}


/*
    @brief: uses the binary insertion sort algorithm to sort an array of record times for the
        leaderboard

    @param: name - name of the potential record setter
    @param: time - time of the potential record setter
    @param: names - array of record-setting names for the leaderboard being updated
    @param: times - array of record-setting times (in seconds) for the leaderboard being updated
    @param: numRecords - pointer to the number of records currently existing in the leaderboard

    @return: 1 to MAX_RECORDS - the rank of the new record set
             0 - no new records set

    Precondition: numRecords is less than or equal to MAX_RECORDS. times is arranged in increasing
        order and names contains names corresponding to the times they set.
*/
int updateRecord(string20 name, int time, string20 names[], int times[], int *numRecords) {
    int i;
    int index = 0;
    int high = *numRecords - 1;
    int mid;
	
    while (index <= high) { // determines the index of the first element less than time
        mid = (index + high) / 2;

        if (times[mid] <= time) {
            index = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    if (index == MAX_RECORDS) return -1;

    for (i = MAX_RECORDS - 1; i > index; i--) { // shifts the lower records to the right
        strcpy(names[i], names[i - 1]);
        times[i] = times[i - 1];
    }

    strcpy(names[index], name); // adds the current information as a record
    times[index] = time;

    if (*numRecords < MAX_RECORDS) (*numRecords)++; // ensures the number of records won't pass MAX_RECORDS

    return index + 1; // the rank of the newly set record; 0 if no record is set
}


/*
    @brief: updates the leaderboard structure then the text file based on the structure information
	
	@param: mode - the recently concluded game's mode
    @param: outcome - the recently concluded game's outcome
	@param: name - the player name of the recently concluded game
	@param: seconds - the recently concluded game's time in seconds
	@param: CurrentLeaderboard - pointer to the current leaderboard struct that we want to update
	
	@return: 0 - game did not set a new record | leaderboard file failed to open
			 rank - rank of the new record set

    Precondition: LEADERBOARD_DIRECTORY is accurate. rank obtained from updateRecord is accurate.
*/
int updateLeaderboard(string20 mode, string20 outcome, string20 name, int seconds, struct Leaderboard *CurrentLeaderboard) {
    int i;
    int rank;
    FILE *fp;

    if (strcmp(outcome, WON_OUTCOME) != 0) return 0;

    struct Records *EasyRecords = &CurrentLeaderboard->EasyRecords;
    struct Records *DifficultRecords = &CurrentLeaderboard->DifficultRecords;
    struct Records *CustomRecords = &CurrentLeaderboard->CustomRecords;

    if (strcmp(mode, EASY_MODE) == 0) { // update easy leaderboard
        rank = updateRecord(name, seconds, EasyRecords->names, EasyRecords->times, &EasyRecords->numRecords);
    }
    else if (strcmp(mode, DIFFICULT_MODE) == 0) { // update difficult leaderboard
        rank = updateRecord(name, seconds, DifficultRecords->names, DifficultRecords->times,
            &DifficultRecords->numRecords);
    }
    else if (strcmp(mode, CUSTOM_MODE) == 0) { // update custom leaderboard
        rank = updateRecord(name, seconds, CustomRecords->names, CustomRecords->times, &CustomRecords->numRecords);
    }

    fp = fopen(LEADERBOARD_DIRECTORY, "w");
    if (fp == NULL) return 0;

    fprintf(fp, "%d\n", EasyRecords->numRecords);
    for (i = 0; i < EasyRecords->numRecords; i++) {
        fprintf(fp, "%s %d\n", EasyRecords->names[i], EasyRecords->times[i]);
    }

    fprintf(fp, "\n%d\n", DifficultRecords->numRecords);
    for (i = 0; i < DifficultRecords->numRecords; i++) {
        fprintf(fp, "%s %d\n", DifficultRecords->names[i], DifficultRecords->times[i]);
    }

    fprintf(fp, "\n%d\n", CustomRecords->numRecords);
    for (i = 0; i < CustomRecords->numRecords; i++) {
        fprintf(fp, "%s %d\n", CustomRecords->names[i], CustomRecords->times[i]);
    }

    fclose(fp);
    return rank;
}


/*
    @brief: Guides the user in game mode selection and game proper. Handles the underlying game
        generation process.
	
	@param: CurrentProfile - pointer to the current profile structure storing current profile
        information
	@param: CurrentLeaderboard - pointer to the current leaderboard struct that we want to update
        after a game has concluded
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
	
	Precondition: The user chose to begin a game from the main menu.
*/
void gameHandler(struct Profile *CurrentProfile, struct Leaderboard *CurrentLeaderboard, int theme) {
    char userResponse;
    int keyValue;
    int mineLocations[35];
    struct Game *CurrentGame = &CurrentProfile->CurrentGame;

    int mines = 0;
    int gameState = 0;
    int currRow = 0;
    int currColumn = 0;

    time_t startTime, endTime;
    int timeTaken;
    int rank;

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();
        printf("\n");
		
		printEvade(theme);
        printf("\n");
		
        printf(" There are two game options:\n\n");
        printf(" [a] Classic Game\n [b] Custom Game\n\n");

        printDivider();
        printf("\n\n");

        printf(" Please input the letter corresponding to your choice (a/b): ");

        scanf("%c", &userResponse);
        clearInputBuffer();
    } while (!(userResponse == 'a' || userResponse == 'b'));

    if (userResponse == 'a') { // Classic Game
        do {
            Sleep(SHORT_SLEEP);
            system("cls");

            printf("\n");
            printDivider();
            printf("\n");
            
            printEvade(theme);
            printf("\n");
            
            printf(" There are two difficulties:\n\n");
            printf(" [a] Easy\n [b] Difficult\n\n");

            printDivider();
            printf("\n\n");

            printf(" Please input the letter corresponding to your choice (a/b): ");

            scanf("%c", &userResponse);
            clearInputBuffer();
        } while (!(userResponse == 'a' || userResponse == 'b'));

        if (userResponse == 'a') { // easy board
            strcpy(CurrentGame->mode, EASY_MODE);
            CurrentGame->rows = 8;
            CurrentGame->columns = 8;
            mines = 10;
        }
        else if (userResponse == 'b') { // difficult board
            strcpy(CurrentGame->mode, DIFFICULT_MODE);
            CurrentGame->rows = 10;
            CurrentGame->columns = 15;
            mines = 35;
        }

        generateClassicGame(CurrentGame->Board, mineLocations, CurrentGame->rows, CurrentGame->columns, mines);
    }
    else if (userResponse == 'b') { // Custom Game
        if (generateCustomGame(CurrentGame->Board, &CurrentGame->rows, &CurrentGame->columns, mineLocations, &mines, theme)) {
            strcpy(CurrentGame->mode, CUSTOM_MODE);
        }
        else return;
    }
    
    initializeTileStates(CurrentGame->Board, mineLocations, CurrentGame->rows, CurrentGame->columns, mines);

    Sleep(SHORT_SLEEP);
    time(&startTime); // start tracking the time

    while (gameState == 0) { // run the game while the user has not yet won/lost/quit
        do {
            system("cls");

            printEvade(theme);
            printBoard(CurrentGame->Board, CurrentGame->rows, CurrentGame->columns, currRow, currColumn, theme);
            printf("\n Navigate the game board with your arrow keys.\n\n");
            printf(" Press 'Enter' to select your action. Press 'Esc' to quit the game.");

            keyValue = detectKeyPress(&currRow, &currColumn, CurrentGame->rows, CurrentGame->columns);
            Sleep(BOARD_REFRESH);

            if (keyValue == ESCAPE_VALUE) { // user wants to quit
                printf("\n\n Are you sure you want to quit?\n");
                
                if (confirmAction()) { // user confirms quitting
                    gameState = 3;
                }
                else {
                    keyValue = 0;
                }
            }
        } while (!(keyValue == ENTER_VALUE || keyValue == ESCAPE_VALUE)); // wait for an enter key press

        system("cls");
        printEvade(theme);
        printBoard(CurrentGame->Board, CurrentGame->rows, CurrentGame->columns, currRow, currColumn, theme);

        if (gameState != 3) { // user has not quit
            do {
                system("cls");

                printEvade(theme);
                printBoard(CurrentGame->Board, CurrentGame->rows, CurrentGame->columns, currRow, currColumn, theme);
                printf("\n Inspect [I], Flag [F], Remove Flag [R], or Unselect [U]: ");

                scanf("%c", &userResponse);
                clearInputBuffer();
            } while (!(userResponse == 'I' || userResponse == 'F' || userResponse == 'R' || userResponse == 'U'));

            if (userResponse == 'I') { // user chose to inspect
                revealTiles(CurrentGame->Board, CurrentGame->rows, CurrentGame->columns, currRow, currColumn);
            }
            else if (userResponse == 'F') { // user chose to flag
                CurrentGame->Board[currRow][currColumn].isFlagged = 1;
            }
            else if (userResponse == 'R') { // user chose to remove a flag
                CurrentGame->Board[currRow][currColumn].isFlagged = 0;
            }

            gameState = getGameState(CurrentGame->Board, CurrentProfile->CurrentGame.rows, CurrentProfile->CurrentGame.columns, mineLocations, mines);
        }
    }

    // post-game processing
    system("cls");

    time(&endTime);
    timeTaken = difftime(endTime, startTime);

    CurrentGame->exists = 1;
    CurrentProfile->lifetimeGames++;
    setMineVisibility(1, CurrentGame->Board, mineLocations, mines);

    printEvade(theme);
    printBoard(CurrentGame->Board, CurrentGame->rows, CurrentGame->columns, -1, -1, theme);
    printf("\n");
    Sleep(SHORT_SLEEP);

    if (gameState == 1) { // user won
        strcpy(CurrentGame->outcome, WON_OUTCOME);

        printf ("\x1b[1m\x1b[37m %c %c %c%c%c %c %c  %c   %c %c%c%c %c%c%c\x1b[0m\n", 219, 219, 219, 223, 219, 219, 219, 219, 219, 219, 223, 219, 219, 223, 219);
	  	printf ("\x1b[1m\x1b[32m %c%c%c %c %c %c %c  %c %c %c %c %c %c %c\x1b[0m\n", 223, 219, 223, 219, 219, 219, 219, 219, 220, 219, 219, 219, 219, 219);
	  	printf ("\x1b[1m\x1b[37m  %c  %c%c%c %c%c%c  %c%c%c%c%c %c%c%c %c %c\x1b[0m\n\n", 219, 219, 220, 219, 219, 220, 219, 219, 220, 223, 220, 219, 219, 220, 219, 219, 219);

        Sleep(SHORT_SLEEP);
        printf(" You won!");
    }
    else if (gameState == 2) { // user lost
        strcpy(CurrentGame->outcome, LOST_OUTCOME);
        
		printf ("\x1b[1m\x1b[37m %c %c %c%c%c %c %c  %c   %c%c%c %c%c%c %c%c%c\x1b[0m\n", 219, 219, 219, 223, 219, 219, 219, 219, 219, 223, 219, 219, 223, 223, 223, 219, 223);
	  	printf ("\x1b[1m\x1b[31m %c%c%c %c %c %c %c  %c   %c %c %c%c%c  %c\x1b[0m\n", 223, 219, 223, 219, 219, 219, 219, 219, 219, 219, 223, 223, 219, 219);
	  	printf ("\x1b[1m\x1b[37m  %c  %c%c%c %c%c%c  %c%c%c %c%c%c %c%c%c  %c\x1b[0m\n\n", 219, 219, 220, 219, 219, 220, 219, 219, 220, 220, 219, 220, 219, 220, 220, 219, 219);
	
        Sleep(SHORT_SLEEP);
        printf(" You inspected a mine!");
    }
    else if (gameState == 3) { // user quit
        strcpy(CurrentGame->outcome, QUIT_OUTCOME);

        setMineVisibility(0, CurrentGame->Board, mineLocations, mines);
        Sleep(SHORT_SLEEP);
        printf(" Game quitted successfully.");
    }

    Sleep(LONG_SLEEP);
    printf("\n\n Time: %d seconds", timeTaken);
    CurrentGame->seconds = timeTaken; // update game time

    // update profile and leaderboard information
    rank = updateLeaderboard(CurrentGame->mode, CurrentGame->outcome, CurrentProfile->name, CurrentGame->seconds, CurrentLeaderboard);

    if (rank != 0) { // user set a new record
        Sleep(LONG_SLEEP);
        printf("\n\n Congratulations! You set a new #%d record for %s in the all-time leaderboard!", rank, CurrentGame->mode);
    }

    updateProfile(CurrentProfile);

    Sleep(LONG_SLEEP);
    printf("\n\n");
    pressEnter();
}


/*
    @brief: checks if a level created is valid or not
	
	@param: Board - a 2-dimensional array of tiles representing the current game board
	@param: rows - indicates the number of rows
	@param: columns - indicates the number of columns
	
	@return: 0 - level is not valid
			 1 - level is valid (there is at least 1 mine and 1 non-mine tile)
*/
int isValidLevel(struct Tile Board[][15], int rows, int columns) {
    int mineCount = 0;
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            if (Board[i][j].state == 9) {
                mineCount++;
            }
        }
    }

    return mineCount > 0 && mineCount < rows * columns;
}


/*
    @brief: guides the user in creating a level; handles the underlying logic and file creation;
        updates the list of levels
	
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
	
	Precondition: LEVELS_DIRECTORY is accurate.
*/
void createLevel(int theme) {
    FILE *fp;
    FILE *fp2;
    int i, j;
    struct Tile Board[10][15];

    string100 directory;
    string100 name;
    char userResponse;

    int numRows, numColumns;
    int row, column;
    int isConfirmed;
    int isValid;

    string100 levels[MAX_LEVELS];
    int numLevels;

    fp = fopen(LEVELS_DIRECTORY, "r");

    if (fp == NULL) {
        fp = fopen(LEVELS_DIRECTORY, "w");
        fprintf(fp, "0\n");
        fclose(fp);

        createLevel(theme);
        return;
    }

    fscanf(fp, "%d", &numLevels);

    if (numLevels >= MAX_LEVELS) {
        Sleep(SHORT_SLEEP);

        printf("\nSorry, you cannot create a new level as there can only be a maximum of %d levels.", MAX_LEVELS);
        Sleep(LONG_SLEEP);

        printf("\n\n");
        pressEnter();
        return;
    }

    // initialize the board, i.e., the 2D array of tiles
    for (i = 0; i < MAX_ROWS; i++) {
        for (j = 0; j < MAX_COLUMNS; j++) {
            Board[i][j].state = 0;
            Board[i][j].isFlagged = 0;
            Board[i][j].isRevealed = 0;
        }
    }

    do {
        Sleep(SHORT_SLEEP);
        system("cls");

        printf("\n");
        printDivider();

        printLevels(theme);

        printf("\n");
        printDivider();
        printf("\n\n");

        printf(" Enter a valid name for the level (or '0' to return to the main menu): ");
        scanf("%s", name);
        clearInputBuffer();

        if (strcmp(name, "0") == 0) return;

        strcpy(directory, "levels\\");
        strcat(directory, name);
        strcat(directory, ".txt");
        fp2 = fopen(directory, "r");

        if (fp2 != NULL) { // user provided a name that already exists
            printf("\n The name '%s' has already been taken!", name);
            fclose(fp2);
        }
    } while (fp2 != NULL);

    fp2 = fopen(directory, "w"); // create the text file

    do {
        printf("\n Enter the number of rows [5-10]: ");
        scanf("%d", &numRows);
        clearInputBuffer();
    } while (!(numRows >= 5 && numRows <= 10));

    do {
        printf("\n Enter the number of columns [5-15]: ");
        scanf("%d", &numColumns);
        clearInputBuffer();
    } while (!(numColumns >= 5 && numColumns <= 15));

    fprintf(fp2, "%d %d\n", numRows, numColumns);

    do {
        do {
            Sleep(SHORT_SLEEP);
            system("cls");

            printEvade(theme);
            printBoard(Board, numRows, numColumns, -1, -1, theme);
            printf("\n Place Mine [P], Delete Mine [D], or Finish Editing [F]: ");
            scanf("%c", &userResponse);
            clearInputBuffer();
        } while (!(userResponse == 'P' || userResponse == 'D' || userResponse == 'F'));

        if (userResponse == 'F') { // user is finished editing
            printf("\n Are you sure you are finished editing?\n");
            
            isConfirmed = confirmAction();

            if (isConfirmed) {
                isValid = isValidLevel(Board, numRows, numColumns);

                if (isValid) { // valid level
                    Sleep(SHORT_SLEEP);

                    for (i = 0; i < numLevels; i++) {
                        fscanf(fp, "%s", levels[i]);
                    }
                    fclose(fp);

                    fp = fopen(LEVELS_DIRECTORY, "w");
                    fprintf(fp, "%d\n", numLevels + 1);
                    for (i = 0; i < numLevels; i++) {
                        fprintf(fp, "%s\n", levels[i]);
                    }
                    fprintf(fp, "%s\n", name);
                    fclose(fp);
                }
                else { // invalid level
                    printf("\n Error. Please continue editing, as a level should have at least one mine and at least one plain tile.");
                    Sleep(LONG_SLEEP);
                }
            }
        }
        else { // user wants to place or delete a mine
            do {
                printf("\n Enter row: ");
                scanf("%d", &row);
                clearInputBuffer();
            } while (!(row >= 1 && row <= numRows));

            do {
                printf("\n Enter column: ");
                scanf("%d", &column);
                clearInputBuffer();
            } while (!(column >= 1 && column <= numColumns));

            if (userResponse == 'P') {
                Board[row - 1][column - 1].state = 9;
                Board[row - 1][column - 1].isRevealed = 1;
            }
            else if (userResponse == 'D') {
                Board[row - 1][column - 1].state = 0;
                Board[row - 1][column - 1].isRevealed = 0;
            }
        }
    } while (!isConfirmed || !isValid);

    // post-edit processing
    for (i = 0; i < numRows; i++) {
        for (j = 0; j < numColumns; j++) {
            if (Board[i][j].state == 0) {
                fprintf(fp2, "%c", '.');
            }
            else if (Board[i][j].state == 9) {
                fprintf(fp2, "%c", 'X');
            }
        }
        fprintf(fp2, "\n");
    }
    fclose(fp2);

    Sleep(SHORT_SLEEP);
    printf("\n Successful.");
    Sleep(SHORT_SLEEP);

    printf(" The custom level '%s' has been saved.", name);
    Sleep(LONG_SLEEP);
    
    printf("\n\n");
    pressEnter();
}


/*
    @param: guides the user in deleting a level; ensures that the user deletes an actual level;
	
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
*/
void deleteLevel(int theme) {
    FILE *fp;
    FILE *fp2;
    int i;

    string100 directory;
    string100 name;

    string100 levels[MAX_LEVELS];
    string100 level;
    int numLevels;

    fp = fopen(LEVELS_DIRECTORY, "r");

    if (fp == NULL) {
        fp = fopen(LEVELS_DIRECTORY, "w");
        fprintf(fp, "0\n");
        fclose(fp);

        deleteLevel(theme);
        return;
    }

    fscanf(fp, "%d", &numLevels);

    do {
        Sleep(SHORT_SLEEP);
        system("cls");
        
        printf("\n");
        printDivider();

        printLevels(theme);

        printf("\n");
        printDivider();
        printf("\n\n");

        printf(" Enter an existing level to delete (or '0' to return to the main menu): ");
        scanf("%s", name);
        clearInputBuffer();

        if (strcmp(name, "0") == 0) return;

        strcpy(directory, "levels\\");
        strcat(directory, name);
        strcat(directory, ".txt");

        fp2 = fopen(directory, "r");
        if (fp2 == NULL) {
            printf("\n The level '%s' does not exist!", name);
        }
    } while (fp2 == NULL);
    
    fclose(fp2);
    printf("\n Are you sure you want to delete the level '%s'?\n", name);

    if (confirmAction()) {
        for (i = 0; i < numLevels; i++) {
            fscanf(fp, "%s", level);
            strcpy(levels[i], level);
        }

        fclose(fp);
        fp = fopen(LEVELS_DIRECTORY, "w");
        fprintf(fp, "%d\n", numLevels - 1);

        for (i = 0; i < numLevels; i++) {
            if (!(strcmp(levels[i], "") == 0 || strcmp(levels[i], name) == 0)) {
                fprintf(fp, "%s\n", levels[i]);
            }
        }

        remove(directory);
    } else {
        deleteLevel(theme);
    }

    Sleep(SHORT_SLEEP);
    printf("\n Successful.");
    Sleep(SHORT_SLEEP);

    printf(" The custom level '%s' has been deleted from the levels folder.", name);
    Sleep(LONG_SLEEP);
    
    printf("\n\n");
    pressEnter();
    fclose(fp);
}


/*
    @brief: computes the winrate given the number of games won and lost in a particular mode
	
	@param: wins - indicates the total games won in a specific mode
	@param: losses - indicates the total games lost in a specific mode
	
	@return: 0 if no game has been played
             otherwise, the winrate in float percentage

    Precondition: Assumes wins and losses are from the same game mode.
*/
float getWinRate(int wins, int losses) {
    if (wins + losses == 0) return 0;
    return (float) wins / (wins + losses) * 100;
}


/*
    @brief: computes the average time in seconds to win in a particular mode
	
	@param: totalSeconds - indicates the total time it took to win across all games in a specific mode
	@param: gamesWon - indicates the total number of games won the profile has in a specific mode
	
	@return: average seconds of the profile in a specific game mode won as int

    Precondition: Assumes totalSeconds and gamesWon are from the same game mode.
*/
int getAverageSeconds(int totalSeconds, int gamesWon) {
    if (gamesWon == 0) return 0;
    return totalSeconds / gamesWon;
}


/*
    @brief: prints a player's statistics
	
	@param: CurrentProfile - pointer to Profile structure instance containing information to reflect
        the necessary data on the statistics screen
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)

    Precondition: Assumes the player's statistics is accurate.
*/
void statisticsScreen(struct Profile *CurrentProfile, int theme) {
    Sleep(SHORT_SLEEP);
    system("cls");
    
    printf("\n");
    printDivider();
    printf("\n\n");

    printTitle(theme);
    printf("\n\n");

    printDivider();
    printf("\n\n\n");

    int month = CurrentProfile->creationDate / 1000000;
    int day = CurrentProfile->creationDate / 10000 % 100;
    int year = CurrentProfile->creationDate % 10000;

    float easyWinRate = getWinRate(CurrentProfile->EasyStats.won, CurrentProfile->EasyStats.lost);
    float difficultWinRate = getWinRate(CurrentProfile->DifficultStats.won, CurrentProfile->DifficultStats.lost);
    float customWinRate = getWinRate(CurrentProfile->CustomStats.won, CurrentProfile->CustomStats.lost);

    int easyAverageSeconds = getAverageSeconds(CurrentProfile->EasyStats.totalSeconds,
        CurrentProfile->EasyStats.won);
    int difficultAverageSeconds = getAverageSeconds(CurrentProfile->DifficultStats.totalSeconds,
        CurrentProfile->DifficultStats.won);
    int customAverageSeconds = getAverageSeconds(CurrentProfile->CustomStats.totalSeconds,
        CurrentProfile->CustomStats.won);

    printf(" -------------------------------------\n");
    printf(" ----- Basic Profile Information -----\n");
    printf(" -------------------------------------\n\n");

    printf(" Name: %s\n", CurrentProfile->name);
    printf(" Date of Profile Creation: %02d/%02d/%d\n", month, day, year);
    printf(" Lifetime Games Played: %d\n\n\n", CurrentProfile->lifetimeGames);

    printf(" -----------------------------------\n");
    printf(" ----- Classic Game Statistics -----\n");
    printf(" -----------------------------------\n\n");

    printf(" Easy Win Rate: %.2f %%\n", easyWinRate);
    printf(" Easy Games Won Average Time: %d seconds\n", easyAverageSeconds);
    printf(" Easy Games Won: %d\n", CurrentProfile->EasyStats.won);
    printf(" Easy Games Lost: %d\n\n", CurrentProfile->EasyStats.lost);

    printf(" Difficult Win Rate: %.2f %%\n", difficultWinRate);
    printf(" Difficult Games Won Average Time: %d seconds\n", difficultAverageSeconds);
    printf(" Difficult Games Won: %d\n", CurrentProfile->DifficultStats.won);
    printf(" Difficult Games Lost: %d\n\n\n", CurrentProfile->DifficultStats.lost);

    printf(" ----------------------------------\n");
    printf(" ----- Custom Game Statistics -----\n");
    printf(" ----------------------------------\n\n");

    printf(" Custom Win Rate: %.2f %%\n", customWinRate);
    printf(" Custom Games Won Average Time: %d seconds\n", customAverageSeconds);
    printf(" Custom Games Won: %d\n", CurrentProfile->CustomStats.won);
    printf(" Custom Games Lost: %d\n\n\n", CurrentProfile->CustomStats.lost);

    printf(" ------------------------\n");
    printf(" ----- Recent Games -----\n");
    printf(" ------------------------\n\n");

    if (CurrentProfile->RecentGame1.exists) { // recent game 1
        printf("\n ----- Recent Game 1 -----\n");
        printf("\n Mode: %s\n", CurrentProfile->RecentGame1.mode);
        printf("\n Outcome: %s\n", CurrentProfile->RecentGame1.outcome);
        printBoard(CurrentProfile->RecentGame1.Board, CurrentProfile->RecentGame1.rows,
            CurrentProfile->RecentGame1.columns, -1, -1, theme);
    }
    else {
        printf(" %s has never played a game of Minesweeper.\n", CurrentProfile->name);
    }

    if (CurrentProfile->RecentGame2.exists) { // recent game 2
        printf("\n\n ----- Recent Game 2 -----\n");
        printf("\n Mode: %s\n", CurrentProfile->RecentGame2.mode);
        printf("\n Outcome: %s\n", CurrentProfile->RecentGame2.outcome);
        printBoard(CurrentProfile->RecentGame2.Board, CurrentProfile->RecentGame2.rows,
            CurrentProfile->RecentGame2.columns, -1, -1, theme);
    }

    if (CurrentProfile->RecentGame3.exists) { // recent game 3
        printf("\n\n ----- Recent Game 3 -----\n");
        printf("\n Mode: %s\n", CurrentProfile->RecentGame3.mode);
        printf("\n Outcome: %s\n", CurrentProfile->RecentGame3.outcome);
        printBoard(CurrentProfile->RecentGame3.Board, CurrentProfile->RecentGame3.rows,
            CurrentProfile->RecentGame3.columns, -1, -1, theme);
    }

    printf("\n\n");
    printDivider();
    printf("\n\n");

    printf(" Press 'Enter' to return to the Main Menu. Press 'Delete' to reset %s's statistics.",
        CurrentProfile->name);

    if (getch() == 224 && getch() == 83) { // the user confirms resetting their statistics
        printf("\n\n Are you sure you want to reset %s's statistics?\n", CurrentProfile->name);
        
        if (confirmAction()) {
            initializeProfile(CurrentProfile, CurrentProfile->name);

            Sleep(SHORT_SLEEP);
            printf("\n Successful.");
            Sleep(SHORT_SLEEP);

            printf(" %s's statistics has been reset.", CurrentProfile->name);
            Sleep(LONG_SLEEP);
            
            printf("\n\n");
            pressEnter();
        }
        else {
            statisticsScreen(CurrentProfile, theme);
        }
    }
}


/*
    @brief: Prints the all-time-leaderboard
	
	@param: CurrentLeaderboard - leaderboard structure containing necessary information to reflect
        the all-time leaderboard
	@param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
*/
void leaderboardScreen(struct Leaderboard *CurrentLeaderboard, int theme) {
    int i;
    struct Records *EasyRecords = &CurrentLeaderboard->EasyRecords;
    struct Records *DifficultRecords = &CurrentLeaderboard->DifficultRecords;
    struct Records *CustomRecords = &CurrentLeaderboard->CustomRecords;
    
    Sleep(SHORT_SLEEP);
    system("cls");

    printf("\n");
    printDivider();
    printf("\n\n\n");
	
	printLeaderboard(theme);
    printf("\n\n");

    printDivider();
    printf("\n\n\n");

    printf(" -------------------------\n");
    printf(" ----- %s -----\n", EASY_MODE);
    printf(" -------------------------\n\n");

    if (EasyRecords->numRecords == 0) {
        printf(" No easy game has been won to date.\n");
    }
    else {
        for (i = 0; i < EasyRecords->numRecords; i++) {
            printf(" %d.) %s | %d seconds\n", i + 1, EasyRecords->names[i], EasyRecords->times[i]);
        }
    }

    printf("\n\n ------------------------------\n");
    printf(" ----- %s -----\n", DIFFICULT_MODE);
    printf(" ------------------------------\n\n");

    if (DifficultRecords->numRecords == 0) {
        printf(" No difficult game has been won to date.\n");
    }
    else {
        for (i = 0; i < DifficultRecords->numRecords; i++) {
            printf(" %d.) %s | %d seconds\n", i + 1, DifficultRecords->names[i], DifficultRecords->times[i]);
        }
    }

    printf("\n\n ------------------\n");
    printf(" ----- %s -----\n", CUSTOM_MODE);
    printf(" ------------------\n\n");

    if (CustomRecords->numRecords == 0) {
        printf(" No custom game has been won to date.\n");
    }
    else {
        for (i = 0; i < CustomRecords->numRecords; i++) {
            printf(" %d.) %s | %d seconds\n", i + 1, CustomRecords->names[i], CustomRecords->times[i]);
        }
    }

    printf("\n\n");
    printDivider();
    printf("\n\n");

    printf(" Press 'Enter' to return to the Main Menu. Press 'Delete' to reset the all-time leaderboard.");

    if (getch() == 224 && getch() == 83) { // the user confirms resetting their statistics
        printf("\n\n Are you sure you want to reset the all-time leaderboard?\n");
        
        if (confirmAction()) {
            remove(LEADERBOARD_DIRECTORY);
            loadLeaderboard(CurrentLeaderboard);

            Sleep(SHORT_SLEEP);
            printf("\n Successful.");
            Sleep(SHORT_SLEEP);

            printf(" The all-time leaderboard has been reset.");
            Sleep(LONG_SLEEP);
            
            printf("\n\n");
            pressEnter();
        }
        else {
            leaderboardScreen(CurrentLeaderboard, theme);
        }
    }
}


/*
    @brief: prints the termination sequence

    @param: theme - integer that dictates the color (cyan/bright red/bright green/purple)
*/
void terminationSequence(int theme) {
    Sleep(SHORT_SLEEP);
    system("cls");

    Sleep(SHORT_SLEEP);
    printEnd(theme);
    Sleep(LONG_SLEEP);
    
    printf("\n Thank you for trying out our Minesweeper program! :D");

    Sleep(LONG_SLEEP);
    printf("\n - CJ & Andre");

    Sleep(LONG_SLEEP);
    printf("\n");
}


/*
    @brief: This is the main function of the program. It initializes the program's main variables,
        executes the different program logic (e.g., profile and leaderboard initialization, game
        generation, profile handling, etc.), prints the corresponding outputs, then terminates the
        program upon completion/termination.

    @return: 0 for successful execution; otherwise, a non-zero value corresponding to the error.

*/
int main() {
	srand(time(NULL)); // initializes rand() using system time

	int theme = getRandInt(1, 4); // randomize program theme
    int programIsRunning = 1;
    char userResponse;

    struct Profile CurrentProfile;
    struct Leaderboard CurrentLeaderboard;

    CurrentProfile.creationDate = getDateCode();
    initializeProfile(&CurrentProfile, "GUEST");
    initializeLeaderboard(&CurrentLeaderboard);
    loadLeaderboard(&CurrentLeaderboard);

    system("cls");
    Sleep(SHORT_SLEEP);

    printTitle(theme);
    printf("\n\n\x1b[1m\x1b[37m                      A CCPROG2 project by Christian Joseph Bunyi and Andre Gabriel Llanes.\x1b[0m\n\n");
    printf("\x1b[1m\x1b[37m                                            Press enter to continue.\x1b[0m");
    clearInputBuffer();

    profileHandler(&CurrentProfile, theme);

    while (programIsRunning) {
        do {
            Sleep(SHORT_SLEEP);
            system("cls");

            printf("\n");
            printDivider();
            printf("\n\n");
			
			printMenu(theme);
            printf("\n\n");
			
            printf(" Welcome to Minesweeper, %s!\n\n", CurrentProfile.name);
            printf(" [a] Start Game\n [b] Create a Level\n [c] Delete a Level\n [d] Change Profile\n");
            printf(" [e] View Statistics\n [f] View Leaderboard\n [g] Reroll\n [h] Quit\n\n");

            printDivider();
            printf("\n\n");

            printf(" Please input the letter corresponding to your action (a/b/c/d/e/f/g/h): ");
            
            scanf("%c", &userResponse);
            clearInputBuffer();
        } while (!(userResponse >= 'a' && userResponse <= 'h'));

        if (userResponse == 'a') { // Start Game
            gameHandler(&CurrentProfile, &CurrentLeaderboard, theme);
        }
        else if (userResponse == 'b') { // Create a Level
            createLevel(theme);
        }
        else if (userResponse == 'c') { // Delete a Level
            deleteLevel(theme);
        }
        else if (userResponse == 'd') { // Change Profile
            profileHandler(&CurrentProfile, theme);
        }
        else if (userResponse == 'e') { // View Statistics
            statisticsScreen(&CurrentProfile, theme);
        }
        else if (userResponse == 'f') { // View Leaderboard
            leaderboardScreen(&CurrentLeaderboard, theme);
        }
        else if (userResponse == 'g') { // Reroll
            printf("\n Rolling.");
            Sleep(SHORT_SLEEP);

            printf(".");
            Sleep(SHORT_SLEEP);

            printf(".");
            Sleep(LONG_SLEEP);

            theme = getRandInt(1, 4);
            printf(" You rolled a %d!", theme);
            Sleep(SHORT_SLEEP);
        }
        else if (userResponse == 'h') { // Quit
            Sleep(SHORT_SLEEP);
            printf("\n Are you sure you want to quit?\n");

            if (confirmAction()) { // user confirms quitting
                programIsRunning = 0;
            }
        }
    }

    terminationSequence(theme);

    return 0;
}


/*
    This is to certify that this project is our own work, based on our personal
    efforts in studying and applying the concepts learned. We have constructed
    the functions and their respective algorithms and corresponding code by
    ourselves. The program was run, tested, and debugged by our own efforts.
   
    We further certify that we have not copied in part or whole or otherwise
    plagiarized the work of other students and/or persons.

    Christian Joseph Cachero Bunyi, DLSU ID# 12306452
    Andre Gabriel De Ocampo Llanes, DLSU ID# 12307327
*/