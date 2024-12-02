#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "utils.h"
#include "labelHandler.h"
#include "binaryCodeHandler.h"

#define REACHED_END (-2)

/*A function to sort all the lines of the file and execute them using sub outings*/
int
make_first_pass(FILE *fil, short memory[], short data[], int *IC, int *DC, LabelList *labelList, EntryList *entryList,
                ExternalList *externalList);

/*A function that checks if the line is a directing line and acts accordingly*/
int is_directing_command(char line[], int lineNumber, int indexAfterLabel, short data[], int *DC, Label *temp);

/*A method that takes the numbers from a data line*/
int getDataTypeArguments(char *command, char line[], short data[], int positionInLine, int lineNumber, int DC);

/*checks if a line is a command line*/
int is_command(char line[], short memory[], int indexAfterLabel, int lineNumber, int IC);

/*checking if a line is an entry or extern*/
int extern_or_entry(char line[], int indexAfterLabel, int lineNumber, EntryList *entryList, ExternalList *extList);

/*a line the processes a data line*/
int process_data(char line[], short data[], int positionInLine, int lineNumber, int dataCounter);

/*a method to get a number*/
short getNumber(char *line, int *index);

/*a method t process a string*/
int process_string(char line[], short data[], int positionInLine, int lineNumber, int dataCounter);

/*a function to process a struct*/
int process_struct(char line[], short data[], int positionInLine, int lineNumber, int dataCounter);

/*a function that checks if a command and its operands are correct*/
int correct_operands(char command[], int source_address, int destination_address);


#endif