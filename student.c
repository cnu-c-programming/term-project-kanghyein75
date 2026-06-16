#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void addStudent(Student **head, int id, const char *name, int score) {
    Student *newN = (Student *)malloc(sizeof(Student));
    if (newN == NULL) return; 

    newN->id = id;
    
    strncpy(newN->name, name, 31);
    newN->name[31] = '\0'; 
    
    newN->score = score;
    newN->next = NULL;
    
    if (*head == NULL) {
        *head = newN;
    } else {
        Student *current = *head;

        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newN;
    }
}

void freeStudents(Student **head) {
    Student *current = *head;
    Student *nextN; 

    while (current != NULL) {
        nextN = current->next; 
        free(current);            
        current = nextN;      
    }

    *head = NULL;
}
