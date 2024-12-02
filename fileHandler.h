
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 1000 /* max length of name */

/* open a file with the given name, with the given extension and in what mode
 */
FILE *openFile (char *fName, char *extension, char *mode);






#endif
