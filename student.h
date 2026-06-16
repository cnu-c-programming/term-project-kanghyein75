#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student *next;
} Student;

void addStudent(Student **head, int id, const char *name, int score);
void freeStudents(Student **head);

#endif
