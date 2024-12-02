#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "labelHandler.h"
#include "utils.h"
#include "binaryCodeHandler.h"

int make_second_pass(FILE *fil, short memory[], LabelList *labelList, ExternalList *extList);

int preCode(short memory[], int IC, int lineNumber, char opSource[], char opTarget[], int sourceAddressing,
            int targetAddressing, LabelList *lblList, ExternalList *extList);

int handelAddressing0and1and2(char *operand, int addressing, short memory[], int currentIC, LabelList *lblList,
                              ExternalList *extList);

int labelAddressCode(char *operand, LabelList *lblList, ExternalList *extList, int currentIC);

unsigned short codeSourceRegister(char const *reg);

unsigned short codeTargetRegister(char const *reg);

short getNumberCode(char *word);

void separateStructToNameAndField(char *operand, char name[], int *field);


#endif
