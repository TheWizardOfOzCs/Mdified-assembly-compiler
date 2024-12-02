#ifndef FINALPROJECT_UTILS_H
#define FINALPROJECT_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define START_OF_MEMORY 100
#define MAX_LINE 80
#define MAX_LABEL_NAME 30
#define RAM 256
#define ERROR (-1)

#define NUMBER_OF_INSTRUCTIONS 16
#define NUMBER_OF_DIRECTIONS 5

#define REACHED_END (-2)

enum bool {
    FALSE, TRUE
};

int is_instruction(char *label);

int is_directing(char *label);

int is_register(char *label);

int move_to_not_white(char line[], int length, int current);

int end_of_line(char const line[]);

int how_many_ops(char command[]);

void insertWord(char *line, char *word, int *index);

int source_directing(char operand[], int lineNumber);

int code_word(int addressSource, int addressDest, int numberOps);

int check_valid_data(char word[], int digitsInNumber);

int printERROR(int lineNumber, char *errorMessage);


static const char *instructions[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec",
                                     "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
static const char *directing[] = {"data", "string", "struct", "entry", "extern"};

#endif
