#include "secondpass.h"

/* the second pass over the file adds the addresses of the operands in the command lines
 * after each command line.
 * the algorithm is as follows:
 * check the first word after the label (if there is one). if it's not an instruction line, go to the next line.
 * otherwise, check how many operands the command accepts. get that number of operands and get their directing method.
 * send those arguments to preCode to get the number of instructions to advance the instruction counter.
 * return an indicator whether an error had been found during the second run over the file.
 */
int make_second_pass(FILE *fil, short memory[], LabelList *labelList, ExternalList *extList) {
    char line[MAX_LINE], command[MAX_LINE], opSource[MAX_LINE], opTarget[MAX_LINE];
    int IC, errorFound, index, numOps, sourceAddressing, targetAddress, lineNumber, answer;

    IC = 0;
    index = 0;
    lineNumber = 0;
    errorFound = FALSE;


    while (fgets(line, MAX_LINE + 2, fil)) {
        memset(opSource, '\0', MAX_LINE);
        memset(opTarget, '\0', MAX_LINE);
        targetAddress = REACHED_END;
        sourceAddressing = REACHED_END;

        lineNumber++;
        index = move_to_not_white(line, MAX_LINE, 0);
        if (index == MAX_LINE || line[index] == ';' || line[index] == '\n')
            continue;
        insertWord(line, command, &index);

        if (!is_label(command))
            index = 0;
        insertWord(line, command, &index);
        if (!is_instruction(command))
            continue;

        numOps = how_many_ops(command);
        IC++;
        if (!numOps)
            continue;

        index = move_to_not_white(line, MAX_LINE, index);
        insertWord(line, opSource, &index);

        /*get first operand addressing*/
        sourceAddressing = source_directing(opSource, lineNumber);

        if (numOps == 2) { /*if there are 2 operands get the second operand */
            index = move_to_not_white(line, MAX_LINE, index);

            if (line[index] == ',')    /*skipping comma*/
                index = move_to_not_white(line, MAX_LINE, index + 1);

            insertWord(line, opTarget, &index);
            targetAddress = source_directing(opTarget, index);
        }
        /* send the two operands to process their binary code*/
        answer = preCode(memory, IC, lineNumber, opSource, opTarget, sourceAddressing, targetAddress, labelList,
                         extList);
        if (answer == ERROR)
            errorFound = TRUE;
        else
            IC = answer;
    }
    freeLabelList(labelList);
    if (errorFound)
        return ERROR;
    else
        return IC;
}


/* check if each operand exist. if the two of them exist, check if both are registers, if so create one binary code
 * representing the two registers numbers, add that code to the memory and return IC+1. otherwise
 *  check for each operand if it's a register, if not check for addressing 0-2.
 *  if the addressing is 0-3 add the operand code to the memory and return the number of instructions advanced.
 *  if not, return an error
 */
int preCode(short memory[], int IC, int lineNumber, char opSource[], char opTarget[], int sourceAddressing,
            int targetAddressing, LabelList *lblList, ExternalList *extList) {
    int answer;
    if (sourceAddressing == 3 && targetAddressing == 3) {
        memory[IC] = (short) (codeSourceRegister(opSource) | codeTargetRegister(opTarget));
        return IC + 1;
    }

    if (sourceAddressing != REACHED_END) {
        if (sourceAddressing == 3) {
            answer = codeSourceRegister(opSource);
            memory[IC++] = (short) answer;
        } else {
            answer = handelAddressing0and1and2(opSource, sourceAddressing, memory, IC, lblList, extList);
            if (answer == FALSE || answer == ERROR)
                return printERROR(lineNumber, "operand is not allowed in current context");
            else
                IC = answer;
        }
        if (targetAddressing != REACHED_END) {
            if (targetAddressing == 3) {
                answer = codeTargetRegister(opTarget);
                memory[IC++] = (short) answer;
            } else {
                answer = handelAddressing0and1and2(opTarget, targetAddressing, memory, IC, lblList, extList);
                if (answer == FALSE || answer == ERROR)
                    return printERROR(lineNumber, "operand is not allowed in current context");
                else
                    IC = answer;
            }
        }
    } else
        return ERROR;

    return IC;
}

/* handel operand addressing methode  0,1,2 and add if the operand's addressing methode matches 0-2
 * then add its necessary addresses to the memory. and return the number of instructions added
 */
int handelAddressing0and1and2(char *operand, int addressing, short memory[], int currentIC, LabelList *lblList,
                              ExternalList *extList) {
    short address;

    if (addressing == 0) {
        int num = getNumberCode(operand);
        memory[currentIC] = (short) num;
        return currentIC + 1;
    }

    if (addressing == 1) {
        address = (short) labelAddressCode(operand, lblList, extList, currentIC);
        memory[currentIC] = address;
        return currentIC + 1;
    }
    /* structure operand return 2 addresses.
      one for the operand's address, and one for the first or second field*/
    if (addressing == 2) {
        int field = 0;
        char name[MAX_LINE];
        separateStructToNameAndField(operand, name, &field);
        address = (short) labelAddressCode(name, lblList, extList, currentIC);
        memory[currentIC] = address;
        memory[++currentIC] = (short) (field << 2);
        return currentIC + 1;
    }
    return FALSE;
}

/* create a code from the label address.
 * if the label is an external, the address is 0 and addressing is ARE field is 1
 */
int labelAddressCode(char *operand, LabelList *lblList, ExternalList *extList, int currentIC) {
    Label *labelTemp;
    External *extTemp;
    int address;
    if ((labelTemp = isLabelExist(lblList, operand)) != NULL) {
        address = (labelTemp->address << 2) | 2;
        return address;
    }

    if ((extTemp = isExternalExist(extList, operand)) != NULL) {
        extTemp->addressList = addAddress(extTemp->addressList, currentIC + START_OF_MEMORY);
        return 1;
    }
    return FALSE;
}

/* separate a structure operand to the structure name and field
 */
void separateStructToNameAndField(char *operand, char name[], int *field) {
    int j = 0, length = (int) strlen(operand);
    while (j < length && operand[j] != '.') {
        name[j] = operand[j];
        j++;
    }
    name[j] = '\0';
    *field = operand[++j] - '0';
}

/* create a binary representing the source register number
 */
unsigned short codeSourceRegister(char const *reg) {
    unsigned short num = reg[1] - '0';
    num <<= 6;
    return num;
}

/* create a binary representing the target register number
 */
unsigned short codeTargetRegister(char const *reg) {
    unsigned short num = reg[1] - '0';
    num <<= 2;
    return num;
}

/* return a binary of the number
 */
short getNumberCode(char *word) {
    char temp[MAX_LINE];
    int index;
    short num;
    index = move_to_not_white(word, MAX_LINE, 1);

    insertWord(word, temp, &index);
    num = atoi(temp);
    num = num << 2;
    return num;
}
