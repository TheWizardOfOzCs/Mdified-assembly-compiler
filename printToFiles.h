#ifndef PRINTTOFILES_H
#define PRINTTOFILES_H

#include "fileHandler.h"

#define BASE_ADDRESS 100
#define bits_5 31

#define OB ".ob"
#define ENT ".ent"
#define EXT ".ext"

#include "utils.h"
#include "labelHandler.h"

static const char symbols[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                               'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

/*converts a machine code to base 32*/
void convert_base_32(int word, char conversion[]);

/*a function that creates all the files*/
void
createFiles(char *name, short memory[], short data[], int IC, int DC, EntryList *entryList, ExternalList *externalList);

/*a function that creates the object file*/
void write_object_file(int DC, int IC, short memory[], short data[], char fileName[]);

/*a function that writes externals names and the addresses where they are being used*/
void write_extern(ExternalList *list, char fileName[]);

/*a function that prints all the labels that are entries*/
void write_entries(EntryList *list, char fileName[]);

/*a function that prints all the addresses in a linked list*/
void printAddressList(FILE *outputFile, addressList *list, char *labelName);

#endif
