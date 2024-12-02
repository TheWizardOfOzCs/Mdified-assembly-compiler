
#ifndef BINARYCODEHANDLER_H
#define BINARYCODEHANDLER_H

#include "utils.h"


typedef struct OpWord {
    unsigned short op_code: 4;
    unsigned short source_op: 2;
    unsigned short target_op: 2;
    unsigned short alloc: 2;
} OpWord;

OpWord *addCommandOpcode(OpWord *word, char *command);

OpWord *addSourceOp(OpWord *word, short addressing);

OpWord *addTargetOp(OpWord *word, short addressing);

short opWordToBinary(OpWord *word);

OpWord *initializeOpWord();

void freeOpWord(OpWord *word);


#endif
