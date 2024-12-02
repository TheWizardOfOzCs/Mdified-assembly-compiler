#include "printToFiles.h"
#include "utils.h"


/*A function that converts a number to base 32
 * @param short word the command we wish to convert
 * @param char conversion[] an array that holds the conversion*/
void convert_base_32(int word, char conversion[]) {
    /*taking the five right bits*/
    int number = word & bits_5;
    /*finding the corresponding representation for the number */
    conversion[1] = symbols[number];
    int number2 = (word >> 5) & bits_5;
    conversion[0] = symbols[number2];
    conversion[2] = NULL;
}

void createFiles(char *name, short memory[],short data[],int IC, int DC, EntryList *entryList, ExternalList *externalList) {

    write_object_file(DC, IC, memory,data, name);
    write_entries(entryList, name);
    write_extern(externalList, name);

    freeExternalList(externalList);
    freeEntryList(entryList);

}

/*function to crate an object file that prints the addresses and the machine code in base 32*/
void write_object_file(int DC, int IC, short memory[],short data[],char fileName[]) {
    FILE *outputFile;
    char conversion1[3] = {0};
    char conversion2[3] = {0};
    int i = 0;

    outputFile = openFile(fileName, OB, "w");

    if (!outputFile) {
        printf("can't create %s%s outputFile\n", fileName, OB);
        return;
    }
    printf("creating %s%s outputFile\n", fileName, OB);

    /*converting the IC and DC in base 32*/
    convert_base_32(IC, conversion1);
    convert_base_32(DC, conversion2);

    fprintf(outputFile, "   %s %s\n", conversion1, conversion2);
    for (; i < IC; i++) {
        /*converting the address and the content of the instruction*/
        char address[3];
        convert_base_32(i + BASE_ADDRESS, address);
        char word[3];
        convert_base_32(memory[i], word);

        fprintf(outputFile, "%s\t%s\n", address, word);
    }
    /*printing the struct content*/
    for (i = 0; i < DC; i++) {
        char address[3] = {0};
        convert_base_32(i + BASE_ADDRESS + IC, address);
        char word[3] = {0};
        convert_base_32(data[i], word);

        fprintf(outputFile, "%s\t%s\n", address, word);
    }
    fclose(outputFile);
    printf("%s%s had been made\n", fileName, OB);
}

/*function to print all teh entries and their addresses*/
void write_entries(EntryList *list, char fileName[]) {
    Entry *temp;
    temp = list->firstEntry;

    /*if we have no entries we don't create th outputFile*/
    if (temp == NULL)
        return;
    /*getting the address of the first use of the entry*/

    FILE *outputFile;
    outputFile = openFile(fileName, ENT, "w");

    if (!outputFile) {
        printf("can't create %s%s file\n", fileName, ENT);
        return;
    }
    printf("creating %s%s file\n", fileName, ENT);

    /*while we have more entries*/
    while (temp != NULL) {
        char address[3] = {0};
        convert_base_32((int)temp->address, address);
        fprintf(outputFile,"%s\t%s\n",temp->name,address);
        temp = temp->next;
    }
    fclose(outputFile);

    printf("%s%s had been made\n", fileName, ENT);

}

/*function to print all the external and their addresses*/
void write_extern(ExternalList *list, char fileName[]) {
    External *temp;
    temp = list->firstExternal;
    /*if we have no externals we don't create th file*/
    if (temp == NULL)
        return;
    /*getting the address of the first use of the extern*/


    FILE *outputFile;
    outputFile = openFile(fileName, EXT, "w");

    if (!outputFile) {
        printf("can't create %s%s outputFile\n", fileName, EXT);
        return;
    }
    printf("creating %s%s outputFile\n", fileName, EXT);

    /*while we have more externals*/
    while (temp != NULL) {
        if (temp->addressList != NULL)
            printAddressList(outputFile, temp->addressList, temp->name);
        temp = temp->next;
    }
    fclose(outputFile);

    printf("%s%s had been made\n", fileName, EXT);
}

void printAddressList(FILE *outputFile, addressList *list, char *labelName) {
    addressHistory *address = list->firstAddress;
    while (address != NULL) {
        char address1[3] = {0};
        convert_base_32(address->address, address1);
        fprintf(outputFile, "%s\t%s\n", labelName, address1);
        address = address->next;
    }
}