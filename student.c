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

int deleteStudent(Student **head, int id) {
    if (*head == NULL) return 0;

    Student *temp = *head;
    
    if (temp->id == id) {
        *head = temp->next; 
        free(temp);         
        return 1;
    }

    Student *prev = NULL;
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return 0;

    prev->next = temp->next;
    free(temp); 
    return 1;
}

int updateStudent(Student *head, int id, int newScore) {
    Student *current = head;
    
    while (current != NULL) {
        if (current->id == id) {
            current->score = newScore; 
            return 1;
        }
        current = current->next;
    }
    return 0; 
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
