#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

void loadStudents(Student **head, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return; 
    }

    char line[500];
    
    if (fgets(line, sizeof(line), fp) != NULL) {
        
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
    
    (void)head; 
}

void saveStudents(Student *head, const char *filename) {
    (void)head;
    (void)filename;
}
