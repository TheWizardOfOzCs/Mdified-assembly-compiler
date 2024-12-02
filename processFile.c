
#include "processFile.h"


void initializeVariables(short data[], short memory[]);

/* process the post macro file.
 * send the file for the firs run to process, if any errors been found, don't proceed for the second process.
 * otherwise, update the labels and entries addresses and send the file for the second process.
 * if during the second process there were no errors, create output files for the file.
 */
int processFile(FILE *amFile, char *fileName) {
    int answer, IC, DC;
    short data[RAM], memory[RAM];

    IC = 0, DC = 0;

    LabelList *labelList = initializeLabelList();
    EntryList *entryList = initializeEntryList();
    ExternalList *externalList = initializeExternalList();

    initializeVariables(data, memory);

    answer = make_first_pass(amFile, memory, data, &IC, &DC, labelList, entryList, externalList);

    if (answer == TRUE)
        return printERROR(-1, "an error has been found during process");
    else {
        updateData(IC, labelList, entryList);

        rewind(amFile);
        /*start second process*/
        IC = make_second_pass(amFile, memory, labelList, externalList);

        if (IC != ERROR) {
            createFiles(fileName, memory, data, IC, DC, entryList, externalList);
        } else
            return printERROR(-1, "an Error had been found. can't create output files");
    }

    return 0;
}

/* set all the data and memory cells to 0
 */
void initializeVariables(short data[], short memory[]) {
    memset(data, 0, RAM);
    memset(memory, 0, RAM);
}

/* update all data labels addresses to be at their location+IC+start of memory (usually 100)
 */
void updateDataLabelAddresses(LabelList *labelList, int IC) {
    Label *labelTemp;
    labelTemp = labelList->firstLabel;
    while (labelTemp != NULL) {
        if (labelTemp->type == DATA)
            labelTemp->address += IC + START_OF_MEMORY;

        labelTemp = labelTemp->next;
    }
}

/* add to each entry the address of the co-responding label defined in the file
 */
void addAddressesToEntries(LabelList *lblList, EntryList *entryList) {
    Label *labelTemp;
    Entry *entryTemp;
    entryTemp = entryList->firstEntry;
    while (entryTemp != NULL) {
        labelTemp = lblList->firstLabel;
        while (labelTemp != NULL) {
            if (!strcmp(entryTemp->name, labelTemp->name)) {
                entryTemp->address = labelTemp->address;
                break;
            }
            labelTemp = labelTemp->next;
        }
        entryTemp = entryTemp->next;
    }
}

/* update all addresses of the data labels and put all data at the end of memory stack
 */
void updateData(int IC, LabelList *labelList, EntryList *entryList) {
    updateDataLabelAddresses(labelList, IC);
    addAddressesToEntries(labelList, entryList);
}
