
#include "fileHandler.h"

/* open a file with the given name, with the given extension and in what mode
 */
FILE *openFile (char *fName, char *extension, char *mode)
{
    char name[MAX_NAME_LENGTH];
    strcpy(name, fName);
    strcat(name, extension);

    return fopen(name, mode);
}

