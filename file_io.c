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
        int id, score;
        char name[32];

        if (sscanf(line, "%d, %31[^,], %d", &id, name, &score) == 3) {
            addStudent(head, id, name, score);
        }
    }

    fclose(fp);
    
}

void saveStudents(Student *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Cannot open file to save.\n");
        return;
    }

    fprintf(fp, "id,name,score\n");

    Student *current = head;
    while (current != NULL) {
        fprintf(fp, "%d,%s,%d\n", current->id, current->name, current->score);
        current = current->next;
    }

    fclose(fp);
}
