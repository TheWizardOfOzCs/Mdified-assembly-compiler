
#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include "utils.h"
#include "firstpass.h"
#include "secondpass.h"
#include "printToFiles.h"


int processFile(FILE *amFile, char *fileName);

void updateDataLabelAddresses(LabelList *labelList, int IC);

void addAddressesToEntries(LabelList *lblList, EntryList *entryList);

void updateData(int IC, LabelList *labelList, EntryList *entryList);

#endif
