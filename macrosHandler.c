#include "macrosHandler.h"
#include <stdlib.h>

/* the function gets a string representing the name of the macro we want to add,
 * a file buffer located at the next line after the "macro" keyword
 * and the list of existing macros to append the new macro to the end of the list
 * the function create a new macro, adds its name and send it over to copyMacroLines()
 * to copy the macro lines to the macro structure
 */
Macro *addMacro(FILE *file, MacroList *macroList, char *macroName) {
    macroName = saveLine(macroName);
    if(!isMacroNameIsSafeWord(macroName)) {
        if (isMacroNameExist(macroList, macroName) == NULL) {
            Macro *macro = initializeMacro(macroName);
            if ((macro = copyMacroLines(file, macro)) != NULL) {
                addMacroToList(macroList, macro);
                return macro;
            }
        } else
            printf("macro already exist");
    }
    return NULL;
}
/* checks the first word in the given line
 * is a name of an existing macro in the given macro list.
 * if thw word is an exist macro name, return a pointer to the macro
 * if the word is not a name of a macro, return NULL
 */
Macro *isMacroNameExist(MacroList *macroList, char *line) {
    char *token = getFirstWord(line);
    Macro *returnMacro = NULL;
    if (token != NULL) {
        MacroNode *macroNode = macroList->firstMacro;
        while (macroNode != NULL) {
            if (!strcmp(token, macroNode->macro->name)) {
                returnMacro = macroNode->macro;
                break;
            }
            macroNode = macroNode->next;
        }
    }
    return returnMacro;
}
/* checks if the name of the macro is an instruction or a
 */
int isMacroNameIsSafeWord(char *macroName){
    if(is_instruction(macroName)){
        printf("name of macro can't be an instruction");
        return 1;
    }
    if(is_directing(macroName)){
        printf("name of macro can't be a directing");
        return 1;
    }
    if(is_register(macroName)){
        printf("name of macro can't be a register");
        return 1;
    }
    return 0;
}
/* going through each line from the buffer point,
 * checking if the first word in line is the "endmacro" key word.
 * if it's not the "endmacro" key word, add the line to the given macro
 * if it's the "endmacro" key word, free the line memory and stop buffering.
 * at the end return the macro
 */
Macro *copyMacroLines(FILE *file, Macro *macro) {
    char *temp, *line;
    temp = malloc(MAX_LINE_LENGTH);
    while ((line = saveLine(fgets(temp, MAX_LINE_LENGTH, file)))) {
        if (!isEndOfMacro(line))
            addLineToMacro(macro, line);
        else {
            free(line);
            break;
        }
    }
    return macro;
}
/* checks if the first word in the given line is the "endmacro" key word.
 * return 1 if it is "endmacro"
 * return 0 if it's not
 */
int isEndOfMacro(char *line) {
    char *token;
    token = getFirstWord(line);
    if (token != NULL && !strcmp(token, MACRO_END))
        return 1;
    return 0;
}
/* checks if the first word in the given line is the "macro" key word.
 * if its "macro", save the next word (the macro name by definition) to the line pointer and return 1.
 * if it's not, return 0
 */
int isStartOfMacro(char *line) {
    char *token;
    token = getFirstWord(line);
    if (token != NULL && !strcmp(token, MACRO_START)) {
        token = strtok(NULL, " \t\n");
        strcpy(line, token);
        return 1;
    } else
        return 0;
}
/* create a new line node that holds the given line
 * and be stored in the macro line list.
 * if the first line node in the macro is empty,
 * set its first and last pointer to the new line node
 * otherwise, set the last line's next node to point at the new node
 * and set the last node pointer to the new node.
 * at the end increment the line count of the macro by 1
 */
void addLineToMacro(Macro *macro, char *line) {
    LineNode *lineNode;
    lineNode = initializeLineNode(line);
    if (macro->firstMacroLine == NULL) {
        macro->firstMacroLine = lineNode;
        macro->lastMacroLine = lineNode;
    } else {
        macro->lastMacroLine->next = lineNode;
        macro->lastMacroLine = lineNode;
    }
    macro->numOfLines++;
}
/* add the given macro to the given list
 * creating a new macro node that holds the macro
 * if the first macro node of the list is empty,
 * set its first and last pointer to the new macro node
 * otherwise, set the last node's next node to point at the new node
 * and set the last node pointer to the new node.
 * at the end increment the macros count by 1
 */
void addMacroToList(MacroList *macroList, Macro *macro) {
    MacroNode *newMacroNode;
    newMacroNode = initializeMacroNode(macro);
    if (macroList->firstMacro == NULL) {
        macroList->firstMacro = newMacroNode;
        macroList->lastMacro = newMacroNode;
    } else {
        macroList->lastMacro->next = newMacroNode;
        macroList->lastMacro = newMacroNode;
    }
    macroList->length++;
}
/* the function prints to the given file all the line nodes lines
 * stored in the macro until reaching a NULL node
 */
void printMacroToFile(FILE *outputFile, Macro *macro) {
    LineNode *line;
    line = macro->firstMacroLine;
    while (line != NULL) {
        fprintf(outputFile, "%s", line->line);
        line = line->next;
    }
}
/* initialize a new line node to store in the macro line list, with a given line.
 * allocating memory.
 * setting the line pointer to the given line.
 * set the next line node to NULL
 * returns a pointer to the initialized line node
 */
LineNode *initializeLineNode(char *line) {
    LineNode *node;
    node  = (LineNode *) malloc(sizeof(LineNode));
    node->line = line;
    node->next = NULL;

    return node;
}

/* initialize a new macro node with a given macro to store in the macro list.
 * allocating memory.
 * setting the macro pointer to the given macro
 * set the next node to NULL
 * returns a pointer to the initialized macro node
 */
MacroNode *initializeMacroNode(Macro *macro) {
    MacroNode *node;
    node = (MacroNode *) malloc(sizeof(MacroNode));
    node->macro = macro;
    node->next = NULL;
    return node;
}
/* initialize a new macro with a given name.
 * allocating memory.
 * set the name of the macro to the given name
 * setting the number of lines in the macro to 0.
 * setting the first and last nodes to NULL
 * returns a pointer to the initialized macro
 */
Macro *initializeMacro(char *macroName) {
    Macro *macro;
    macro = (Macro *) malloc(sizeof(Macro));
    macro->name = macroName;
    macro->numOfLines = 0;
    macro->firstMacroLine = NULL;
    macro->lastMacroLine = NULL;
    return macro;
}


/* initialize a new macro list.
 * allocating memory. setting the length of the list to 0.
 * setting the first and last nodes to NULL
 * returns a pointer to the initialized macro list
 */
MacroList *initializeMacroList() {
    MacroList *macroList;
    macroList  = (MacroList *) malloc(sizeof(MacroList));
    macroList->length = 0;
    macroList->firstMacro = NULL;
    macroList->lastMacro = NULL;
    return macroList;
}


/* sending the first macro node in the list to the freeMacroNodes function
 * then freeing the list's allocated memory space and setting its pointer to NULL
 */
void freeMacroList(MacroList *macroList) {
    freeMacroNodes(macroList->firstMacro);
    free(macroList);
    macroList = NULL;
}
/* iterate through each node form the given node until reaching a NULL node,
 * sending each macro in the macro nodes to the freeMacro function.
 * then freeing the allocated memory of each node and setting the nodes pointers to NULL
 */
void freeMacroNodes(MacroNode *node) {
    MacroNode *temp;
    while (node != NULL) {
        temp = node;
        node = node->next;
        freeMacro(temp->macro);
        free(temp);
        temp = NULL;
    }
}
/* sending the first line node of the macro to the freeLineNodes function
 * and then freeing the allocated memory of the macro, then setting the macro
 * pointer to NULL
 */
void freeMacro(Macro *macro) {
    freeLineNodes(macro->firstMacroLine);
    free(macro);
    macro = NULL;
}

/* iterate through each node form the given node until reaching a NULL node,
 * freeing the allocated memory of the line nodes and setting the pointer to NULL
 */
void freeLineNodes(LineNode *line) {
    LineNode *temp;
    while (line != NULL) {
        temp = line;
        line = line->next;
        free(temp);
        temp = NULL;
    }
}