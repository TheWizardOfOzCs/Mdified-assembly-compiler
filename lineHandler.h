
#ifndef LINEHANDLER_H
#define LINEHANDLER_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/* duplicate the string so the tokenizer would not temper it
 */
char* saveLine(char* str);
/* return the first word in the line
 */
char* getFirstWord(char* line);




#endif
