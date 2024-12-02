
#ifndef MACROSHANDLER_H
#define MACROSHANDLER_H

#include "lineHandler.h"

#define MAX_LINE_LENGTH 80

#define MACRO_START "macro"
#define MACRO_END "endmacro"

/* a node in single linked list that holds
 * a pointer to a string, and a pointer to the next node
 * used to hold the code lines of the macro
 */
typedef struct LineNode{
    char *line;
    struct LineNode *next;
}LineNode;

typedef struct {
    char *name;
    int numOfLines;
    LineNode *firstMacroLine;
    LineNode *lastMacroLine;
} Macro;

typedef struct MacroNode {
    Macro *macro;
    struct MacroNode *next;
} MacroNode;

typedef struct {
    int length;
    MacroNode *firstMacro;
    MacroNode *lastMacro;
} MacroList;

/*initialize structures*/

MacroList *initializeMacroList();

MacroNode *initializeMacroNode(Macro *macro);

Macro *initializeMacro(char *macroName);

LineNode *initializeLineNode(char *line);

/*functions in charge of handling the macros*/

/* the function gets a string representing the name of the macro we want to add,
 * a file buffer located at the next line after the "macro" keyword
 * and the list of existing macros to append the new macro to the end of the list
 * the function create a new macro, adds its name and send it over to copyMacroLines()
 * to copy the macro lines to the macro structure
*/
Macro *addMacro(FILE *file, MacroList *macroList, char *macroName);

/* checks the first word in the given line
 * is a name of an existing macro in the given macro list.
 * if it is, return the found macro
 * if the word is not a name of an existing macro, return NULL
 */
Macro *isMacroNameExist(MacroList *macroList, char *line);

int isMacroNameIsSafeWord(char *macroName);

/* the function stores all lines from the file to the given macro's lines list up until
 * the file buffer reaching an "endmacro" keyword
*/
Macro *copyMacroLines(FILE *file, Macro *macro);

/* checks if the first word in the given line is the "endmacro" key word.
 * return 1 if it is "endmacro"
 * return 0 if it's not
 */
int isEndOfMacro(char *line);

/* checks if the first word in the given line is the "macro" key word.
 * if its "macro", save the next word (the macro name by definition) to the line pointer and return 1.
 * if it's not, return 0
 */
int isStartOfMacro(char *line);

/* add the given line to the end of the given macro's line list
 */
void addLineToMacro(Macro *macro, char *line);

/* add the given macro to the end of the given macro list
 */
void addMacroToList(MacroList *macroList, Macro *macro);

/* the function prints to the given file all the line nodes lines
 * stored in the macro until reaching a NULL node
 */
void printMacroToFile(FILE *outputFile, Macro *macro);


/*free allocated memory of all data in each structure */

void freeMacroList(MacroList *macroList);

void freeMacroNodes(MacroNode *node);

void freeMacro(Macro *macro);

void freeLineNodes(LineNode *line);


#endif
