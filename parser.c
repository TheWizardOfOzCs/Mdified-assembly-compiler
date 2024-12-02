#include "parser.h"


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
FILE *firstRun(FILE *file, char *fileName) {
    FILE *outPutFile;
    if (!(outPutFile = openFile(fileName, ".am", "w")))
        return NULL;

    MacroList *macroList = initializeMacroList();/*initialize a new macro list to store all the macros pointers */
    char line[MAX_LINE_LENGTH * 2];
    Macro *macro = NULL;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if ((macro = isMacroNameExist(macroList, line)) != NULL) {/*checks if first word is a name of a saved macro */
            printMacroToFile(outPutFile, macro);
            continue;
        }
        if (isStartOfMacro(line)) {     /*checks if first word is the "macro" keyword */
            addMacro(file, macroList, line);    /*save the macro to the list */
        } else
            fprintf(outPutFile, "%s", line);
    }
    fclose(outPutFile);

    freeMacroList(macroList);   /*free all the macros in the list*/
    return outPutFile;
}



