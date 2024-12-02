
#include "lineHandler.h"
/* duplicate the string so the tokenizer would not temper it
 */
char* saveLine(char *str) {
    return strdup(str);
}

/* return the first word in the line
 */
char* getFirstWord(char* line){
    char *dup= saveLine(line);
    char *token = strtok(dup," \t\n");
    return token;
}