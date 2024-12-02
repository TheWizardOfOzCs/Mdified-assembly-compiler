#include "utils.h"

/* checks if the given label is a saved name of an instruction
 */
int is_instruction(char *label) {
    int i;
    for (i = 0; i < NUMBER_OF_INSTRUCTIONS; i++) {
        if (!strcmp(instructions[i], label))
            return TRUE;
    }
    return FALSE;
}

/* checks if the given label is a saved name of a directing
 */
int is_directing(char *label) {
    int i;
    for (i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
        if (!strcmp(label, directing[i]))
            return TRUE;
    }
    return FALSE;
}

/* checks if the given label is a saved name of a register
 */
int is_register(char *label) {
    if (strlen(label) == 2) {
        if (label[0] == 'r') {
            if (label[1] >= '0' && label[1] <= '7')
                return TRUE;
        }
    }
    return FALSE;
}

/* move to the next not white character in the line
 */
int move_to_not_white(char line[], int length, int current) {
    int i;
    for (i = current; i < length && line[i] != '\n' && isspace(line[i]); i++);
    return i;
}

/* return the index of the end of the line
 */
int end_of_line(char const line[]) {
    int i;
    i = 0;
    while (line[i] != '\n' && i < MAX_LINE)
        i++;
    return i;
}

/* checks if the command match the instructions compatible with 2 operands:
 * (0-3,6) in the instruction list
 */
int twoOperands(char *command) {
    int i;
    for (i = 0; i <= 6; i++) {
        if (i == 4) /*skip instructions 4,5*/
            i = 6;
        if (!strcmp(command, instructions[i]))
            return TRUE;
    }
    return FALSE;
}

/* checks if the command match the instructions compatible with only 1 operand:
 * (4-5,7-13) in the instruction list
 */
int oneOperand(char *command) {
    int i;
    for (i = 4; i <= 13; i++) {
        if (i == 6)/*skip instruction 6*/
            i = 7;
        if (!strcmp(command, instructions[i]))
            return TRUE;
    }
    return FALSE;
}

/* checks how many operands the command accepts
 */
int how_many_ops(char command[]) {
    if (oneOperand(command) == TRUE)
        return 1;
    if (twoOperands(command))
        return 2;
    return 0;
}

/* until reaching a space character or a comma,
 * inserting each character to the word array,
 * and advancing the index to the end of the word in the line
 */
void insertWord(char *line, char *word, int *index) {
    int wordIndex, lineIndex, lineLength;
    memset(word, '\0', MAX_LINE);

    lineLength = (int) strlen(line);
    wordIndex = 0;
    lineIndex = *index;
    lineIndex = move_to_not_white(line, MAX_LINE, lineIndex);
    while (!isspace(line[lineIndex]) && line[lineIndex] != ',' && lineIndex < lineLength)
        word[wordIndex++] = line[lineIndex++];

    *index = lineIndex;
}

/*This method calculates the number of words the command needs for the machine code
 * @param addressSource is the addressing method for the first operand
 * @param addressDest is the addressing method for the second operand
 * @param numberOps is the number of ops the command takes
 */
int code_word(int addressSource, int addressDest, int numberOps) {
    int numWords;
    numWords = 0;
    if (numberOps == 0)
        return 0;
    /*if both operands are registers they are going to be share an additional word*/
    if (addressSource == 3 && addressDest == 3)
        return 1;
    else if (addressSource == 2)
        numWords += 2;
    else if (addressSource == 0 || addressSource == 1) {
        numWords += 1;
    }
        /*if the operand is a register*/
    else
        numWords += 1;
    /*if the command has two operands we need to find how many words the destination word takes*/
    if (numberOps == 2) {
        if (addressDest == 2)
            numWords += 2;
        else if (addressDest == 0 || addressDest == 1)
            numWords += 1;
        else
            numWords += 1;
    }
    return numWords;
}


/* A function to determine the addressing method of the operands
 * if it's an immediate addressing represented by '#', return 0
 * if it's a structure, return 2
 * if it's a register (from r0 to r7) return 3
 * and if it's none of the above then it's a label, then return 1
 */
int source_directing(char operand[], int lineNumber) {
    int addressing, length;

    if (operand[0] == '\n')
        return REACHED_END;
    if (operand[0] == '\0')
        return printERROR(lineNumber, "invalid operand or too many commas");

    length = (int) strlen(operand);


    /*direct addressing*/
    if (operand[0] == '#') {
        addressing = 0;
        operand[0] = ' ';
        int valid = check_valid_data(operand, length);
        if (!valid) {
            return printERROR(lineNumber, "invalid number");
        } else
            return addressing;
    }
        /*checking if the operand is a valid register*/
    else if (length == 2 && operand[0] == 'r') {
        if (operand[1] >= '0' && operand[1] <= '7') {
            addressing = 3;
            return addressing;
        } else {
            addressing = 1;
            return addressing;
            /*return printERROR(lineNumber, "invalid register number");*/
        }
    }
    int j = 0;
    while (j < length) {
        if (operand[j] == '.') {
            if (j + 2 == length) {
                if (operand[j + 1] == '1' || operand[j + 1] == '2') {
                    addressing = 2;
                    return addressing;
                }
            }
            return printERROR(lineNumber, "invalid reference to struct");
        } else {
            j++;
        }
    }



    /*if it's not a number, register or a structure then it's a label*/
    addressing = 1;
    return addressing;
}

/* checks if the given word is a valid number (may include on '+' or '-')
 */
int check_valid_data(char word[], int digitsInNumber) {
    int i;
    i = move_to_not_white(word, MAX_LINE, 0);
    if (!isdigit(word[i]) && (word[i] != '-') && (word[i] != '+'))
        return FALSE;
    i++;
    for (; i < digitsInNumber; i++) {
        if (!isdigit(word[i]))
            return FALSE;
    }
    return TRUE;
}

/* error format. prints a message format telling in what line the error occurred
 * and what was the error
 * return ERROR
 */
int printERROR(int lineNumber, char *errorMessage) {
    printf("ERROR IN LINE %d: %s\n", lineNumber, errorMessage);
    return ERROR;
}