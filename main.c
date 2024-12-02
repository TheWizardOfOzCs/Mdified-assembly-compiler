#include "fileHandler.h"
#include "parser.h"
#include "processFile.h"

#define ASSEMBLY_EXTENSION ".as"


FILE *get_am_file(FILE *asFile, char *fileName);

FILE *get_am_file(FILE *asFile, char *fileName) {
    FILE *outputFile;
    outputFile = firstRun(asFile, fileName);
    fclose(asFile);
    if (outputFile != NULL) {
        printf("%s.am file has been created\n", fileName);
        return outputFile;
    } else {
        printf("could not creat %s.am file\n", fileName);
        return NULL;
    }
}

int main(int argc, char *argv[]) {
    FILE *file;
    int fileNumber;
    char fileName[MAX_NAME_LENGTH];
    for (fileNumber = 1; fileNumber < argc; fileNumber++) {
        strcpy(fileName, argv[fileNumber]);
        if ((file = openFile(fileName, ASSEMBLY_EXTENSION, "r"))) {
            printf("processing %s.as file\n", fileName);
            FILE *amFile;
            amFile = get_am_file(file, fileName);
            if (amFile != NULL) {
                amFile = openFile(fileName, ".am", "r");
                processFile(amFile, fileName);
            }
        } else {
            printf("can't open %s\n", fileName);
            continue;
        }
        printf("\n");
    }

    return 0;
}

