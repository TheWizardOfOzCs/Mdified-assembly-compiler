
#ifndef LABELHANDLER_H
#define LABELHANDLER_H

#include "utils.h"

#define MAX_LABEL_NAME 30
#define INSTRUCTION 10
#define DATA 11

/* a structure for label. containing:
 * name of the label
 * address
 * type: data or instruction
 * and the next label in the node
 */
typedef struct Label {
    char name[MAX_LABEL_NAME];
    unsigned int address;
    int type;
    struct Label *next;
} Label;

/* a list of labels holding a pointer
 * for the first label and the last
 */
typedef struct LabelList {
    Label *firstLabel;
    Label *lastLabel;
} LabelList;

/* a structure for holding a record of all
 * instructions where an external has been used
 */
typedef struct addressHistory {
    unsigned short address;
    struct addressHistory *next;
} addressHistory;

/* a list of address history holding a pointer
 * for the first address and the last
 */
typedef struct addressList {
    addressHistory *firstAddress;
    addressHistory *lastAddress;
} addressList;

/* a structure for entry. containing:
 * name of the entry
 * address
 * and the next entry in the node
 */
typedef struct Entry {
    char name[MAX_LABEL_NAME];
    unsigned int address;
    struct Entry *next;
} Entry;

/* a list of entries holding a pointer
 * for the first entry and the last
 */
typedef struct EntryList {
    Entry *firstEntry;
    Entry *lastEntry;
} EntryList;

/* a structure for externals. containing:
 * name of the external
 * list of addresses for each line the external had been called
 * and the next external in the node
 */
typedef struct External {
    char name[MAX_LABEL_NAME];
    addressList *addressList;
    struct External *next;
} External;

/* a list of externals holding a pointer
 * for the first external and the last
 */
typedef struct ExternalList {
    External *firstExternal;
    External *lastExternal;
} ExternalList;

/*is exist methods for the label, entry and external lists*/

int is_valid_label(char str[], int line_number, LabelList *labList, ExternalList *extList);

Label *isLabelExist(LabelList *list, char *labelName);

int is_label(char word[]);

External *isExternalExist(ExternalList *list, char *name);

Entry *isEntryExist(EntryList *list, char *name);

int isNameExistAsEntryOrExternal(EntryList *entryList, ExternalList *extList, char *name);

/*adding elements to list functions*/

void addEntryToList(EntryList *list, Entry *entry);

void addLabelToList(LabelList *list, Label *label);

void addExternalToList(ExternalList *list, External *external);

addressList *addAddress(addressList *list, int address);

/* initialization functions for all structures */

addressHistory *initializeAddressHistory(short address);

addressList *initializeAddressList();

Label *initializeLabel(char *labelName);

LabelList *initializeLabelList();

Entry *initializeEntry(char *entryName);

EntryList *initializeEntryList();

External *initializeExternal(char *externalName);

ExternalList *initializeExternalList();

/* free structures functions*/
void freeAddresses(addressHistory *history);

void freeLabel(Label *label);

void freeEntry(Entry *entry);

void freeExternal(External *external);

void freeEntryList(EntryList *list);

void freeLabelList(LabelList *list);

void freeExternalList(ExternalList *list);

void freeAddressList(addressList *list);

#endif
