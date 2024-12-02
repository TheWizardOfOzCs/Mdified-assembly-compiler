#ifndef PARSER_H
#define PARSER_H

#include "fileHandler.h"
#include "macrosHandler.h"

#include <ctype.h>

#define MAX_LINE_LENGTH 80


/*the function create a .am file from the given file.
 * the .am file is the code file after spreading all the macros in it
 * a macro is declared by the keyword "macro" and the name of the macro after the declaration
 * and ends with the keyword "endmacro".
 * firstRun gets the source file and its name, open a new .am file to write to and starts going through
 * the source file line by line.
 *
 * if the first word in line is a name of a saved macro, print all the macro lines.
 * if the first word in line is the "macro" keyword, pass the file buffer, macros list and the second word in line
 * to add addMacro function
 * if it's not either of the two above options, print the line to the .am file.
 * at the end return the new .am file.
 */
FILE* firstRun(FILE *file, char *fileName);

#endif

