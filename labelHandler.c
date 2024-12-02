#include "labelHandler.h"


/* checks if the label name is valid
 * if it's not valid, return ERROR
 * otherwise, change str to be the label name, and return TRUE
 */
int is_valid_label(char str[], int line_number, LabelList *labList, ExternalList *extList) {
    int index, length, i;
    char labelName[MAX_LABEL_NAME];

    length = (int) strlen(str) - 1;
    strcpy(labelName, str);
    labelName[length] = '\0';

    index = move_to_not_white(str, MAX_LINE, 0);

    if (length > MAX_LABEL_NAME)
        return printERROR(line_number, "Label too long");

    if (!isalpha(str[index]))
        return printERROR(line_number, "A Label must start with a letter");

    for (i = index; i < length; i++)
        if (isspace(str[i]) || !(isalnum(str[i])))
            return printERROR(line_number, "there shouldn't be any spaces or non-alpha chars in the Label");

    if (isspace(str[length - 1]))
        return printERROR(line_number, "Can't have a space between Label and :");

    if (is_instruction(labelName) || is_directing(labelName) || is_register(labelName))
        return printERROR(line_number, "A Label's name can't be a reserved word in assembly");

    if (isLabelExist(labList, labelName) != NULL || isExternalExist(extList, labelName))
        return printERROR(line_number, "Label already exists");


    strcpy(str, labelName);
    return TRUE;
}

/* checks if the label name already exist in the list
 */
Label *isLabelExist(LabelList *list, char *labelName) {
    Label *labTemp;
    labTemp = list->firstLabel;
    while (labTemp != NULL) {
        if (!strcmp(labTemp->name, labelName)) {
            return labTemp;
        }
        labTemp = labTemp->next;
    }
    return NULL;
}

/* checks if the given name exist in the entry list
 * if it exists, return the entry
 * else return NULL
 */
Entry *isEntryExist(EntryList *list, char *name) {
    Entry *temp;
    temp = list->firstEntry;
    while (temp != NULL) {
        if (!strcmp(temp->name, name))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/* checks if the given name exist in the external list
 * if it exists, return the external
 * else return NULL
 */
External *isExternalExist(ExternalList *list, char *name) {
    External *temp;
    temp = list->firstExternal;
    while (temp != NULL) {
        if (!strcmp(temp->name, name))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/* checks if a name exist in the external or the entry lists
 */
int isNameExistAsEntryOrExternal(EntryList *entryList, ExternalList *extList, char *name) {
    return (isEntryExist(entryList, name) != NULL) || (isExternalExist(extList, name) != NULL);
}

/* checks if the word given to the function is a label declaration
*/
int is_label(char word[]) {
    int strLength;

    strLength = (int) strlen(word);
    if (word[strLength - 1] == ':')
        return TRUE;
    else
        return FALSE;
}

/* add a new label with a given name to the label list
 * return the new label
 */
void addLabelToList(LabelList *list, Label *label) {
    if (list->firstLabel == NULL) {
        list->firstLabel = label;
        list->lastLabel = label;
    } else {
        list->lastLabel->next = label;
        list->lastLabel = label;
    }
}

/* add a new entry with a given name to the entry list
 * return the new entry
 */
void addEntryToList(EntryList *list, Entry *entry) {
    if (list->firstEntry == NULL) {
        list->firstEntry = entry;
        list->lastEntry = entry;
    } else {
        list->lastEntry->next = entry;
        list->lastEntry = entry;
    }
}

/* add a new external with a given name to the external list
 * return the new external
 */
void addExternalToList(ExternalList *list, External *external) {
    if (list->firstExternal == NULL) {
        list->firstExternal = external;
        list->lastExternal = external;
    } else {
        list->lastExternal->next = external;
        list->lastExternal = external;
    }
}

/* add a new address to the given address list
 */
addressList *addAddress(addressList *list, int address) {
    addressHistory *newAddress;
    newAddress= initializeAddressHistory(address);
    if (list == NULL) {
        list = initializeAddressList();
        list->firstAddress = newAddress;
        list->lastAddress = newAddress;
    } else {
        list->lastAddress->next = newAddress;
        list->lastAddress = newAddress;
    }
    return list;
}

/* initialize a new label with a given name
 * a new initialized label is initialized with the values for the fields:
 * address= -1
 * next= NULL
 * and return the new label
 */
Label *initializeLabel(char *labelName) {
    Label *label;
    label = (Label *) malloc(sizeof(Label));
    strcpy(label->name, labelName);
    label->address = -1;
    label->type = -1;
    label->next = NULL;

    return label;
}

/* initialize a new label list with:
 * firstLabel= NULL
 * lastLabel= NULL
 * and return the new label list
 */
LabelList *initializeLabelList() {
    LabelList *list;
    list = (LabelList *) malloc(sizeof(LabelList));
    list->firstLabel = NULL;
    list->lastLabel = NULL;

    return list;
}

/* initialize a new entry node
 */
Entry *initializeEntry(char *entryName) {
    Entry *entry;
    entry = (Entry *) malloc(sizeof(Entry));
    strcpy(entry->name, entryName);
    entry->next = NULL;
    entry->address = -1;
    return entry;
}

/* initialize a new label list with:
 * firstLabel= NULL
 * lastLabel= NULL
 * and return the new label list
 */
EntryList *initializeEntryList() {
    EntryList *list;
    list = (EntryList *) malloc(sizeof(EntryList));
    list->firstEntry = NULL;
    list->firstEntry = NULL;

    return list;
}


/* initialize a new external with a given name
 * a new initialized external is initialized with the values for the fields:
 * address= -1
 * next= NULL
 * and return the new external
 */
External *initializeExternal(char *externalName) {
    External *external;
    external = (External *) malloc(sizeof(External));
    strcpy(external->name, externalName);
    external->next = NULL;
    external->addressList = NULL;
    return external;
}

/* initialize a new external list with:
 * firstExternal= NULL
 * lastExternal= NULL
 * and return the new external list
 */
ExternalList *initializeExternalList() {
    ExternalList *list;
    list = (ExternalList *) malloc(sizeof(ExternalList));
    list->firstExternal = NULL;
    list->lastExternal = NULL;

    return list;
}

/* initialize a new address node
 */
addressHistory *initializeAddressHistory(short address) {
    addressHistory *addresses;
    addresses = (addressHistory *) malloc(sizeof(addressHistory));
    addresses->address = address;
    addresses->next = NULL;
    return addresses;
}

/* allocating space for a new address list
 */
addressList *initializeAddressList() {
    addressList *newList;
    newList = (addressList *) malloc(sizeof(addressList));
    newList->firstAddress = NULL;
    newList->lastAddress = NULL;
    return newList;
}

/* free the label and all the other labels coming after it in the node
 */
void freeLabel(Label *label) {
    Label *temp;
    while (label != NULL) {
        temp = label;
        label = label->next;
        free(temp);
        temp = NULL;
    }

}

/* send the first label to freeLabel and then free the label list
 */
void freeLabelList(LabelList *list) {
    freeLabel(list->firstLabel);
    free(list);
    list = NULL;
}

/* free the entry and all the other labels coming after it in the node
 */
void freeEntry(Entry *entry) {
    Entry *temp;
    while (entry != NULL) {
        temp = entry;
        entry = entry->next;
        free(temp);
        temp = NULL;
    }

}

/* send the first external to freeExternal and then free the external list
 */
void freeEntryList(EntryList *list) {
    freeEntry(list->firstEntry);
    free(list);
    list = NULL;
}

/* free the external and all the other externals coming after it in the node
 */
void freeExternal(External *external) {
    External *temp;
    while (external != NULL) {
        temp = external;
        external = external->next;
        if (temp->addressList != NULL)
            freeAddressList(temp->addressList);
        free(temp);
        temp = NULL;
    }
}

/* send the first external to freeExternal and then free the external list
 */
void freeExternalList(ExternalList *list) {
    freeExternal(list->firstExternal);
    free(list);
    list = NULL;
}

/* send the first address to freeAddresses and the free the address list
 */
void freeAddressList(addressList *list) {
    freeAddresses(list->firstAddress);
    free(list);
    list = NULL;
}

/* free the address and all the other addresses coming after it in the node
 */
void freeAddresses(addressHistory *history) {
    addressHistory *temp;
    while (history != NULL) {
        temp = history;
        history = history->next;
        free(temp);
        temp = NULL;
    }
}