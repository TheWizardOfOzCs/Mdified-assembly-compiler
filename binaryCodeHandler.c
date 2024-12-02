
#include "binaryCodeHandler.h"


short opWordToBinary(OpWord *word) {
    int binaryWord;
    binaryWord = 0;
    binaryWord = binaryWord | (word->op_code << 6);
    binaryWord = binaryWord | (word->source_op << 4);
    binaryWord = binaryWord | (word->target_op << 2);
    binaryWord = binaryWord | word->alloc;

    return (short) binaryWord;
}


OpWord *addCommandOpcode(OpWord *word, char *command) {
    short i;
    for (i = 0; i < NUMBER_OF_INSTRUCTIONS; i++)
        if (!strcmp(command, instructions[i])) {
            word->op_code = i;
            return word;
        }
    return word;
}

OpWord *addSourceOp(OpWord *word, short addressing) {
    word->source_op = addressing;
    return word;
}

OpWord *addTargetOp(OpWord *word, short addressing) {
    word->target_op = addressing;
    return word;
}


OpWord *initializeOpWord() {
    OpWord *newOpWord;
    newOpWord= (OpWord *) malloc(sizeof(OpWord));
    newOpWord->op_code = 0;
    newOpWord->source_op = 0;
    newOpWord->target_op = 0;
    newOpWord->alloc = 0;
    return newOpWord;
}


void freeOpWord(OpWord *word) {
    free(word);
}