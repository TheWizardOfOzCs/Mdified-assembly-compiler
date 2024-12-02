
#include "firstpass.h"

/* the function goes through the given file line by line and preform the following algorithm:
 * check if the line is a comment line starting with ';' or if it's an empty line. if so, go to the next line
 * if the firs word in the line is a label, initialize a new label with that name
 * go to the next word after the label (if there is no label, go back to the start of the line)
 * check if the next word is an instruction to save data to the hip. if so, save the data and go to the next line
 * check if the next word is an entry or extern command.
 *    if so, save their name to the correct list of entries or externals and go the next line
 * if there was a label declaration in the line, add it to the label list
 * check if the lien represent a command line. if so, check if there was a label at the start of the line,
 *    set its address to be the current instruction counter and its type to instruction.
 *    increment the instruction counter by the number of words reserved for the instruction line. go to the next line
 * if the line is not any of the above, then it's not a valid line.
 * if at any point in the process an error occurred, raise an error flag, and keep going
 * at the end of the run, if anything went well
 *    the instruction counter is updated to hold the number of reserved spaces in memory for all the instruction codes
 *    the data counter is updated to hold the number of data elements in the hip
 *    and the label, entry and external lists holds
 */
int
make_first_pass(FILE *fil, short memory[], short data[], int *IC, int *DC, LabelList *labelList, EntryList *entryList,
                ExternalList *extList) {
    int currentIC, currentDC, index, lineNumber, errorFlag, answer;
    char line[MAX_LINE], firstWord[MAX_LINE];
    Label *currentLabel;

    currentIC = *IC;
    currentDC = *DC;

    index = 0;
    lineNumber = 0;
    errorFlag = FALSE;

    /*iterating through the lines of the file*/
    while (fgets(line, MAX_LINE + 2, fil)) {
        lineNumber++;

        currentLabel = NULL;
        index = move_to_not_white(line, MAX_LINE, 0);
        if (index == MAX_LINE || line[index] == ';' || line[index] == '\n')
            continue;
        insertWord(line, firstWord, &index);

        /*checking if we have a label*/
        if (is_label(firstWord) == TRUE) {
            if (is_valid_label(firstWord, lineNumber, labelList, extList) == TRUE) {
                currentLabel = initializeLabel(firstWord);
                currentLabel->address = currentDC;
            } else {
                errorFlag = TRUE;
                continue;
            }
        } else
            index = 0;

        /*checking if this is a data entry line*/
        answer = is_directing_command(line, lineNumber, index, data, &currentDC, currentLabel);
        if (answer == TRUE) {
            addLabelToList(labelList, currentLabel);
            continue;
        } else if (answer == ERROR)
            errorFlag = TRUE;
        /*entry or external declaration*/
        answer = extern_or_entry(line, index, lineNumber, entryList, extList);
        if (answer == TRUE) {
            if (currentLabel != NULL)
                freeLabel(currentLabel);
            continue;
        }

        if (answer == ERROR)
            errorFlag = TRUE;
        /*if we found a label we insert it to the symbol list*/
        if (currentLabel != NULL)
            addLabelToList(labelList, currentLabel);

        answer = is_command(line, memory, index, lineNumber, currentIC);
        if (answer != ERROR) {
            if (currentLabel != NULL) {
                currentLabel->address = currentIC + START_OF_MEMORY;
                currentLabel->type = INSTRUCTION;
            }
            currentIC += answer;
        } else
            errorFlag = TRUE;
    }


    *IC = currentIC;
    *DC = currentDC;

    return errorFlag;
}

/* A function that checks if we have a data,string or struct entry otherwise it returns 0
 * if a command was found the function returns the number of data the program entered or error in case
 * of an invalid data entry
 */
int
getDataTypeArguments(char *command, char line[], short data[], int positionInLine, int lineNumber, int DC) {
    if (!strcmp(command, ".data"))
        return process_data(line, data, positionInLine, lineNumber, DC);
    else if (!strcmp(command, ".string"))
        return process_string(line, data, positionInLine, lineNumber, DC);
    else if (!strcmp(command, ".struct"))
        return process_struct(line, data, positionInLine, lineNumber, DC);
    else
        return 0;
}


/*  The function checks if we  have a data entry line if so it uses a sub method to enter the data in to memory
 * and gives the label in case there is one, the address of the first word entered
 */
int is_directing_command(char line[], int lineNumber, int indexAfterLabel, short data[], int *DC,
                         Label *currentLabel) {
    int index, currentDC;
    char command[MAX_LINE];

    index = indexAfterLabel;
    currentDC = *DC;

    if (currentLabel != NULL) {
        if (line[indexAfterLabel] == '\n')
            return printERROR(lineNumber, "A Label without commands");
        else
            currentLabel->address = currentDC;

        index = move_to_not_white(line, MAX_LINE, index);
        /*this loop copies the command until it hits a space or a new line*/
    }

    /*we take the command*/
    insertWord(line, command, &index);
    int numberOfArguments;
    numberOfArguments = getDataTypeArguments(command, line, data, index, lineNumber, currentDC);

    if (numberOfArguments == ERROR)
        return ERROR;
    /*has a valid Label*/
    if (numberOfArguments == 0)
        return FALSE;
    else {
        /*giving the label the address of the first word*/
        *DC += numberOfArguments;
        currentLabel->type = DATA;
    }
    return TRUE;
}

/* A function that iterates through the line checks if there is an entry ofr external declaration
 * and inserts it in to a linked list of externals
 */
int extern_or_entry(char line[], int indexAfterLabel, int lineNumber, EntryList *entryList, ExternalList *extList) {
    int index;
    char temp[MAX_LINE];

    index = move_to_not_white(line, MAX_LINE, indexAfterLabel);
    insertWord(line, temp, &index);

    if (line[index] == '\n' || index >= MAX_LINE)
        return FALSE;


    if (!strcmp(temp, ".extern")) {
        index = move_to_not_white(line, MAX_LINE, index);

        /*if there was a call to extern without an argument*/
        if (line[index] == '\n' || index >= MAX_LINE)
            return printERROR(lineNumber, "reached end of line without name");

        insertWord(line, temp, &index);

        /*checking if the label already exists*/
        if (isNameExistAsEntryOrExternal(entryList, extList, temp))
            return printERROR(lineNumber, "external already exists");

        External *newExternal;
        newExternal = initializeExternal(temp);
        addExternalToList(extList, newExternal);
        return TRUE;

        /*dealing with entries*/
    } else if (!strcmp(temp, ".entry")) {
        index = move_to_not_white(line, MAX_LINE, index);
        /*declaration with no name*/
        if (line[index] == '\n' || index >= MAX_LINE)
            return printERROR(lineNumber, "reached end of line without name");

        insertWord(line, temp, &index);

        /*adding the entry to the list*/
        if (isNameExistAsEntryOrExternal(entryList, extList, temp))
            return printERROR(lineNumber, "entry already exists");

        Entry *newEntry;
        newEntry = initializeEntry(temp);
        addEntryToList(entryList, newEntry);
        return TRUE;
    }
    return FALSE;
}

/* A function the processes a data instructions and isolates each argument to make sure it
 * is a valid data entry
 */
int process_data(char line[], short data[], int positionInLine, int lineNumber, int dataCounter) {
    int argumentsCounter, tempCount, index, lineLength;
    short number;

    argumentsCounter = 0;
    tempCount = dataCounter;
    lineLength = (int) strlen(line);
    index = move_to_not_white(line, MAX_LINE, positionInLine);

    /*checking each number separately*/
    while (index < lineLength && line[index] != '\n') {
        number = getNumber(line, &index);
        if (number == FALSE)
            return printERROR(lineNumber, "invalid data entry");

        /*inserting the number in to the data array*/
        data[tempCount] = number;
        tempCount++;
        index++;
        index = move_to_not_white(line, MAX_LINE, index);
        argumentsCounter++;
    }
    /*returning how many arguments we entered*/
    return argumentsCounter;
}

/* the function gets a line and a position
 * and goes until reaching: comma or a space character or the end of the line.
 * the function then checks if it's a number, if so it returns the number
 * if not, it returns FALSE
 */
short getNumber(char *line, int *index) {
    int lineLength, numberStrIndex, tempIndex;
    char numberString[MAX_LINE] = {0};

    lineLength = (int) strlen(line);
    numberStrIndex = 0;
    tempIndex = move_to_not_white(line, lineLength, *index);

    while (line[tempIndex] != ',' && !isspace(line[tempIndex]) && tempIndex < lineLength)
        numberString[numberStrIndex++] = line[tempIndex++];

    if (!check_valid_data(numberString, numberStrIndex))
        return FALSE;
    short num;
    num = (short) atoi(numberString);
    *index = tempIndex;
    return num;
}

/* checks if the next position in line is a valid string,
 * if it is, add each character in the string to the data array and
 *  return the number of characters in the string.
 * if it's not a valid string, print an error message and return ERROR
 */
int process_string(char line[], short data[], int positionInLine, int lineNumber, int dataCounter) {
    int string_counter, index, stringLength;
    char string[MAX_LINE];

    string_counter = 0;
    insertWord(line, string, &positionInLine);
    stringLength = (int) strlen(string);

    if (string[0] != '"' || string[stringLength - 1] != '"')
        return printERROR(lineNumber, "missing start or end of string");

    for (index = 1; index < stringLength - 1; index++) {
        /*if the string has the \0 char init it's illegal*/
        if (!string[index])
            return printERROR(lineNumber, "illegal string definition");

        /*inserting each char separately*/
        data[dataCounter] = (short) string[index];
        dataCounter++;
        string_counter++;
    }
    /*we add 1 to include the \0*/
    return string_counter + 1;
}

/* checks if the next position in line is a valid struct, combining 1 number, a comma, and a string
 * if it is, add all the struct elements to the data array,
 *  and return the number of arguments in the structure.
 * if it's not a valid struct, print an error message and return ERROR
 */
int process_struct(char line[], short data[], int positionInLine, int lineNumber, int dataCounter) {
    int stringFieldLength, index;
    short numberField;

    index = move_to_not_white(line, MAX_LINE, positionInLine);
    numberField = getNumber(line, &index);

    /*checking the number is valid*/
    if (numberField == FALSE)
        return printERROR(lineNumber, "invalid number");

    index = move_to_not_white(line, MAX_LINE, index);
    /*making sure a comma separates the two filed*/
    if (line[index] != ',')
        return printERROR(lineNumber, "missing comma between the fields");

    /*processing the string*/
    stringFieldLength = process_string(line, data, index + 1, lineNumber, dataCounter + 1);
    if (stringFieldLength == ERROR)
        return ERROR;

    /*entering the number*/
    data[dataCounter] = numberField;

    /*combining the 1 number argument and the length of the string
     including the \0 character at the end of the string*/
    return 1 + stringFieldLength;
}

/* A function that gets a command and the directing methods of its operands and checks
 * if they are compatible
 */
int correct_operands(char command[], int source_address, int destination_address) {

    if (!strcmp(command, "lea")) {
        if (source_address == REACHED_END || destination_address == REACHED_END || destination_address == 0 ||
            source_address == 0 || source_address == 3)
            return FALSE;
        return TRUE;
    } else if (!strcmp(command, "mov") || !strcmp(command, "add") || !strcmp(command, "sub")) {
        if (source_address == REACHED_END || destination_address == REACHED_END || destination_address == 0)
            return FALSE;
        return TRUE;
    } else if (!strcmp(command, "cmp")) {
        if (source_address == REACHED_END || destination_address == REACHED_END)
            return FALSE;
        return TRUE;
    } else if (!strcmp(command, "not") || !strcmp(command, "clr") || !strcmp(command, "jmp") || !strcmp(command, "inc")
               || !strcmp(command, "dec") || !strcmp(command, "get") || !strcmp(command, "bne") ||
               !strcmp(command, "jsr")) {
        if (source_address == 0 || source_address == REACHED_END || destination_address != REACHED_END)
            return FALSE;
        return TRUE;
    } else if (!strcmp(command, "prn")) {
        if (source_address == REACHED_END || destination_address != REACHED_END)
            return FALSE;
        return TRUE;
    } else {
        /*commands hlt and rts*/
        if (source_address != REACHED_END || destination_address != REACHED_END)
            return FALSE;
        return TRUE;
    }
}


/* Function to check if a line is a command line
* return number of words we need to reserve
*/
int is_command(char line[], short memory[], int indexAfterLabel, int lineNumber, int IC) {
    char command[MAX_LINE], source[MAX_LINE], dest[MAX_LINE];
    int index, howMany, addressing, addressingDest, lineLength;

    OpWord *opWord;
    opWord = initializeOpWord();
    lineLength = (int) strlen(line);
    index = move_to_not_white(line, end_of_line(line), indexAfterLabel);
    insertWord(line, command, &index);

    /*checking if found the command*/
    if (!is_instruction(command))
        return ERROR;

    index = move_to_not_white(line, MAX_LINE, index);

    /*checking if we have an operand*/
    if (index >= lineLength || line[index] == '\n') {
        /*if we reached the end we have no operands*/
        addressing = REACHED_END;
        addressingDest = REACHED_END;
        if (correct_operands(command, addressing, addressingDest)) {
            addCommandOpcode(opWord, command);
            memory[IC] = opWordToBinary(opWord);
            return 1;
        }
    } else {
        insertWord(line, source, &index);
        /*checking the addressing method*/
        addressing = source_directing(source, lineNumber);   /*dont forget to remove*/
        if (addressing == ERROR)
            return printERROR(lineNumber, "incorrect addressing method");
    }
    index = move_to_not_white(line, MAX_LINE, index);
    /*looking for a second operand*/
    if (index >= lineLength || line[index] == '\n')
        addressingDest = REACHED_END;
    else {
        /*operands must be seperated by a comma*/
        if (line[index] != ',') {
            return printERROR(lineNumber, "missing comma between the fields");
        }
        index = move_to_not_white(line, MAX_LINE, index + 1);
        if (index >= lineLength || line[index] == '\n') {
            return printERROR(lineNumber, "missing second command");
        } else {
            insertWord(line, dest, &index);

            addressingDest = source_directing(dest, lineNumber);
            if (addressingDest == ERROR)
                return printERROR(lineNumber, "incorrect addressing method");

        }
        index = move_to_not_white(line, MAX_LINE, index);
        if (line[index] != '\n')
            return printERROR(lineNumber, "too many arguments in line");
    }/*checking if the addressing methods comply with the command*/
    if (correct_operands(command, addressing, addressingDest)) {
        howMany = how_many_ops(command);
        addCommandOpcode(opWord, command);
        /*if the command has only one operand we insert it to bits 3 4*/
        if (howMany == 1) {
            addTargetOp(opWord, addressing);

        } else {
            addSourceOp(opWord, addressing);
            addTargetOp(opWord, addressingDest);
        }
        /*we return how many extra words we need and plus one for the command*/
        memory[IC] = opWordToBinary(opWord);
        freeOpWord(opWord);

        return 1 + code_word(addressing, addressingDest, howMany);
    }
    return printERROR(lineNumber, "incompatible addressing methods");
}



